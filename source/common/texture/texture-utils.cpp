#include "texture-utils.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <iostream>

// added to get the glm functions
#include<glm/glm.hpp>
our::Texture2D* our::texture_utils::empty(GLenum format, glm::ivec2 size){
    our::Texture2D* texture = new our::Texture2D();
    //TODO: (Req 11) Finish this function to create an empty texture with the given size and format

    // bind the texture to set its parameters
    texture->bind();
    
    // get the number of mipmap levels

    GLsizei levels=(GLsizei)glm::floor(glm::log2((float)glm::max(size.x, size.y)))+1;

    
    // allocate the storage for the texture
    // target => GL_TEXTURE_2D
    // levels => levels
    // internalformat on gpu => GL_RGBA8
    // width => size.r
    // height => size.y
    glTexStorage2D(GL_TEXTURE_2D, levels, GL_RGBA8, size.r, size.y);

   // unbind the texture
    texture->unbind();
    
    return texture;
}

our::Texture2D* our::texture_utils::loadImage(const std::string& filename, bool generate_mipmap) {
    glm::ivec2 size;
    int channels;
    //Since OpenGL puts the texture origin at the bottom left while images typically has the origin at the top left,
    //We need to till stb to flip images vertically after loading them
    stbi_set_flip_vertically_on_load(true);
    //Load image data and retrieve width, height and number of channels in the image
    //The last argument is the number of channels we want and it can have the following values:
    //- 0: Keep number of channels the same as in the image file
    //- 1: Grayscale only
    //- 2: Grayscale and Alpha
    //- 3: RGB
    //- 4: RGB and Alpha (RGBA)
    //Note: channels (the 4th argument) always returns the original number of channels in the file
    unsigned char* pixels = stbi_load(filename.c_str(), &size.x, &size.y, &channels, 4);
    if(pixels == nullptr){
        std::cerr << "Failed to load image: " << filename << std::endl;
        return nullptr;
    }
    // Create a texture
    our::Texture2D* texture = new our::Texture2D();
    //Bind the texture such that we upload the image data to its storage
    //TODO: (Req 5) Finish this function to fill the texture with the data found in "pixels"
    
    // bind the texture
    texture->bind();

    // create a new two dimensional texture object in memory and initialize it with data.
    // type => GL_TEXTURE_2D
    // level (original size) => 0
    // internalformat on gpu => GL_RGBA8
    // width => size.x
    // height => size.y
    // border => 0 must be 0
    // format of the pixel data => GL_RGBA
    // type of the pixel data => GL_UNSIGNED_BYTE
    // data => pixels
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size.r, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    // generate mipmap if needed
    if(generate_mipmap){
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    // unbind the texture
    texture->unbind();
    
    stbi_image_free(pixels); //Free image data after uploading to GPU
    return texture;
}