#include "./physical-component.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our
{

    void PhysicalComponent::deserialize(const nlohmann::json &data)
    {
        if (!data.is_object())
            return;

        collisionCellX = data.value("collisionCellX", 12.9f);
        collisionCellZ = data.value("collisionCellZ", 12.9f);
        soundName = data.value("soundName", "");
    }
}