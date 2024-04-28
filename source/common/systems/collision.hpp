#pragma once

#include "../ecs/world.hpp"
#include "../maze/maze.hpp"

#include "../components/camera.hpp"
#include "../components/coin.hpp"
#include "../components/free-camera-controller.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include "../application.hpp"

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
                    glm::vec3 coinPosition = glm::vec3(entity->getLocalToWorldMatrix() * glm::vec4(entity->localTransform.position, 1.0));
                    if (abs(position.x - coinPosition.x) < 0.4 && abs(position.z - coinPosition.z) < 0.1)
                    {
                        std::cout << "COIN!" << std::endl;
                    }
                }
            }
        }
    };

}