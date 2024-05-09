#pragma once

#include "../ecs/world.hpp"
#include "../components/camera.hpp"
#include "../components/wall.hpp"
#include"../components/door.hpp"
#include "../components/free-camera-controller.hpp"
#include "systems/sound-system.hpp"
#include "systems/game-actions.hpp"
#include "systems/collision.hpp"
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
            if (our::GameActionsSystem::getSpeedUp()&&!our::GameActionsSystem::getGravityUp()&&!our::GameActionsSystem::getGravityDown()) // replace left shift with speedUp
                current_sensitivity *= controller->speedupFactor;

            // We change the camera position based on the keys WASD/QE
            // S & W moves the player back and forth
            // If the move makes a collision, we revert it
            bool collided = false;
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

            collided = detectBarrierCollision(world, position);
            crossedMazeBorders = detectMazeBordersCrossing(world, position);
            if (collided)
                position -= glm::vec3(0.2, 0.0, 0.2) * front * (deltaTime * current_sensitivity.z); // was (0.2,0.2,0.2)

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

            collided = detectBarrierCollision(world, position);
            crossedMazeBorders = detectMazeBordersCrossing(world, position);

            if (collided)
                position += glm::vec3(0.2, 0.0, 0.2) * front * (deltaTime * current_sensitivity.z); // was (0.2,0.2,0.2)
            // Q & E moves the player up and down
            if (app->getKeyboard().isPressed(GLFW_KEY_Q))
                position += 0 * (deltaTime * current_sensitivity.y); // was up became 0
            if (our::GameActionsSystem::getGravityUp()){
                position += up * (deltaTime * 10);
                position.x = our::GameActionsSystem::getGravityX();
                position.z = our::GameActionsSystem::getGravityZ();
            }
            if (our::GameActionsSystem::getGravityDown()){
                position -= up * (deltaTime * 10);
                position.x = our::GameActionsSystem::getGravityX();
                position.z = our::GameActionsSystem::getGravityZ();
            }
            collided = detectBarrierCollision(world, position);
            if (collided)
                position -= 0 * (deltaTime * current_sensitivity.y); // was up became 0
            if (app->getKeyboard().isPressed(GLFW_KEY_E))
                position -= 0 * (deltaTime * current_sensitivity.y); // was up became
            collided = detectBarrierCollision(world, position);
            if (collided)
                position += 0 * (deltaTime * current_sensitivity.y); // was up became 0
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
            collided = detectBarrierCollision(world, position);
            crossedMazeBorders = detectMazeBordersCrossing(world, position);
            if (crossedMazeBorders)
            {
                position=our::GameActionsSystem::cameraPosition;
                rotation=our::GameActionsSystem::cameraRotation;
                return;
            }

            if (collided)
                position -= right * (deltaTime * current_sensitivity.x);
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
            collided = detectBarrierCollision(world, position);
            crossedMazeBorders = detectMazeBordersCrossing(world, position);

            if (collided)
                position += right * (deltaTime * current_sensitivity.x);
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

         bool detectBarrierCollision(World *world, glm::vec3 &position)
        {
            auto entities = world->getEntities();

            for (auto entity : entities)
            {
                glm::vec3 entityPosition = glm::vec3(entity->getLocalToWorldMatrix()[3]);

                if (entity->getComponent<DoorComponent>())
                {
                    float minX = entityPosition.x - 2.0f;
                    float maxX = entityPosition.x + 2.0f;
                    float minZ = entityPosition.z - 14.0f;
                    float maxZ = entityPosition.z + 14.0f;
                    bool isInsideDoorCell = (position.x >= minX && position.x <= maxX && position.z >= minZ && position.z <= maxZ);

                    if (isInsideDoorCell)
                    {
                        //std::cout << "DOOR" << std::endl;
                        return true;
                    }
                }
                else if (entity->getComponent<WallComponent>())
                {
                    if (CollisionSystem::checkCollision(position, entityPosition, entity->getComponent<WallComponent>()))
                    {
                        if (our::GameActionsSystem::getPortal()&&!our::GameActionsSystem::getGravityUp()&&!our::GameActionsSystem::getGravityDown())
                        {
                            our::GameActionsSystem::portalStateInc();

                            return false;
                        }
                        return true;
                    }
                }
            }

            return false;
        }
    };

}
