#pragma once

#include "../ecs/component.hpp"

#include <glm/mat4x4.hpp>
#include <string>

namespace our
{

    // An enum that defines the type of the collision
    enum class CollisionType
    {
        COIN,
        WALL,
        BOLT,
        ROCKET,
        KEY,
        PORTAL,
        MASTER_KEY,
        DOOR,
        GROUND
    };

    class CollisionComponent : public Component
    {
    public:
        CollisionType collisionType; // The type of the collision
        float collisionCellX;
        float collisionCellZ;
        float playerHeight;
        std::string soundName;
        // The ID of this component type is "Collision"
        static std::string getID() { return "Collision"; }

        // Reads collision parameters from the given json object
        void deserialize(const nlohmann::json &data) override;
    };

}