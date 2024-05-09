#pragma once
#include "./physical-component.hpp"

#include <glm/glm.hpp>

namespace our
{

    class WallComponent : public PhysicalComponent
    {
    public:
        static std::string getID() { return "wall"; }
        void deserialize(const nlohmann::json &data) override;
    };

}