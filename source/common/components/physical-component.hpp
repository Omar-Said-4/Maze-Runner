#pragma once
#include "../ecs/component.hpp"

#include <glm/glm.hpp>

namespace our
{

    class PhysicalComponent : public Component
    {
    public:
        float collisionCellX;
        float collisionCellZ;
        std::string soundName;
        void deserialize(const nlohmann::json &data) override;
    };

}