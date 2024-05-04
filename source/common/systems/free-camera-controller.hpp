#pragma once

#include "../ecs/world.hpp"
#include "../components/collision.hpp"
#include "../components/camera.hpp"
#include "../components/free-camera-controller.hpp"
#include "systems/sound-system.hpp"
#include "systems/game-actions.hpp"
#include "../application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

namespace our
{

    // The free camera controller system is responsible for moving every entity which contains a FreeCameraControllerComponent.
    // This system is added as a slightly complex example for how use the ECS framework to implement logic.
    // For more information, see "common/components/free-camera-controller.hpp"
    class FreeCameraControllerSystem
    {
        Application *app;          // The application in which the state runs
        bool mouse_locked = false; // Is the mouse locked

    public:
        // When a state enters, it should call this function and give it the pointer to the application
        void enter(Application *app)
        {
            this->app = app;
        }

        void handlePhysicalBarrierCollision(glm::vec3 &position, const glm::vec3 &front, const glm::vec3 &right, const glm::vec3 &up, float deltaTime, glm::vec3 current_sensitivity)
        {
            if (app->getKeyboard().isPressed(GLFW_KEY_W))
                position -= glm::vec3(0.2, 0.2, 0.2) * front * (deltaTime * (current_sensitivity.z));

            if (app->getKeyboard().isPressed(GLFW_KEY_S))
                position += glm::vec3(0.2, 0.2, 0.2) * front * (deltaTime * current_sensitivity.z);

            // A & D moves the player left or right
            if (app->getKeyboard().isPressed(GLFW_KEY_D))
                position -= right * (deltaTime * current_sensitivity.x);

            if (app->getKeyboard().isPressed(GLFW_KEY_A))
                position += right * (deltaTime * current_sensitivity.x);

            // Prevent player from climbing walls and doors
            position -= up * (deltaTime * (current_sensitivity.y * 0.5f));
        }

        bool checkCollision(const glm::vec3 &playerPosition, const glm::vec3 &targetPosition, CollisionComponent *collisionComponent)
        {
            float minX = targetPosition.x - collisionComponent->collisionCellX / 2;
            float maxX = targetPosition.x + collisionComponent->collisionCellX / 2;
            float minZ = targetPosition.z - collisionComponent->collisionCellZ / 2;
            float maxZ = targetPosition.z + collisionComponent->collisionCellZ / 2;

            if (playerPosition.x >= minX && playerPosition.x <= maxX && playerPosition.z >= minZ && playerPosition.z <= maxZ)
            {
                // Play collision sound if given one
                our::SoundSystem::play_custom_sound(collisionComponent->soundName, false, false);
                return true;
            }

            // No collision
            return false;
        }

        bool detectMazeBordersCrossing(World *world, glm::vec3 &position)
        {
            std::pair<int, int> xMazeBorders = world->getXBordersOfMaze();
            std::pair<int, int> zMazeBorders = world->getZBordersOfMaze();
            if (position.x < xMazeBorders.first || position.x > xMazeBorders.second ||
                position.z > zMazeBorders.first || position.z < zMazeBorders.second)
            {
                std::cout << "Crossed maze borders" << std::endl;
                return true;
            }
            return false;
        }

        // This should be called every frame to update all entities containing a FreeCameraControllerComponent
        void update(World *world, float deltaTime)
        {

            // if door is open game is done no need for the camer controller
            if (our::GameActionsSystem::getOpenDoor())
            {
                return;
            }
            // First of all, we search for an entity containing both a CameraComponent and a FreeCameraControllerComponent
            // As soon as we find one, we break
            CameraComponent *camera = nullptr;
            FreeCameraControllerComponent *controller = nullptr;
            for (auto entity : world->getEntities())
            {
                camera = entity->getComponent<CameraComponent>();
                controller = entity->getComponent<FreeCameraControllerComponent>();
                if (camera && controller)
                    break;
            }
            // If there is no entity with both a CameraComponent and a FreeCameraControllerComponent, we can do nothing so we return
            if (!(camera && controller))
                return;
            // Get the entity that we found via getOwner of camera (we could use controller->getOwner())
            Entity *entity = camera->getOwner();

            // If the left mouse button is pressed, we lock and hide the mouse. This common in First Person Games.
            if (app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1) && !mouse_locked)
            {
                app->getMouse().lockMouse(app->getWindow());
                mouse_locked = true;
                // If the left mouse button is released, we unlock and unhide the mouse.
            }
            else if (!app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1) && mouse_locked)
            {
                app->getMouse().unlockMouse(app->getWindow());
                mouse_locked = false;
            }

            // We get a reference to the entity's position and rotation
            glm::vec3 &position = entity->localTransform.position;
            glm::vec3 &rotation = entity->localTransform.rotation;

            // If the left mouse button is pressed, we get the change in the mouse location
            // and use it to update the camera rotation
            if (app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1))
            {
                glm::vec2 delta = app->getMouse().getMouseDelta();
                if (!our::GameActionsSystem::getGravityUp() && !our::GameActionsSystem::getGravityDown())
                {
                    rotation.x -= delta.y * controller->rotationSensitivity; // The y-axis controls the pitch
                }
                rotation.y -= delta.x * controller->rotationSensitivity; // The x-axis controls the yaw
            }

            // We prevent the pitch from exceeding a certain angle from the XZ plane to prevent gimbal locks
            if (rotation.x < -glm::half_pi<float>() * 0.99f)
                rotation.x = -glm::half_pi<float>() * 0.99f;
            if (rotation.x > glm::half_pi<float>() * 0.99f)
                rotation.x = glm::half_pi<float>() * 0.99f;
            // This is not necessary, but whenever the rotation goes outside the 0 to 2*PI range, we wrap it back inside.
            // This could prevent floating point error if the player rotates in single direction for an extremely long time.
            rotation.y = glm::wrapAngle(rotation.y);

            // We update the camera fov based on the mouse wheel scrolling amount
            float fov = camera->fovY + app->getMouse().getScrollOffset().y * controller->fovSensitivity;
            fov = glm::clamp(fov, glm::pi<float>() * 0.01f, glm::pi<float>() * 0.99f); // We keep the fov in the range 0.01*PI to 0.99*PI
            camera->fovY = fov;

            // We get the camera model matrix (relative to its parent) to compute the front, up and right directions
            glm::mat4 matrix = entity->localTransform.toMat4();

            glm::vec3 front = glm::vec3(matrix * glm::vec4(0, 0, -1, 0)),
                      up = glm::vec3(matrix * glm::vec4(0, 1, 0, 0)),
                      right = glm::vec3(matrix * glm::vec4(1, 0, 0, 0));

            glm::vec3 current_sensitivity = controller->positionSensitivity;
            // If the LEFT SHIFT key is pressed, we multiply the position sensitivity by the speed up factor
            if (our::GameActionsSystem::getSpeedUp() && !our::GameActionsSystem::getGravityUp() && !our::GameActionsSystem::getGravityDown()) // replace left shift with speedUp
                current_sensitivity *= controller->speedupFactor;

            // We change the camera position based on the keys WASD/QE
            // S & W moves the player back and forth
            // If the move makes a collision, we revert it
            bool crossedMazeBorders = false;
            if (app->getKeyboard().isPressed(GLFW_KEY_W) && !our::GameActionsSystem::getGravityUp() && !our::GameActionsSystem::getGravityDown())
            {
                position += glm::vec3(0.2, 0.0, 0.2) * front * (deltaTime * (current_sensitivity.z)); // was (0.2,0.2,0.2)
                if (our::GameActionsSystem::getSpeedUp())
                {
                    our::SoundSystem::play_custom_sound("RUN", false, false);
                }
                else
                {
                    our::SoundSystem::play_custom_sound("WALK", false, false);
                }
            }

            crossedMazeBorders = detectMazeBordersCrossing(world, position);

            if (app->getKeyboard().isPressed(GLFW_KEY_S) && !our::GameActionsSystem::getGravityUp() && !our::GameActionsSystem::getGravityDown())
            {
                position -= glm::vec3(0.2, 0.0, 0.2) * front * (deltaTime * current_sensitivity.z); // was (0.2,0.2,0.2)
                if (our::GameActionsSystem::getSpeedUp())
                {
                    our::SoundSystem::play_custom_sound("RUN", false, false);
                }
                else
                {
                    our::SoundSystem::play_custom_sound("WALK", false, false);
                }
            }

            crossedMazeBorders = detectMazeBordersCrossing(world, position);

            if (our::GameActionsSystem::getGravityUp())
            {
                position += up * (deltaTime * 10);
                position.x = our::GameActionsSystem::getGravityX();
                position.z = our::GameActionsSystem::getGravityZ();
            }
            if (our::GameActionsSystem::getGravityDown())
            {
                position -= up * (deltaTime * 10);
                position.x = our::GameActionsSystem::getGravityX();
                position.z = our::GameActionsSystem::getGravityZ();
            }

            // A & D moves the player left or right
            if (app->getKeyboard().isPressed(GLFW_KEY_D) && !our::GameActionsSystem::getGravityUp() && !our::GameActionsSystem::getGravityDown())
            {
                position += right * (deltaTime * current_sensitivity.x);
                if (our::GameActionsSystem::getSpeedUp())
                {
                    our::SoundSystem::play_custom_sound("RUN", false, false);
                }
                else
                {
                    our::SoundSystem::play_custom_sound("WALK", false, false);
                }
            }

            crossedMazeBorders = detectMazeBordersCrossing(world, position);
            if (crossedMazeBorders)
            {
                position = our::GameActionsSystem::cameraPosition;
                rotation = our::GameActionsSystem::cameraRotation;
                return;
            }

            if (app->getKeyboard().isPressed(GLFW_KEY_A) && !our::GameActionsSystem::getGravityUp() && !our::GameActionsSystem::getGravityDown())
            {
                position -= right * (deltaTime * current_sensitivity.x);
                if (our::GameActionsSystem::getSpeedUp())
                {
                    our::SoundSystem::play_custom_sound("RUN", false, false);
                }
                else
                {
                    our::SoundSystem::play_custom_sound("WALK", false, false);
                }
            }

            for (auto entity : world->getEntities())
            {
                auto collisionComponent = entity->getComponent<CollisionComponent>();
                if (collisionComponent)
                {
                    switch (collisionComponent->collisionType)
                    {
                    case CollisionType::WALL:
                    {
                        // Calculate world-space positions of wall's center
                        glm::vec3 wallPosition = glm::vec3(entity->getLocalToWorldMatrix()[3]);

                        if (checkCollision(position, wallPosition, collisionComponent))
                        {
                            if (our::GameActionsSystem::getPortal() && !our::GameActionsSystem::getGravityUp() && !our::GameActionsSystem::getGravityDown())
                            {
                                our::GameActionsSystem::portalStateInc();
                            }
                            else
                            {
                                handlePhysicalBarrierCollision(position, front, right, up, deltaTime, current_sensitivity);
                            }
                        }
                    }
                    break;
                    case CollisionType::GROUND:
                    {
                        glm::vec3 groundPosition = glm::vec3(entity->getLocalToWorldMatrix()[3]);
                        if (position.y < (groundPosition.y + collisionComponent->playerHeight))
                        {
                            position.y = (groundPosition.y + collisionComponent->playerHeight);
                        }
                    }
                    break;
                    case CollisionType::BOLT:
                    {
                        // Calculate world-space positions of the bolt
                        glm::vec3 boltPosition = glm::vec3(entity->getLocalToWorldMatrix()[3]);

                        if (checkCollision(position, boltPosition, collisionComponent))
                        {
                            our::GameActionsSystem::setSpeedUp();
                            our::GameActionsSystem::collectPowerup();
                            our::GameActionsSystem::resetPowerupTimer(our::powerups::speedUp);
                            world->markForRemoval(entity);
                        }
                    }
                    break;
                    case CollisionType::ROCKET:
                    {
                        // Calculate world-space positions of the rocket
                        glm::vec3 rocketPosition = glm::vec3(entity->getLocalToWorldMatrix()[3]);

                        if (checkCollision(position, rocketPosition, collisionComponent))
                        {
                            our::GameActionsSystem::setGravityUp();
                            our::GameActionsSystem::collectPowerup();
                            our::GameActionsSystem::resetPowerupTimer(our::powerups::gravityUp);
                            world->markForRemoval(entity);
                            our::GameActionsSystem::setGravity(rocketPosition.x, rocketPosition.z);
                        }
                    }
                    break;
                    case CollisionType::KEY:
                    {
                        // Calculate world-space positions of the key
                        glm::vec3 keyPosition = glm::vec3(entity->getLocalToWorldMatrix()[3]);

                        if (checkCollision(position, keyPosition, collisionComponent))
                        {
                            our::GameActionsSystem::collectKey();
                            world->markForRemoval(entity);
                        }
                    }
                    break;
                    case CollisionType::PORTAL:
                    {
                        // Calculate world-space positions of the portal
                        glm::vec3 portalPosition = glm::vec3(entity->getLocalToWorldMatrix()[3]);

                        if (checkCollision(position, portalPosition, collisionComponent))
                        {
                            our::GameActionsSystem::collectPowerup();
                            our::GameActionsSystem::portalStateInc();
                            world->markForRemoval(entity);
                        }
                    }
                    break;
                    case CollisionType::MASTER_KEY:
                    {
                        glm::vec3 masterKeyPosition = glm::vec3(entity->getLocalToWorldMatrix()[3]);

                        if (checkCollision(position, masterKeyPosition, collisionComponent))
                        {
                            if (our::GameActionsSystem::getKeysCollected() != our::GameActionsSystem::getTotalKeys() && !our::GameActionsSystem::getCantCollectMasterKey())
                            {
                                our::SoundSystem::play_custom_sound("NOCOLLECT", false, false);
                                our::GameActionsSystem::setCantCollectMasterKey();
                            }
                            else if (our::GameActionsSystem::getKeysCollected() == our::GameActionsSystem::getTotalKeys())
                            {
                                our::SoundSystem::play_custom_sound("KEY2", false, false);
                                our::GameActionsSystem::collectExitKey();
                                world->markForRemoval(entity);
                            }
                        }
                    }
                    break;
                    case CollisionType::COIN:
                    {
                        glm::vec3 coinPosition = glm::vec3(entity->getLocalToWorldMatrix()[3]);
                        bool isTaken = entity->localTransform.scale.x == 0.0f;

                        if (checkCollision(position, coinPosition, collisionComponent) && !isTaken)
                        {
                            world->markForRemoval(entity);
                            our::GameActionsSystem::collectCoin();
                        }
                    }
                    break;
                    case CollisionType::DOOR:
                    {
                        // Update door start angle
                        our::GameActionsSystem::doorStartAngle = entity->localTransform.rotation.y;
                        // Calculate world-space positions of the door
                        glm::vec3 doorPosition = glm::vec3(entity->getLocalToWorldMatrix()[3]);

                        if (checkCollision(position, doorPosition, collisionComponent))
                        {
                            our::SoundSystem::play_custom_sound(collisionComponent->soundName, false, false);
                            handlePhysicalBarrierCollision(position, front, right, up, deltaTime, current_sensitivity);

                            if (our::GameActionsSystem::getOpenDoor())
                            {
                                entity->localTransform.rotation.y = (our::GameActionsSystem::doorStartAngle - 0.3 * (our::GameActionsSystem::getPowerupTimer(powerups::door)));
                                return;
                            }
                            if (abs(position.x - doorPosition.x) < 2.3 && abs(position.z - doorPosition.z) < 10.8)
                            {
                                if (!our::GameActionsSystem::getExitKey() && !our::GameActionsSystem::getTouchDoor())
                                {
                                    our::SoundSystem::play_custom_sound("NOCOLLECT", false, false);
                                    our::GameActionsSystem::setTouchDoor();
                                }
                                else if (our::GameActionsSystem::getExitKey())
                                {
                                    our::SoundSystem::play_custom_sound("DOOR", false, false);
                                    our::GameActionsSystem::setOpenDoor();
                                }
                            }
                        }
                    }
                    break;
                    }
                }
            }
        }

        // When the state exits, it should call this function to ensure the mouse is unlocked
        void exit()
        {
            if (mouse_locked)
            {
                mouse_locked = false;
                app->getMouse().unlockMouse(app->getWindow());
            }
        }
    };
}
