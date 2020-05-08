#include "Maze.h"
#include "SDLHelpers.h"
#include "lib/debug.h"

#include <cmath>
#include <string>
#include <vector>
#include <cassert>

int Maze::wall[3][MAX_EXT][MAX_EXT][MAX_EXT] = {};

std::string vTextureFilenames[] = {  // must have NTEXTURES elements
                                    "/etc/Pacman3D/data/grass.bmp",    // FLOOR
                                    "/etc/Pacman3D/data/bricks.bmp",   // WALL
                                    "/etc/Pacman3D/data/portal.bmp"    // PORTAL
                                    //""                   // ROOF 
                                  };
// constructor
Maze::Maze() : _pSurface(NULL), _width(0), _height(0)
{
}

// destructor
Maze::~Maze()
{
    // delete dynamically allocated objects
    
    // pills
    for (std::list<Pill*>::iterator i = _pills.begin(); i != _pills.end(); ++i)
        delete (*i);              

    // bonuses
    for (std::list<Bonus*>::iterator i = _bonuses.begin(); i != _bonuses.end(); ++i)
        delete (*i);              

    // powerups
    for (std::list<PowerUp*>::iterator i = _powerups.begin(); i != _powerups.end(); ++i)
        delete (*i);              
   
    // players
    for (std::list<Player*>::iterator i = _players.begin(); i != _players.end(); ++i)
        delete (*i);              

}

void Maze::convertTextures() 
{
    // create texture 
    glGenTextures(NTEXTURES, &_vTextureIds[0]);

    // load textures 
    for (int i = 0; i < NTEXTURES; ++i) {
        // typical texture generation using data from the bitmap 
        glBindTexture(GL_TEXTURE_2D, _vTextureIds[i]);

        _vpTextureSurfaces[i] = toRGB(_vpTextureSurfaces[i]);

        // generate texture 
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, 
                     _vpTextureSurfaces[i]->w,
                     _vpTextureSurfaces[i]->h, 
                     0, GL_RGB, GL_UNSIGNED_BYTE,
                     _vpTextureSurfaces[i]->pixels);

#ifdef FILTER    
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, 
                          _vpTextureSurfaces[i]->w, 
                          _vpTextureSurfaces[i]->h, 
                          GL_RGB, GL_UNSIGNED_BYTE, 
                          _vpTextureSurfaces[i]->pixels); 
#else
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
#endif
        // linear filtering 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    // free up memory 
    for (int i = 0; i < NTEXTURES; ++i)
        if (_vpTextureSurfaces[i])
            SDL_FreeSurface(_vpTextureSurfaces[i]);
}

void Maze::load(unsigned int width, unsigned int height, const EdgeSet& edges)
{
    _width = width;
    _height = height;
    _grid.clear();

    // load texure surfaces
    _vpTextureSurfaces.resize(NTEXTURES);
    for (int i = 0; i < NTEXTURES; ++i) {
        _vpTextureSurfaces[i] = SDL_LoadBMP(vTextureFilenames[i].c_str());
    }

    convertTextures();     // load maze textures into gl

    // init wall
    for (int i = OB_X; i <= OB_Z; ++i)
        for (int z = 0; z < MAX_EXT; ++z)
            for (int y = 0; y < MAX_EXT; ++y)
                for (int x = 0; x < MAX_EXT; ++x)
                    wall[i][z][y][x] = NONE;

    fillgrid(edges);

    // traverse grid
    for (unsigned int i = 0; i < _height; ++i) {
        for (unsigned int j = 0; j < _width; ++j) {
            if (_grid[j][i]) {

                // should only set walls that are not already set. If wall is already set, remove it, 
                // since it is now an invisible inner wall between two boxes                                 

                // left and right face 
                if (wall[OB_X][i][0][j] == NONE)
                    wall[OB_X][i][0][j] = WALL;
                else
                    wall[OB_X][i][0][j] = NONE;

                if (wall[OB_X][i+1][0][j] == NONE)
                    wall[OB_X][i+1][0][j] = WALL;
                else
                    wall[OB_X][i+1][0][j] = NONE;

                // front and rear face 
                if (wall[OB_Y][j][0][i] == NONE)
                    wall[OB_Y][j][0][i] = WALL;
                else
                    wall[OB_Y][j][0][i] = NONE;

                if (wall[OB_Y][j+1][0][i] == NONE)
                    wall[OB_Y][j+1][0][i] = WALL;
                else
                    wall[OB_Y][j+1][0][i] = NONE;

                // bottom and top face
                if (wall[OB_Z][0][j][i] == NONE)
                    wall[OB_Z][0][j][i] = ROOF;  

                if (wall[OB_Z][1][j][i] == NONE)
                    wall[OB_Z][1][j][i] = ROOF;  
            }
        }
    }

    // create borders 
    for (unsigned int y = 0; y < _width; ++y) {
        if (wall[OB_X][0][0][y] == NONE) 
            wall[OB_X][0][0][y] = WALL;   
        //else
            //wall[OB_X][0][0][y] = NONE;   

        if (wall[OB_X][_height][0][y] == NONE) 
            wall[OB_X][_height][0][y] = WALL;
        //else
            //wall[OB_X][_height][0][y] = NONE;
    }

    for (unsigned int x = 0; x < _height; ++x) {
        if (wall[OB_Y][0][0][x] == NONE) 
            wall[OB_Y][0][0][x] = WALL;   
        //else
            //wall[OB_Y][0][0][x] = NONE;   

        if (wall[OB_Y][_width][0][x] == NONE) 
            wall[OB_Y][_width][0][x] = WALL;
        //else
            //wall[OB_Y][_width][0][x] = NONE;
    }

    // bottom
    for (unsigned int y = 0; y < _height; ++y) {
        for (unsigned int x = 0; x < _width; ++x) {
            if (wall[OB_Z][0][x][y] == NONE) 
                wall[OB_Z][0][x][y] = FLOOR;   
        }
    }
}

void Maze::setPlayerPosition(Player* player, unsigned int edge, unsigned int pos, unsigned int direction)
// map server positions to real positions
{
    assert(player != NULL);

    const float EDGE_LEN = 64.0;    // 1/64 min server unit

    unsigned int row = edge / (2 * _width); 
    unsigned int col = edge % _width; 

    float offset = (float)pos / EDGE_LEN;
    if (((edge / _width) % 2) == 0) {
        // vertical edge (x)
        if (direction == 0)                  // going south   0(S)---->(N)EDGE_LEN
            player->setOrientation(SOUTH);
        else if (direction == 1)             // going north
            player->setOrientation(NORTH);
        // set maze coordinates 
        player->pos[OB_X] = (float)row + 0.5 - offset;
        player->pos[OB_Y] = (float)col + 0.5;
    } 
    else {
        // horizontal edge (y)
        if (direction == 0)                  // going west   0(E)---->(W)EDGE_LEN
            player->setOrientation(WEST);
        else if (direction == 1)             // going east
            player->setOrientation(EAST);
        // set maze coordinates 
        player->pos[OB_X] = (float)row + 0.5;
        player->pos[OB_Y] = (float)col + 0.5 + offset;
    }

    // debug only when something changed
    static unsigned int prevpos = 0;
    static unsigned int prevdir = 0;
    if (pos != prevpos || player->getOrientation() != prevdir) {
        if (((edge / _width) % 2) == 0)
            DEBUG_0<char>(direction == 0 ? 'S' : 'N', "dir: ", true, false);
        else
            DEBUG_0<char>(direction == 0 ? 'W' : 'E', "dir: ", true, false);
        DEBUG_0<unsigned int>(edge, "  edge: ", false, false);
        DEBUG_0<unsigned int>(pos, "  pos: ", false, false);
        DEBUG_0<float>(player->pos[OB_X], "  x: ", false, false);
        DEBUG_0<float>(player->pos[OB_Y], "  y: ", false, false);
        DEBUG_0<float>(player->pos[OB_Z], "  z: ", false);
        prevpos = pos;
        prevdir = player->getOrientation();
    }
}

void Maze::fillgrid(const EdgeSet& edges)
{
    _grid.resize(_height);
    for (unsigned int i = 0; i < _height; ++i) {
        _grid[i].resize(_width);
        for (unsigned int j = 0; j < _width; ++j) {
            _grid[i][j] = true;
        }
    }

    for (EdgeSet::const_iterator it = edges.begin(); it != edges.end(); ++it) {
        int edge = *it;
        unsigned int row = edge / (2 * _width); 
        unsigned int col = edge % _width; 
        if (((edge / _width) % 2) == 0) {
            // vertical edge
            if (row == 0)
                addVerticalPortal(col);
            else {
                _grid[col][row] = false;
                _grid[col][row-1] = false;
            }
        } 
        else {
            // horizontal edge
            if (col == (_width - 1))
                addHorizontalPortal(row);
            else {
                _grid[col][row] = false;
                _grid[col+1][row] = false;
            }
        }
    }
}

void Maze::addVerticalPortal(int col)
{
    // add left portal
    _grid[col-1][0] = true;                 // pillar walls
    _grid[col+1][0] = true;
    wall[OB_X][0][0][col] = PORTAL;         // left face

    // add right portal
    _grid[col-1][_height-1] = true;
    _grid[col+1][_height-1] = true;
    wall[OB_X][_height  ][0][col] = PORTAL; // right face
}

void Maze::addHorizontalPortal(int row)
{
    // add bottom portal
    _grid[0][row-1] = true;                 // pillar walls
    _grid[0][row+1] = true;
    wall[OB_Y][0][0][row] = PORTAL;         // bottom face

    // add upper portal
    _grid[_width-1][row-1] = true;
    _grid[_width-1][row+1] = true;
    wall[OB_Y][_width][0][row] = PORTAL; // top face
}

bool Maze::validStep(MappedObject* obj) 
{
    const float RADIUS = 0.49;       // each map cells radius

    int x = (int)floor(obj->pos[OB_X]);
    int y = (int)floor(obj->pos[OB_Y]);
    int z = (int)floor(obj->pos[OB_Z]);

    float xi = obj->pos[OB_X] - floor(obj->pos[OB_X]);
    float yi = obj->pos[OB_Y] - floor(obj->pos[OB_Y]);

    if ((x >= 0) && (x < (int)_height) && (y >= 0) && (y < (int)_width) && (z == 0)) {
        // 4 direct neighbouring walls
        if (wall[OB_X][x  ][0][y] == WALL && (xi < RADIUS)) return false;
        if (wall[OB_X][x+1][0][y] == WALL && (xi > (1.0 - RADIUS))) return false;

        if (wall[OB_Y][y  ][0][x] == WALL && (yi < RADIUS)) return false;
        if (wall[OB_Y][y+1][0][x] == WALL && (yi > (1.0 - RADIUS))) return false;

        // 4 neighbouring borders
        if ((xi < RADIUS) && (yi < RADIUS)) {
            if (wall[OB_X][x][0][y-1] == WALL) return false;
            if (wall[OB_Y][y][0][x-1] == WALL) return false;
        }

        if ((xi < RADIUS) && (yi > (1.0 - RADIUS))) {
            if (wall[OB_X][x][0][y+1] == WALL) return false;
            if (wall[OB_Y][y+1][0][x-1] == WALL) return false;
        }

        if ((xi > (1.0 - RADIUS)) && (yi < RADIUS)) {
            if (wall[OB_X][x+1][0][y-1] == WALL) return false;
            if (wall[OB_Y][y][0][x+1] == WALL) return false;
        }

        if ((xi > (1.0 - RADIUS)) && (yi > (1.0 - RADIUS))) {
            if (wall[OB_X][x+1][0][y+1] == WALL) return false;
            if (wall[OB_Y][y+1][0][x+1] == WALL) return false;
        }
    }
    return true;
}

bool Maze::addStep(unsigned int playerId, float step) 
// advances step if no collision is detected
// returns true if no collision is detected
{
    bool ret = true;
    const float STEP_RES = 0.01;

    Player* player = getPlayer(playerId);
    if (player == NULL) return false;

    float x0 = player->pos[OB_X];   // store original values in case 
    float y0 = player->pos[OB_Y];   // a collision is detected

    float f_sign = (step < 0) ? -1.0 : 1.0;
    float f_abs = fabs(step);

    float aux;

    while (f_abs > STEP_RES) {  
        aux = f_sign * STEP_RES * player->x_vec[OB_Z];
        player->pos[OB_X] -= aux;
        if (!validStep(player)) {
            player->pos[OB_X] += aux;
            ret = false;
            break;
        }
        aux = f_sign * STEP_RES * player->y_vec[OB_Z];
        player->pos[OB_Y] -= aux;
        if (!validStep(player)) {
            player->pos[OB_Y] += aux;
            ret = false;
            break;
        }

        f_abs -= STEP_RES;    
    }

    // undo step
    aux = f_sign * STEP_RES * player->x_vec[OB_Z];
    player->pos[OB_X] -= aux;
    if (!validStep(player)) {
        player->pos[OB_X] += aux;
        ret = false;
    }

    aux = f_sign * STEP_RES * player->y_vec[OB_Z];
    player->pos[OB_Y] -= aux;
    if (!validStep(player)) {
        player->pos[OB_Y] += aux;
        ret = false;
    }

    if (!ret) {
        // restore original values
        player->pos[OB_X] = x0;   // restore original values
        player->pos[OB_Y] = y0;
        return false;
    }

    // wrap player's position
    if (player->pos[OB_X] < 0)
        player->pos[OB_X] += _height;

    if (player->pos[OB_X] > _height)
        player->pos[OB_X] -= _height;

    if (player->pos[OB_Y] < 0)
        player->pos[OB_Y] += _width;

    if (player->pos[OB_Y] > _width)
        player->pos[OB_Y] -= _width;

    return true;
}

void Maze::draw(int width, int height, unsigned int playerId, bool upperView)
{
    Player* player = getPlayer(playerId);
    if (player == NULL) return;

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glLoadIdentity();
    if (!upperView) {
        glViewport(0, 0, width, height);    // draw 3d view
    }
    else {
        glClear(GL_DEPTH_BUFFER_BIT);                               // draw map
        glViewport(2 * width/3, 2 * height/3, width/3, height/3);   // at upper right part of screen
        gluLookAt(0.0, 10.0, 0.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0);   // set view from above
    }

    glShadeModel(GL_SMOOTH);                               // enable smooth color shading
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);   // really nice perspective calculations
    //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);    // fast perspective calculations

    glTranslatef(0, 0, 0.5 * Player::RADIUS);              // translate to players eyes

    GLfloat matrix[16];
    // rotate depending on direction player is facing
    matrix[ 0] = player->x_vec[OB_X]; 
    matrix[ 1] = player->x_vec[OB_Y]; 
    matrix[ 2] = player->x_vec[OB_Z]; 
    matrix[ 3] = 0;

    matrix[ 4] = player->y_vec[OB_X]; 
    matrix[ 5] = player->y_vec[OB_Y]; 
    matrix[ 6] = player->y_vec[OB_Z]; 
    matrix[ 7] = 0;

    matrix[ 8] = player->z_vec[OB_X]; 
    matrix[ 9] = player->z_vec[OB_Y]; 
    matrix[10] = player->z_vec[OB_Z]; 
    matrix[11] = 0;

    matrix[12] = 0;                      
    matrix[13] = 0;              
    matrix[14] = 0;                      
    matrix[15] = 1;

    glMultMatrixf(matrix);

    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHTING);

    // TODO remove, just for debugging
    //static float lastx;
    //static float lasty;
    //if (lastx != player->pos[OB_X] || lasty != player->pos[OB_Y]) {
        //lastx = player->pos[OB_X];
        //lasty = player->pos[OB_Y];
        //std::cout << "pos: " << player->pos[OB_X] << " "
            //<< player->pos[OB_Y] << " "
            //<< player->pos[OB_Z] << std::endl;
    //}

    // translate the scene based on player position
    glTranslatef(-player->pos[OB_X], -player->pos[OB_Y], -player->pos[OB_Z]);

    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);

    // process all Z tiles (top and bottom faces)
    for (int y = 0; y < MAX_EXT; ++y) {
        for (int x = 0; x < MAX_EXT; ++x) {
            for (int z = 0; z < MAX_EXT; ++z) {
                if (Maze::wall[OB_Z][z][y][x] != Maze::NONE) {

                    // bind the proper texture to the triangle
                    glBindTexture(GL_TEXTURE_2D, Maze::instance().getTextureId(Maze::wall[OB_Z][z][y][x]));
                    glBegin(GL_QUADS);

                    if (z < player->pos[OB_Z]) glNormal3f(0, 0, 1.0);
                    else glNormal3f(0, 0, -1.0);

                    glTexCoord2f(0.0f, 0.0f);  glVertex3f(x,   y,   z);
                    glTexCoord2f(1.0f, 0.0f);  glVertex3f(x+1, y,   z);
                    glTexCoord2f(1.0f, 1.0f);  glVertex3f(x+1, y+1, z);
                    glTexCoord2f(0.0f, 1.0f);  glVertex3f(x,   y+1, z);

                    glEnd();
                }
            }
        }
    }

    // process all Y tiles
    for (int y = 0; y < MAX_EXT; ++y) {
        for (int x = 0; x < MAX_EXT; ++x) {
            for (int z = 0; z < MAX_EXT; ++z) {
                if (Maze::wall[OB_Y][y][z][x] != Maze::NONE) {

                    // bind the proper texture to the triangle
                    glBindTexture(GL_TEXTURE_2D, Maze::instance().getTextureId(Maze::wall[OB_Y][y][z][x]));
                    glBegin(GL_QUADS);

                    if (y < player->pos[OB_Y]) glNormal3f(0, 1.0, 0);
                    else glNormal3f(0, -1.0, 0);

                    glTexCoord2f(0.0f, 0.0f);  glVertex3f(x+1, y, z+1);
                    glTexCoord2f(1.0f, 0.0f);  glVertex3f(x,   y, z+1);
                    glTexCoord2f(1.0f, 1.0f);  glVertex3f(x,   y, z);
                    glTexCoord2f(0.0f, 1.0f);  glVertex3f(x+1, y, z);

                    glEnd();
                }
            }
        }
    }

    // process all X tiles
    for (int y = 0; y < MAX_EXT; ++y) {
        for (int x = 0; x < MAX_EXT; ++x) {
            for (int z = 0; z < MAX_EXT; ++z) {
                if (Maze::wall[OB_X][x][z][y] != Maze::NONE) {

                    // bind the proper texture to the triangle
                    glBindTexture(GL_TEXTURE_2D, Maze::instance().getTextureId(Maze::wall[OB_X][x][z][y]));
                    glBegin(GL_QUADS);

                    if (x < player->pos[OB_X]) glNormal3f(1.0, 0, 0);
                    else glNormal3f(-1.0, 0, 0);

                    glTexCoord2f(0.0f, 0.0f);  glVertex3f(x, y+1,   z);
                    glTexCoord2f(0.0f, 1.0f);  glVertex3f(x, y+1, z+1);
                    glTexCoord2f(1.0f, 1.0f);  glVertex3f(x,   y, z+1);
                    glTexCoord2f(1.0f, 0.0f);  glVertex3f(x,   y,   z);

                    glEnd();
                }
            }
        }
    }

    // draw pills
    for (std::list<Pill*>::iterator i = _pills.begin(); i != _pills.end(); ++i)
        (*i)->draw();              

    //// draw bonuses
    for (std::list<Bonus*>::iterator i = _bonuses.begin(); i != _bonuses.end(); ++i)
        (*i)->draw();              

    //// draw powerups
    for (std::list<PowerUp*>::iterator i = _powerups.begin(); i != _powerups.end(); ++i)
        (*i)->draw();              

    // draw players
    for (std::list<Player*>::iterator i = _players.begin(); i != _players.end(); ++i) {
        if (upperView) {
            if (playerId == 0)
                (*i)->draw(true);            // player is pacman, draw everybody, including pacman
            else if ((*i)->getId() != 0) // player is ghost, draw only ghosts
                (*i)->draw(true);           
        }
        else if ((*i)->getId() != playerId) // 3d view, draw everybody but player
            (*i)->draw();           
    }
    glPopAttrib();
}

void Maze::init()
{
    // init players
    for (std::list<Player*>::iterator i = _players.begin(); i != _players.end(); ++i)
        (*i)->init();
}

bool Maze::addPlayer(int id, Player::tRole role)
{
    Player* player = new Player(id, role);
    player->init();
    _players.push_back(player); 
    return true;
}

Player* Maze::getPlayer(unsigned int playerId) 
{ 
    // find player
    for (std::list<Player*>::iterator i = _players.begin(); i != _players.end(); ++i)
        if ((*i)->getId() == playerId)
            return *i;

    return NULL;   // player not found
}

void Maze::addElement(tElementType type, unsigned int vertexId, tOrientation orientation)
{
    switch (type) {
        case PACMAN_START: break;
        case GHOST_HOUSE:  break;
        case PILL:         _pills.push_back(new Pill(vertexId));       break;
        case POWER_UP:     _powerups.push_back(new PowerUp(vertexId)); break;
        case BONUS:        _bonuses.push_back(new Bonus(vertexId));    break;
        default:           break;
    }
}

bool Maze::updateElementState(tElementType type, unsigned int vertexId, tElementState state)
// updates element's state at vertexId
{
    if (type == PILL) {
        for (std::list<Pill*>::iterator i = _pills.begin(); i != _pills.end(); ++i) {
            if ((*i)->getVertexId() == vertexId) {
                (*i)->setState(state);
                return true;
            }
        }
    }
    else if (type == POWER_UP) {
        for (std::list<PowerUp*>::iterator i = _powerups.begin(); i != _powerups.end(); ++i) {
            if ((*i)->getVertexId() == vertexId) {
                (*i)->setState(state);
                return true;
            }
        }
    }        
    else if (type == BONUS) {
        for (std::list<Bonus*>::iterator i = _bonuses.begin(); i != _bonuses.end(); ++i) {
            if ((*i)->getVertexId() == vertexId) {
                (*i)->setState(state);
                return true;
            }
        }
    }        
    // element not found, add it
    addElement(type, vertexId, NORTH);
    return false;
}
