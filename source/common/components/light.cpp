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
        else if (lightTypeStr == "EMISSIVE")
        {
            lightType = LightType::EMISSIVE;
        }
        else if (lightTypeStr == "AMBIENT")
        {
            lightType = LightType::AMBIENT;
        }
        if (lightType != LightType::AMBIENT)
        {
            // Read the color of the light
            glm::vec3 a = glm::vec3(1.0f);
            color = new glm::vec3(data.value("color", glm::vec3(1.0f, 0.9f, 0.8f)));
        }
        else
        {
            skyLight = new AmbientDir();
            skyLight->top = data.value("top", glm::vec3(0.0f,0.2f,0.5f));
            skyLight->bottom = data.value("bottom", glm::vec3(0.0f,0.0f,0.0f));
            skyLight->horizon = data.value("horizon", glm::vec3(0.2f,0.2f,0.2f));
        }
        if (lightType == LightType::POINT || lightType == LightType::SPOT)
        {
            // Read attenuation of the light
            attenuation = new glm::vec3(data.value("attenuation", glm::vec3(1.0f, 0.0f, 0.0f)));
        }
        if (lightType == LightType::SPOT)
        {
            // Read the cone angles of the light
            coneAngles = new glm::vec2(data.value("coneAngles", glm::vec2(glm::quarter_pi<float>(), glm::half_pi<float>())));

        }
   }
}