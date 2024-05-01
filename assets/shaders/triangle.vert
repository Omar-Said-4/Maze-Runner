#version 330

// This vertex shader should be used to render a triangle whose normalized device coordinates are:
// (-0.5, -0.5, 0.0), ( 0.5, -0.5, 0.0), ( 0.0,  0.5, 0.0)
// And it also should send the vertex color as a varying to the fragment shader where the colors are (in order):
// (1.0, 0.0, 0.0), (0.0, 1.0, 0.0), (0.0, 0.0, 1.0)

out Varyings {
    vec3 color;
} vs_out;

// Currently, the triangle is always in the same position, but we don't want that.
// So two uniforms should be added: translation (vec2) and scale (vec2).
// Each vertex "v" should be transformed to be "scale * v + translation".
// The default value for "translation" is (0.0, 0.0) and for "scale" is (1.0, 1.0).

// Uniform variable that specifies the x and y components of the translation vector
uniform vec2 translation = vec2(0.0, 0.0);
// Uniform variable that specifies the x and y components of the scale factor
uniform vec2 scale = vec2(1.0, 1.0);

//TODO: (Req 1) Finish this shader

void main(){
    // Array of vertices positions
    vec3 verticesPositions[3] = vec3[3](
    vec3(-0.5, -0.5, 0.0),
    vec3(0.5, -0.5, 0.0),
    vec3(0.0, 0.5, 0.0)
    );

    // Array of vertices colors
    vec3 verticesColors[3] = vec3[3](
    vec3(1.0, 0.0, 0.0),
    vec3(0.0, 1.0, 0.0),
    vec3(0.0, 0.0, 1.0)
    );

    vec3 modifiedVertexPosition = vec3(vec3(scale, 0.0) * verticesPositions[gl_VertexID] + vec3(translation, 0.0));

    // Pass the vertex color to the fragment shader
    vs_out.color = verticesColors[gl_VertexID];

    // Convert the modified vertex position to a vec4 
    gl_Position = vec4(modifiedVertexPosition, 1.0);

}