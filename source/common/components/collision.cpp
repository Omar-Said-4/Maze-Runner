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
        else if (collisionTypeStr == "GROUND")
        {
            collisionType = CollisionType::GROUND;
        }
        else if (collisionTypeStr == "COIN")
        {
            collisionType = CollisionType::COIN;
        }
        else if (collisionTypeStr == "BOLT")
        {
            collisionType = CollisionType::BOLT;
        }
        else if (collisionTypeStr == "ROCKET")
        {
            collisionType = CollisionType::ROCKET;
        }
        else if (collisionTypeStr == "KEY")
        {
            collisionType = CollisionType::KEY;
        }
        else if (collisionTypeStr == "PORTAL")
        {
            collisionType = CollisionType::PORTAL;
        }
        else if (collisionTypeStr == "MASTER_KEY")
        {
            collisionType = CollisionType::MASTER_KEY;
        }
        else if (collisionTypeStr == "DOOR")
        {
            collisionType = CollisionType::DOOR;
        }

        collisionCellX = data.value("collisionCellX", 12.9f);
        collisionCellZ = data.value("collisionCellZ", 12.9f);
        playerHeight = data.value("playerHeight", 5.0f);
        soundName = data.value("soundName", "");
    }
}
