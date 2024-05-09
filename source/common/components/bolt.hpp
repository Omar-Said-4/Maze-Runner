#pragma once
#include "./physical-component.hpp"

#include <glm/glm.hpp>

namespace our
{

    class BoltComponent : public PhysicalComponent
    {
    public:
        static std::string getID() { return "bolt"; }
        void deserialize(const nlohmann::json &data) override;
    };

}