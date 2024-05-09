#pragma once
#include "./physical-component.hpp"

#include <glm/glm.hpp>

namespace our
{

    class RocketComponent : public PhysicalComponent
    {
    public:
        static std::string getID() { return "rocket"; }
        void deserialize(const nlohmann::json &data) override;
    };

}