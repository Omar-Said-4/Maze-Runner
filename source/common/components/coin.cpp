#include "coin.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our
{

    void CoinComponent::deserialize(const nlohmann::json &data)
    {
        PhysicalComponent::deserialize(data);
        
        if (!data.is_object())
            return;
    }
}