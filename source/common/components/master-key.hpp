#pragma once
#include "../ecs/component.hpp"

#include <glm/glm.hpp>

namespace our
{

    class MasterKeyComponent : public Component
    {
    public:
        static std::string getID() { return "master-key"; }
        void deserialize(const nlohmann::json &data) override;
    };

}