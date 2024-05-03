#include "master-key.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our
{

    void MasterKeyComponent::deserialize(const nlohmann::json &data)
    {
        if (!data.is_object())
            return;
    }
}