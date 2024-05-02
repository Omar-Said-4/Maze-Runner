#pragma once
#include "../ecs/component.hpp"

#include <glm/glm.hpp>

namespace our
{

    class KeyComponent : public Component
    {
    public:
        static std::string getID() { return "key"; }
        void deserialize(const nlohmann::json &data) override;
    };

}