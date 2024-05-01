#pragma once
#include "../ecs/component.hpp"

#include <glm/glm.hpp>

namespace our
{

    class CoinComponent : public Component
    {
    public:
        static std::string getID() { return "coin"; }
        void deserialize(const nlohmann::json &data) override;
    };

}