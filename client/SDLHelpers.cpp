#include "SDLHelpers.h"

#include <iostream>
#include <cmath>


bool loadTexture(GLuint* tex, const char* name, int mode) 
{
    SDL_Surface* TextureImage = SDL_LoadBMP(name); 
    if (!TextureImage) {
        std::cerr << "loading of texture " << name << " failed ..." << std::endl;
        return false;
    }

    // create texture 
    glGenTextures(1, tex);

    // typical texture generation using data from the bitmap 
    glBindTexture(GL_TEXTURE_2D, *tex);

    TextureImage = toRGB(TextureImage);

    // generate alpha channel 
    if (mode) {
        char* rgba = (char*)malloc(4 * TextureImage->w * TextureImage->h);
        char* rgb = (char*)TextureImage->pixels;

        for(int i = 0; i < (TextureImage->w * TextureImage->h); ++i) {
            rgba[4*i+0] = rgb[3*i+0];
            rgba[4*i+1] = rgb[3*i+1];
            rgba[4*i+2] = rgb[3*i+2];

            if ((rgb[3*i+2] == (mode & 0xff) >> 0) 
                 && (rgb[3*i+1] == (mode & 0xff00) >> 8) 
                 && (rgb[3*i+0] == (mode & 0xff0000) >> 16))
                rgba[4*i+3] = 0;
            else
                rgba[4*i+3] = (char)255;
        }

        // generate the texture 
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TextureImage->w,
                     TextureImage->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba);
#ifdef FILTER    
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, TextureImage->w, TextureImage->h, 
                          GL_RGBA, GL_UNSIGNED_BYTE, rgba); 
#else
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
#endif
        free(rgba);
    } 
    else {  
        // generate the texture 
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, TextureImage->w, TextureImage->h, 0, 
                     GL_RGB, GL_UNSIGNED_BYTE, TextureImage->pixels);

#ifdef FILTER    
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, TextureImage->w, TextureImage->h, 
                          GL_RGB, GL_UNSIGNED_BYTE, TextureImage->pixels); 
#else
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
#endif
    }

    // linear filtering 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    SDL_FreeSurface(TextureImage);

    return true;
}


SDL_Surface* toRGB(SDL_Surface* pSurface) 
{
    // I need an SDL_PixelFormat, for RGB 24 bit format, struct 
    SDL_Surface* tmp = SDL_CreateRGBSurface(SDL_SWSURFACE, 0, 0, 24, 0x000000ff, 0x0000ff00, 
                                            0x00ff0000, 0x00000000);

    // convert given surface to rgb format 
    SDL_Surface* rgbSurface = SDL_ConvertSurface(pSurface, tmp->format, SDL_SWSURFACE); 
    SDL_FreeSurface(pSurface);
    SDL_FreeSurface(tmp);
    return rgbSurface;
}


