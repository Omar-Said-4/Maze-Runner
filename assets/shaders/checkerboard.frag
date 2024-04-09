#version 330 core

out vec4 frag_color;

// In this shader, we want to draw a checkboard where the size of each tile is (size x size).
// The color of the top-left most tile should be "colors[0]" and the 2 tiles adjacent to it
// should have the color "colors[1]".

//TODO: (Req 1) Finish this shader.

uniform int size = 32;
uniform vec3 colors[2];

void main(){
    int tileY = int(gl_FragCoord.y)/size;   // Gets the tile Y coordinate
    int tileX = int(gl_FragCoord.x)/size;   // Gets the tile X coordinate
    int colorInd = (tileX + tileY ) % 2;
    frag_color = vec4(colors[colorInd],1.0); // Color the tile pixels based on its position
}