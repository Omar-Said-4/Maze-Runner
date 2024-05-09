#pragma once
#include "./physical-component.hpp"

#include <glm/glm.hpp>

namespace our
{

    class MasterKeyComponent : public PhysicalComponent
    {
    public:
        static std::string getID() { return "master-key"; }
        void deserialize(const nlohmann::json &data) override;
    };

}