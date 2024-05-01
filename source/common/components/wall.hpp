#pragma once
#include "../ecs/component.hpp"

#include <glm/glm.hpp>

namespace our
{

    class WallComponent : public Component
    {
    public:
        static std::string getID() { return "wall"; }
        void deserialize(const nlohmann::json &data) override;
    };

}