#pragma once

#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "GL/glew.h"

namespace GFX
{
    class Texture
    {
    public:
        // Data must be formatted as RGBA. 4 bytes per pixel (1 byte for each value).
        // If target is GL_TEXTURE_CUBE_MAP, data contains the data for 6 images in the order
        // of GL_TEXTURE_CUBE_MAP_POSITIVE_X ... GL_TEXTURE_CUBE_MAP_NEGATIVE_Z.
        Texture(const unsigned char* data, unsigned int width, unsigned int height, GLenum target);
        ~Texture();

        void bind(unsigned int unit);
        void unbind(unsigned int unit);

        unsigned int getWidth() const;
        unsigned int getHeight() const;

    private:
        unsigned int _width;
        unsigned int _height;
        GLenum _target;
        GLuint _textureId;

        void initialize(const unsigned char* data);
        void initialize2d(const unsigned char* data);
        void initializeCubeMap(const unsigned char* data);
    };
}

#endif //TEXTURE_HPP
