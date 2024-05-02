#pragma once
#include "../ecs/component.hpp"

#include <glm/glm.hpp>

namespace our
{

    class RocketComponent : public Component
    {
    public:
        static std::string getID() { return "rocket"; }
        void deserialize(const nlohmann::json &data) override;
    };

}