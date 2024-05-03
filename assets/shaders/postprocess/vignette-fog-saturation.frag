#version 330

// Uniform variable representing the texture holding the scene pixels
uniform sampler2D tex;
uniform float time;

// Input variable representing the texture coordinates of the fragment
in vec2 tex_coord;

// Output variable representing the final color of the fragment
out vec4 frag_color;


void main(){
    // Sample the scene texture to get the color of the current pixel
    vec4 sceneColor = texture(tex, tex_coord);

    // Apply vignette effect:

    // Convert texture coordinates to normalized device coordinates (NDC) to apply the vignette effect later
    vec2 ndc_coord = tex_coord * 2.0 - 1.0;

    // Calculate the vignette intensity based on the distance from the center of the screen
    float vignette = 1.0 / (1.0 + dot(ndc_coord, ndc_coord));

    // Boost saturation:

    // Intensity factors for RGB channels used to calculate the luminance of the scene color
    vec3 intensity = vec3(0.2126, 0.7152, 0.0722);

    // Calculate the luminance of the scene color by taking the dot product with intensity factors
    float luminance = dot(sceneColor.rgb, intensity);

    // Boost the saturation of the scene color by blending between its grayscale representation and the original color
    // 2.2 is a saturation boost factor, adjust as needed
    sceneColor.rgb = mix(vec3(luminance), sceneColor.rgb, 1.8);

    // Apply vignette effect:

    // Multiply the scene color by the vignette intensity to apply the vignette effect
    sceneColor.rgb *= vignette;

    float fogMovement = sin(time)* 0.05;
    // Introduce fog based on fragment depth:
    float depth = (tex_coord.y/2 + fogMovement) +(tex_coord.x/2+fogMovement); 

    // Define fog color
    vec3 fogColor = vec3(0.5, 0.5, 0.5); // Adjust fog color as needed

    // Define fog parameters
    float fogStart = 0.2; // Start of fog
    float fogEnd = 1.0; // End of fog
    
    // Calculate fog intensity based on fragment depth
    float fogIntensity = smoothstep(fogStart, fogEnd, abs(depth));
    fogIntensity += smoothstep(fogStart, fogEnd, abs(tex_coord.x - 0.8)); // Use the absolute difference between texture x-coordinate and the center
    // Modulate fog intensity based on time to make fog appear to move
    fogIntensity += fogMovement ; // Adjust the amplitude and speed of fog movement as needed
    // Mix the scene color with the fog color based on fog intensity
    sceneColor.rgb = mix(sceneColor.rgb, fogColor, fogIntensity);

    // Output the final color for the fragment
    frag_color = sceneColor;
}
