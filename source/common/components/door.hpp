#pragma once
#include "./physical-component.hpp"

#include <glm/glm.hpp>

namespace our
{

    class DoorComponent : public PhysicalComponent
    {
    public:
        static std::string getID() { return "door"; }
        void deserialize(const nlohmann::json &data) override;
    };

}