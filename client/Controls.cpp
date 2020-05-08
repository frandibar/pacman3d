#include <cstdarg>

#include "Controls.h"
#include "Maze.h"
#include "Player.h"
#include "SDLHelpers.h"
#include "lib/debug.h"

std::string vMenu[] = {
                        "m    - Toggle Map view",
                        "F1   - Toggle Fullscreen",
                        "F2   - Toggle Video Mode (%dx%d)       ",
                        "F3   - Toggle Sound (%s)",
//                      "F4   - Toggle Music (%s)",
                        "F12  - Quit Game",
                        ""
                     };

std::string vStats[] = {
                        "Pills (eaten)      %d (%d)",
                        "Powerups (eaten)   %d (%d)",
                        "Bonus              %d",
                        "Ghosts             %d",
                        ""
                       };
// constructor
Controls::Controls() : _sound(true), _music(true), _glFont(0), _glLogo(0), _base(0)
{
}

// destructor
Controls::~Controls() 
{
    // delete all 95 font display lists 
    glDeleteLists(_base, 95);
}

void Controls::init()
{
    loadTexture(&_glLogo, "/etc/Pacman3D/data/logo.bmp", false);
    buildFont();
}

// build our font display list 
bool Controls::buildFont()
{
    SDL_Surface* image = NULL;

    glGenTextures(1, &_glFont); // create the texture 

    // load font 
    if (!(image = SDL_LoadBMP("/etc/Pacman3D/data/font.bmp"))) return false;

    // typical texture generation using data from the bitmap 
    glBindTexture(GL_TEXTURE_2D, _glFont);
    image = toRGB(image);

    // generate the texture 
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->w, image->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    SDL_FreeSurface(image);

    // creating 96 display lists (1 for each char)
    _base = glGenLists(96);
    glBindTexture(GL_TEXTURE_2D, _glFont);               // bind our font texture 

    // loop through all 96 lists (characters) 
    for (int i = 0; i < 96; ++i) {
        float cx = 1.0f - (float)(i % 16) / 16.0f;       // x position of current character 
        float cy = 1.0f - 0.25 - (float)(i / 16) / 8.0f; // y position of current character 

        // start building a list 
        glNewList(_base + (95 - i), GL_COMPILE);
            // use a quad for each character 
            glBegin(GL_QUADS);
                glTexCoord2f(cx - 0.0625f, cy);          glVertex2i(0, 0);   // bottom left
                glTexCoord2f(cx, cy);                    glVertex2i(15, 0);  // bottom right
                glTexCoord2f(cx, cy - 0.120f);           glVertex2i(15, 15); // top right
                glTexCoord2f(cx - 0.0625f, cy - 0.120f); glVertex2i(0, 15);  // top left
            glEnd();
            // move to the right of the character 
            glTranslated(10, 0, 0);
        glEndList();
    }

    return true;
}

// where the printing happens 
GLvoid Controls::glPrint(GLint x, GLint y, const char* str, ...) 
{
    char text[256]; // holds our string             
    va_list ap;     // pointer to list of arguments 

    // if there's no text 
    if (str == NULL) return;

    va_start(ap, str);          // parses the string for variable 
    vsprintf(text, str, ap);    // and converts symbols to actual numbers 
    va_end(ap);

    glBindTexture(GL_TEXTURE_2D, _glFont);              // select our font texture 
    glPushMatrix();                                     // store the modelview matrix 
    glLoadIdentity();                                   // reset the modelview matrix 
    glTranslated(x, y, 0);                              // position the text (0, 0 - bottom left) 
    glListBase(_base - 32);                             // choose the font set 
    glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);  // draws the display list text 
    glPopMatrix();
}

void Controls::drawLogo(int screenWidth, int screenHeight) 
{
    glLoadIdentity();   

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, screenWidth-1, 0, screenHeight-1, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    GLint y = screenHeight;
    // draw logo
    int width = 0;
    int height = 0;
    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _glLogo);
    y = screenHeight * 2/3 + 80;
    width = 128;
    height = 128;
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2i((screenWidth - width)/2, y - height/2);
        glTexCoord2f(1.0f, 1.0f); glVertex2i((screenWidth + width)/2, y - height/2);
        glTexCoord2f(1.0f, 0.0f); glVertex2i((screenWidth + width)/2, y + height/2);
        glTexCoord2f(0.0f, 0.0f); glVertex2i((screenWidth - width)/2, y + height/2);
    glEnd();    

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);   // set blending for translucency
    glPopMatrix();
}

void Controls::drawMenu(int screenWidth, int screenHeight) 
{
    drawLogo(screenWidth, screenHeight);

    const int CELL_WIDTH = 10;
    const int CELL_HEIGHT = 18;

    unsigned int i, j;
    // store in j, menu's longest entry
    for (j = 0, i = 0; vMenu[i].length() != 0; ++i)
        j = std::max(j, (unsigned int)vMenu[i].length());

    i += 3; 
    int x = (screenWidth - CELL_WIDTH * j) / 2;
    int y = (screenHeight - CELL_HEIGHT * i) / 2;

    // dim background 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0, 0.0, 0.0, 0.8);  // black with certain transparency
    glLoadIdentity();
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
        glVertex2d(x - CELL_WIDTH, screenHeight - y + CELL_HEIGHT / 2);
        glVertex2d(screenWidth - x + CELL_WIDTH, screenHeight - y + CELL_HEIGHT / 2);
        glVertex2d(screenWidth - x + CELL_WIDTH, screenHeight - y - i * CELL_HEIGHT - CELL_HEIGHT / 2);
        glVertex2d(x - CELL_WIDTH, screenHeight - y - i * CELL_HEIGHT - CELL_HEIGHT / 2);
    glEnd(); 
    glEnable(GL_TEXTURE_2D);
    glColor4f(1.0, 1.0, 1.0, 1.0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glEnable(GL_BLEND);

    std::string msg = "Waiting for game to start...";
    glPrint((screenWidth - msg.length() * CELL_WIDTH) / 2, screenHeight - 1 - y - CELL_HEIGHT, msg.c_str());

    y += 3 * CELL_HEIGHT;
    i -= 3;

    // and draw the entries 
    for (j = 0; j < i; ++j) {
        if (j == 2)       
            glPrint(x, screenHeight - 1 - y - CELL_HEIGHT, vMenu[j].c_str(), screenWidth, screenHeight);
        else if (j == 3)  
            glPrint(x, screenHeight - 1 - y - CELL_HEIGHT, vMenu[j].c_str(), _sound ? "On" : "Off");
        else if (j == 4)  
            glPrint(x, screenHeight - 1 - y - CELL_HEIGHT, vMenu[j].c_str(), _music ? "On" : "Off");
        else           
            glPrint(x, screenHeight - 1 - y - CELL_HEIGHT, vMenu[j].c_str());
        y += CELL_HEIGHT;
    }
}

void Controls::draw2dStatus(int screenWidth, int screenHeight, int playerId, GLfloat fps, std::string chat) 
{
    glLoadIdentity();   

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, screenWidth-1, 0, screenHeight-1, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
    glLoadIdentity();
    glTranslated(0, 0, 0);

    Player* player = Maze::instance().getPlayer(playerId);
    if (player == NULL) return;

    const int x = 16;
    const int y = screenHeight - 16;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f); // reset translucency and color 
    glEnable(GL_TEXTURE_2D);
    // draw player logo 
    glBindTexture(GL_TEXTURE_2D, player->getIcon());
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(   x+12, y-31);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(x+31+12, y-31);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(x+31+12,    y);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(   x+12,    y);
    glEnd();    

    glBlendFunc(GL_SRC_ALPHA, GL_ONE);   // set blending for translucency

    glPrint(x+44, y-34, "%d", player->getScore());  // print score 
    glPrint(x-4, y-50, player->getName().c_str());  // print name 

    glPrint(10, 50, chat.c_str());                          // show chat message
    glPrint(10, 30, "Sound: %s", _sound ? "On" : "Off");    // show sound status
    glPrint(10, 10, "%.1f FPS", fps);                       // show frames per sec

    glPopMatrix();
    glPopMatrix();
}

void Controls::draw(int screenWidth, int screenHeight, int playerId, bool menu, bool status, std::string chat) 
{
    // gather our frames per second 
    static GLint frames = 0;
    static GLfloat fps  = 0;
    static GLint T0     = 0;

    frames++;
    GLint t = SDL_GetTicks();
    if (t - T0 >= 1000) {
        fps = frames / ((t - T0) / 1000.0);
        T0 = t; frames = 0;
    }

    glPushMatrix();
    if (menu)
        drawMenu(screenWidth, screenHeight);
    if (status)
        draw2dStatus(screenWidth, screenHeight, playerId, fps, chat);

    glColor4f(1.0f, 1.0f, 1.0f, 0.5f);

    glMatrixMode(GL_PROJECTION);    // select the projection matrix  
    glPopMatrix();                  // restore the old projection matrix 
    glMatrixMode(GL_MODELVIEW);     // select the modelview matrix 
}

void Controls::showStatistics(int screenWidth, int screenHeight, const std::string& msg, tStats stats)
{
    //drawLogo(screenWidth, screenHeight);

    const int CELL_WIDTH = 10;
    const int CELL_HEIGHT = 18;

    unsigned int i, j;
    // store in j, menu's longest entry
    for (j = 0, i = 0; vStats[i].length() != 0; ++i)
        j = std::max(j, (unsigned int)vStats[i].length());

    i += 3; 
    int x = (screenWidth - CELL_WIDTH * j) / 2;
    int y = (screenHeight - CELL_HEIGHT * i) / 2;

    // dim background 
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0, 0.0, 0.0, 0.8);  // black with certain transparency
    glLoadIdentity();
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
        glVertex2d(x - CELL_WIDTH, screenHeight - y + CELL_HEIGHT / 2);
        glVertex2d(screenWidth - x + CELL_WIDTH, screenHeight - y + CELL_HEIGHT / 2);
        glVertex2d(screenWidth - x + CELL_WIDTH, screenHeight - y - i * CELL_HEIGHT - CELL_HEIGHT / 2);
        glVertex2d(x - CELL_WIDTH, screenHeight - y - i * CELL_HEIGHT - CELL_HEIGHT / 2);
    glEnd(); 
    glEnable(GL_TEXTURE_2D);
    glColor4f(1.0, 1.0, 1.0, 1.0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glEnable(GL_BLEND);

    glPrint((screenWidth - msg.length() * CELL_WIDTH) / 2, screenHeight - 1 - y - CELL_HEIGHT, msg.c_str());

    y += 3 * CELL_HEIGHT;
    i -= 3;

    // and draw the entries 
    glPrint(x, screenHeight - 1 - y - CELL_HEIGHT, vStats[0].c_str(), stats.pills, stats.pillsEaten);
    y += CELL_HEIGHT;
    glPrint(x, screenHeight - 1 - y - CELL_HEIGHT, vStats[1].c_str(), stats.powerups, stats.powerupsEaten);
    y += CELL_HEIGHT;
    glPrint(x, screenHeight - 1 - y - CELL_HEIGHT, vStats[2].c_str(), stats.bonusCounter);
    y += CELL_HEIGHT;
    glPrint(x, screenHeight - 1 - y - CELL_HEIGHT, vStats[3].c_str(), stats.ghostsCounter);
}


