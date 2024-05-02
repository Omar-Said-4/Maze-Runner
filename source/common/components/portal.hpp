#pragma once
#include "../ecs/component.hpp"

#include <glm/glm.hpp>

namespace our
{

    class PortalComponent : public Component
    {
    public:
        static std::string getID() { return "portal"; }
        void deserialize(const nlohmann::json &data) override;
    };

}