#version 330

// Input vertex attributes
layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 tex_coord;
layout(location = 3) in vec3 normal;

// model matrix
uniform mat4 M;

// model inverse transpose matrix for normal vectors
uniform mat4 M_IT;

// view projection matrix
uniform mat4 VP;

// to compute specular
uniform vec3 camera_position;

// output variables
out Varyings {
    vec4 color;
    vec2 tex_coord;
    vec3 normal;
    vec3 view;
    vec3 world;
} vs_out;

void main()
{
    // to get world vector
    vec3 world_position = (M * vec4(position, 1.0)).xyz;
    // position on window
    gl_Position = VP * vec4(world_position, 1.0);
    
    // attributes to be passed as it is
    vs_out.color = color;
    vs_out.tex_coord = tex_coord;
    // normal position in the world space 
    vs_out.normal = normalize((M_IT * vec4(normal, 0.0)).xyz);
    // passing world position
    vs_out.world = world_position;
    // vector used in frag shader to compute specular
    vs_out.view = camera_position - world_position;

}
