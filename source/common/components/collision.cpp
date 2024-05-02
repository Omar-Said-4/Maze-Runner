#include "collision.hpp"
#include "../ecs/entity.hpp"
#include <glm/glm.hpp>
#include "../deserialize-utils.hpp"
#include <glm/gtc/constants.hpp>

namespace our
{
    // Reads collision parameters from the given json object
    void CollisionComponent::deserialize(const nlohmann::json &data)
    {
        if (!data.is_object())
            return;
        // Read the collision type
        std::string collisionTypeStr = data.value("collisionType", "LAND");
        if (collisionTypeStr == "WALL")
        {
            collisionType = CollisionType::WALL;
        }
        else if (collisionTypeStr == "LAND")
        {
            collisionType = CollisionType::LAND;
        }
        else if (collisionTypeStr == "COIN")
        {
            collisionType = CollisionType::COIN;
        }
        else if (collisionTypeStr == "POWERUP")
        {
            collisionType = CollisionType::POWERUP;
        }

        collisionCellSize = data.value("collisionCellSize", 12.9f);
    }
}