#pragma once
#include "./physical-component.hpp"

#include <glm/glm.hpp>

namespace our
{

    class KeyComponent : public PhysicalComponent
    {
    public:
        static std::string getID() { return "key"; }
        void deserialize(const nlohmann::json &data) override;
    };

}