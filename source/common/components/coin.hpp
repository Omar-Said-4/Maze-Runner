#pragma once
#include "./physical-component.hpp"

#include <glm/glm.hpp>

namespace our
{

    class CoinComponent : public PhysicalComponent
    {
    public:
        static std::string getID() { return "coin"; }
        void deserialize(const nlohmann::json &data) override;
    };

}