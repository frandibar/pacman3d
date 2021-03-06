#ifndef CONTROLS_H_INCLUDED
#define CONTROLS_H_INCLUDED

#include "lib/utilities.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <string>

class Controls
{
public:
    Controls();  // constructor
    ~Controls();

    void init();
    void showStatistics(int width, int height, const std::string& msg, tStats stats);
    void draw(int screenWidth, int screenHeight, int playerId, bool menu, bool status, std::string chat);
    void toggleSound() { _sound = !_sound; }
    void toggleMusic() { _music = !_music; }

private:
    bool buildFont();
    GLvoid glPrint(GLint x, GLint y, const char *string, ...);
    void drawMenu(int screenWidth, int screenHeight);
    void drawLogo(int screenWidth, int screenHeight);
    void draw2dStatus(int screenWidth, int screenHeight, int playerId, GLfloat fps, std::string chat);

    // member variables
    bool _sound;                // sound on/off
    bool _music;                // music on/off
    GLuint _glFont;             // font texture id
    GLuint _glLogo;             // logo texture id
    GLuint _base;               // font display list  
};

#endif
