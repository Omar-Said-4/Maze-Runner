#include "sampler.hpp"

#include "deserialize-utils.hpp"

namespace our {

    // Given a json object, this function deserializes the sampler state
    void Sampler::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
        // if key is not found, use GL_LINEAR
        set(GL_TEXTURE_MAG_FILTER, (GLint)gl_enum_deserialize::texture_magnification_filters.at(data.value("MAG_FILTER", "GL_LINEAR")));
        // if key is not found, use GL_LINEAR_MIPMAP_LINEAR
        set(GL_TEXTURE_MIN_FILTER, (GLint)gl_enum_deserialize::texture_minification_filters.at(data.value("MIN_FILTER", "GL_LINEAR_MIPMAP_LINEAR")));
        // if key is not found, use GL_REPEAT
        set(GL_TEXTURE_WRAP_S, (GLint)gl_enum_deserialize::texture_wrapping_modes.at(data.value("WRAP_S", "GL_REPEAT")));
        // if key is not found, use GL_REPEAT
        set(GL_TEXTURE_WRAP_T, (GLint)gl_enum_deserialize::texture_wrapping_modes.at(data.value("WRAP_T", "GL_REPEAT")));
        // if key is not found, use 1.0f
        set(GL_TEXTURE_MAX_ANISOTROPY_EXT, data.value("MAX_ANISOTROPY", 1.0f));
        // if key is not found, use glm::vec4(0, 0, 0, 0)
        set(GL_TEXTURE_BORDER_COLOR, data.value("BORDER_COLOR", glm::vec4(0, 0, 0, 0)));
    }

}