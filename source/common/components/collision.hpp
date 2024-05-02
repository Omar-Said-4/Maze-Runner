#pragma once

#include "../ecs/component.hpp"

#include <glm/mat4x4.hpp>

namespace our
{

    // An enum that defines the type of the collision
    enum class CollisionType
    {
        COIN,
        WALL,
        POWERUP,
        LAND
    };

    class CollisionComponent : public Component
    {
    public:
        CollisionType collisionType; // The type of the collision
        float collisionCellSize;
        // The ID of this component type is "Collision"
        static std::string getID() { return "Collision"; }

        // Reads collision parameters from the given json object
        void deserialize(const nlohmann::json &data) override;
    };

}