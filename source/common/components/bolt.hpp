#pragma once
#include "../ecs/component.hpp"

#include <glm/glm.hpp>

namespace our
{

    class BoltComponent : public Component
    {
    public:
        static std::string getID() { return "bolt"; }
        void deserialize(const nlohmann::json &data) override;
    };

}