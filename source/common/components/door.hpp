#pragma once
#include "../ecs/component.hpp"

#include <glm/glm.hpp>

namespace our
{

    class DoorComponent : public Component
    {
    public:
        static std::string getID() { return "door"; }
        void deserialize(const nlohmann::json &data) override;
    };

}