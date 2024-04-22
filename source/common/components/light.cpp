#include "light.hpp"
#include "../ecs/entity.hpp"
#include <glm/glm.hpp>
#include "../deserialize-utils.hpp"
#include <glm/gtc/constants.hpp> 

namespace our
{
    // Reads light parameters from the given json object
    void LightComponent::deserialize(const nlohmann::json &data)
    {
        if (!data.is_object())
            return;
        // Read the light type
        std::string lightTypeStr = data.value("lightType", "DIRECTIONAL");
        if (lightTypeStr == "DIRECTIONAL")
        {
            lightType = LightType::DIRECTIONAL;
        }
        else if (lightTypeStr == "POINT")
        {
            lightType = LightType::POINT;
        }
        else if (lightTypeStr == "SPOT")
        {
            lightType = LightType::SPOT;
        }
       
       
        color = glm::vec3(data.value("color", glm::vec3(1.0f, 0.9f, 0.8f)));
        position = glm::vec3(data.value("position", glm::vec3(0.0f, 0.0f, 0.0f)));
        direction = glm::vec3(data.value("direction", glm::vec3(0.0f, -1.0f, 0.0f)));

        if (lightType == LightType::POINT || lightType == LightType::SPOT)
        {
            // Read attenuation of the light
            attenuation = new glm::vec3(data.value("attenuation", glm::vec3(0.0f, 1.0f, 0.0f)));
        }
        if (lightType == LightType::SPOT)
        {
            // Read the cone angles of the light
            coneAngles = new glm::vec2(data.value("coneAngles", glm::vec2(glm::quarter_pi<float>(), glm::half_pi<float>())));

        }
   }
}