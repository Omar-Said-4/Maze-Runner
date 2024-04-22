#pragma once

#include "../ecs/component.hpp"

#include <glm/mat4x4.hpp>

namespace our {

    // An enum that defines the type of the light 
    enum class LightType {
        DIRECTIONAL,
        POINT,
        SPOT,
        EMISSIVE,
        AMBIENT
    };
    struct AmbientDir {
        glm::vec3 top,bottom,horizon;
    };
    class LightComponent : public Component {
    public:
        LightType lightType; // The type of the light
        glm::vec3 * color=nullptr; // The color of the light
        glm::vec3 * attenuation=nullptr; // The attenuation of the light (d^2,d,1.0).
        glm::vec2 * coneAngles=nullptr; // The cone angles of the light (inner, outer) for spot light.
        AmbientDir* skyLight=nullptr;  // The sky light parameters
        // The ID of this component type is "Light"
        static std::string getID() { return "Light"; }

        // Reads light parameters from the given json object
        void deserialize(const nlohmann::json& data) override;

        ~LightComponent() {
            if(color)
            delete color;
            if(attenuation)
            delete attenuation;
            if(coneAngles)
            delete coneAngles;
            if(skyLight)
            delete skyLight;
        }
    };

}