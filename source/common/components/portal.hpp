#pragma once
#include "./physical-component.hpp"

#include <glm/glm.hpp>

namespace our
{

    class PortalComponent : public PhysicalComponent
    {
    public:
        static std::string getID() { return "portal"; }
        void deserialize(const nlohmann::json &data) override;
    };

}