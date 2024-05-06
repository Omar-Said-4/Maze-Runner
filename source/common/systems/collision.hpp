#pragma once

#include "../ecs/world.hpp"
#include "../maze/maze.hpp"

#include "../components/camera.hpp"
#include "../components/coin.hpp"
#include "../components/bolt.hpp"
#include "../components/rocket.hpp"
#include "../components/key.hpp"
#include "../components/portal.hpp"
#include "../components/master-key.hpp"
#include "../components/free-camera-controller.hpp"
#include "../components/door.hpp"
#include"../systems/sound-system.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include "../application.hpp"
#include"../systems/game-actions.hpp"
namespace our
{

    class CollisionSystem
    {
        Application *app;

    public:
        void enter(Application *app)
        {
            this->app = app;
        }

        void update(World *world, float deltaTime)
        {
            CameraComponent *camera = nullptr;
            FreeCameraControllerComponent *controller = nullptr;

            for (auto entity : world->getEntities())
            {
                camera = entity->getComponent<CameraComponent>();
                controller = entity->getComponent<FreeCameraControllerComponent>();
                if (camera && controller)
                    break;
            }

            if (!(camera && controller))
                return;
            Entity *entity = camera->getOwner();

            glm::vec3 &position = entity->localTransform.position;

            auto entities = world->getEntities();

            for (auto entity : entities)
            {
                if (entity->getComponent<CoinComponent>())
                {
                    // Calculate world-space positions of the coin
                    glm::vec3 coinPosition = glm::vec3(entity->getLocalToWorldMatrix()[3]);
                    if (abs(position.x - coinPosition.x) < 0.8 && abs(position.z - coinPosition.z) < 0.8)
                    {
                        //std::cout << "COIN!" << std::endl;
                        world->markForRemoval(entity);
                        our::SoundSystem::play_custom_sound("Collect",false,false);
                        our::GameActionsSystem::collectCoin();
                    }
                }
                else if(entity->getComponent<BoltComponent>())
                {
                    // Calculate world-space positions of the bolt
                    glm::vec3 boltPosition = glm::vec3(entity->getLocalToWorldMatrix()[3]);
                    if (abs(position.x - boltPosition.x) < 0.8 && abs(position.z - boltPosition.z) < 0.8)
                    {
                       // std::cout << "BOLT!" << std::endl;
                        our::SoundSystem::play_custom_sound("Powerup",false,false);
                        our::GameActionsSystem::setSpeedUp();
                        our::GameActionsSystem::collectPowerup();
                        our::GameActionsSystem::resetPowerupTimer(our::powerups::speedUp);
                        world->markForRemoval(entity);
                    }
                }
                else if(entity->getComponent<RocketComponent>())
                {
                    // Calculate world-space positions of the rocket
                    glm::vec3 rocketPosition = glm::vec3(entity->getLocalToWorldMatrix()[3]);
                    if (abs(position.x - rocketPosition.x) < 0.8 && abs(position.z - rocketPosition.z) < 0.8)
                    {
                        //std::cout << "ROCKET!" << std::endl;
                        our::SoundSystem::play_custom_sound("Powerup",false,false);
                        our::GameActionsSystem::setGravityUp();
                        our::GameActionsSystem::collectPowerup();
                        our::GameActionsSystem::resetPowerupTimer(our::powerups::gravityUp);
                        world->markForRemoval(entity);
                        our::GameActionsSystem::setGravity(rocketPosition.x,rocketPosition.z);
                    }
                }
                else if(entity->getComponent<KeyComponent>())
                {
                    // Calculate world-space positions of the key
                    glm::vec3 keyPosition = glm::vec3(entity->getLocalToWorldMatrix()[3]);
                    if (abs(position.x - keyPosition.x) < 0.8 && abs(position.z - keyPosition.z) < 0.8)
                    {
                       // std::cout << "KEY!" << std::endl;
                        our::SoundSystem::play_custom_sound("KEY1",false,false);
                        our::GameActionsSystem::collectKey();
                        world->markForRemoval(entity);
                    }
                }
                else if(entity->getComponent<PortalComponent>()&&our::GameActionsSystem::getPortalState()==portalState::off)
                {
                    // Calculate world-space positions of the portal
                    glm::vec3 portalPosition = glm::vec3(entity->getLocalToWorldMatrix()[3]);
                    if (abs(position.x - portalPosition.x) < 0.8 && abs(position.z - portalPosition.z) < 0.8)
                    {
                       // std::cout << "PORTAL!" << std::endl;
                        our::SoundSystem::play_custom_sound("Powerup",false,false);
                        our::GameActionsSystem::collectPowerup();
                        our::GameActionsSystem::portalStateInc();
                        world->markForRemoval(entity);
                    }
                }
                else if(entity->getComponent<MasterKeyComponent>())
                {
                    // Calculate world-space positions of the master key
                    glm::vec3 masterKeyPosition = glm::vec3(entity->getLocalToWorldMatrix()[3]);
                    if (abs(position.x - masterKeyPosition.x) < 0.8 && abs(position.z - masterKeyPosition.z) < 0.8)
                    {
                       // std::cout << "MASTER KEY!" << std::endl;
                        if(our::GameActionsSystem::getKeysCollected() != our::GameActionsSystem::getTotalKeys() && !our::GameActionsSystem::getCantCollectMasterKey())                        {
                            our::SoundSystem::play_custom_sound("NOCOLLECT",false,false);
                            our::GameActionsSystem::setCantCollectMasterKey();
                        }
                        else if (our::GameActionsSystem::getKeysCollected() == our::GameActionsSystem::getTotalKeys() )
                        {
                          our::SoundSystem::play_custom_sound("KEY2",false,false);
                          our::GameActionsSystem::collectExitKey();
                          world->markForRemoval(entity);
                            
                        }
                       
                    }
                }
                else if(entity->getComponent<DoorComponent>())
                {
                    // Calculate world-space positions of the door
                    glm::vec3 doorPosition = glm::vec3(entity->getLocalToWorldMatrix()[3]);
                    if( our::GameActionsSystem::getOpenDoor())
                    {
                        entity->localTransform.rotation.y=(our::GameActionsSystem::doorStartAngle-0.3*(our::GameActionsSystem::getPowerupTimer(powerups::door)));
                        return;
                    }
                    if (abs(position.x - doorPosition.x) < 2.3 && abs(position.z - doorPosition.z) < 10.8)
                    {
                        //std::cout << "DOOR!" << std::endl;
                        if(!our::GameActionsSystem::getExitKey()&& !our::GameActionsSystem::getTouchDoor())
                        {
                            our::SoundSystem::play_custom_sound("NOCOLLECT",false,false);
                            our::GameActionsSystem::setTouchDoor();
                        }
                        else if (our::GameActionsSystem::getExitKey())
                        {
                            our::SoundSystem::play_custom_sound("DOOR",false,false);
                            our::GameActionsSystem::setOpenDoor();
                        }
                    }
                }
            }
        }
    };

}