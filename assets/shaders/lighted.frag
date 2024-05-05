#version 330

#define DIRECTIONAL 0
#define POINT       1
#define SPOT        2
#define MAX_LIGHTS 64
in Varyings {
    vec4 color;
    vec2 tex_coord;
    vec3 normal;
    vec3 view;
    vec3 world;
} fs_in;

struct Material {
    // ability to  diffuse light
    sampler2D albedo;
    // reflection of light from a surface in a specific direction.
    sampler2D specular;
    // for surface shininess
    sampler2D roughness;
    // for more realistic ambient lighting
    sampler2D ambient_occlusion;
    // light emitted by a surface
    sampler2D emissive;
};

struct Light {
    // type of light
    int type;
    // position of light
    vec3 position;
    // direction of light rays from source
    vec3 direction;
    // color of light
    vec3 color;
    // attenuation of spot or point light
    vec3 attenuation;
    // cone angles for spot light
    vec2 cone_angles;
};


uniform Light lights[MAX_LIGHTS];
uniform int light_count;
// for more realistic lighting dependeing on normal
struct Sky {
    vec3 top, horizon, bottom;
};

uniform Sky sky;

vec3 compute_sky_light(vec3 normal){
    vec3 extreme = normal.y > 0 ? sky.top : sky.bottom;
    return mix(sky.horizon, extreme, normal.y * normal.y);
}


uniform Material material;
// input from vertex shader

out vec4 frag_color;
// for diffuse light computations
float lambert(vec3 normal, vec3 world_to_light_direction) {
    return max(0.0, dot(normal, world_to_light_direction));
}
// for specular light computations
float phong(vec3 reflected, vec3 view, float shininess) {
    return pow(max(0.0, dot(reflected, view)), shininess);
}

void main() {
    vec3 normal = normalize(fs_in.normal);
    vec3 view = normalize(fs_in.view);
    
    vec3 ambient_light = compute_sky_light(normal);
    // material properties from textures
    vec3 diffuse = texture(material.albedo, fs_in.tex_coord).rgb;
    vec3 specular = texture(material.specular, fs_in.tex_coord).rgb;
    float roughness = texture(material.roughness, fs_in.tex_coord).r;
    vec3 ambient = diffuse * texture(material.ambient_occlusion, fs_in.tex_coord).r;
    vec3 emissive = texture(material.emissive, fs_in.tex_coord).rgb;
    
    // This function clamps the value of "roughness" between 0.001 and 0.999. to avoid 0 and infinity values.
    float shininess = 2.0 / pow(clamp(roughness, 0.001, 0.999), 4.0) - 2.0;
    
    vec3 world_to_light_dir;
    // doesn't depend on any light source
    vec3 color = emissive + ambient_light * ambient;

    for(int light_idx = 0; light_idx < min(MAX_LIGHTS, light_count); light_idx++){
        Light light = lights[light_idx];
        float attenuation = 1.0;
        if(light.type == DIRECTIONAL){
              // invert vector for correct computations
            world_to_light_dir = -light.direction;
        } else {
             // correct direction for point and spot lights
            world_to_light_dir = light.position - fs_in.world;
             // normalize the direction
            float d = length(world_to_light_dir);
            world_to_light_dir /= d;
            // get attenuation based on distance and calculation method
            attenuation = 1.0 / dot(light.attenuation, vec3(d*d, d, 1.0));

            if(light.type == SPOT){
                 // angle from source
                float angle = acos(dot(light.direction, -world_to_light_dir));
                attenuation *= smoothstep(light.cone_angles.y, light.cone_angles.x, angle);
            }
        }
        // compute diffuse and specular light
        vec3 computed_diffuse = light.color * diffuse * lambert(normal, world_to_light_dir);

        vec3 reflected = reflect(-world_to_light_dir, normal);
        vec3 computed_specular = light.color * specular * phong(reflected, view, shininess);
        // get final pixel color value for this iteration
        color += (computed_diffuse + computed_specular) * attenuation;

    }
    //color=specular;
    //frag_color = vec4(color, 1.0);
    frag_color = vec4(color, 1.0);
}