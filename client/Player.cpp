#include "Player.h"
#include "SDLHelpers.h"
#include "lib/debug.h"

#include <iostream>
#include <string>

const double Player::RADIUS = 0.5;

// constructor
Player::Player(unsigned int playerId, tRole role) : _id(playerId), _role(role), _score(0), _name(""), 
                                                    _glTextureNormal(-1), _glTexturePowerUp(-1),
                                                    _glTextureEaten(-1), _glIcon(-1),
                                                    _isAlive(true), _powerupEnabled(false), _orientation(WEST) 
{
}

void Player::setOrientation(tOrientation newOrientation)
{
    if (newOrientation == _orientation)
        return;

    // rotate player
    switch (_orientation) {

        case NORTH:
            switch (newOrientation) {
                case EAST:  turnRight(90.0); break;
                case SOUTH: turnLeft(180.0); break;
                case WEST:  turnLeft(90.0);  break;
                default: break;
            }
            break;

        case EAST:
            switch (newOrientation) {
                case SOUTH: turnRight(90.0); break;
                case WEST:  turnLeft(180.0); break;
                case NORTH: turnLeft(90.0);  break;
                default: break;
            }
            break;

        case SOUTH:
            switch (newOrientation) {
                case WEST:  turnRight(90.0); break;
                case NORTH: turnLeft(180.0); break;
                case EAST:  turnLeft(90.0);  break;
                default: break;
            }
            break;

        case WEST:
            switch (newOrientation) {
                case NORTH: turnRight(90.0); break;
                case EAST:  turnLeft(180.0); break;
                case SOUTH: turnLeft(90.0);  break;
                default: break;
            }
            break;

        default: break;
    }

    _orientation = newOrientation;
}

void Player::init() 
{
    // load player's textures
    std::string colors = "ygcmr678";
    std::string filename = "/etc/Pacman3D/data/";
    filename += colors[_id];
    filename += "_player.bmp";
    loadTexture(&_glTextureNormal, filename.c_str(), false);
    if (_role == GHOST) {
        loadTexture(&_glTexturePowerUp, "/etc/Pacman3D/data/b_player.bmp", false);
        loadTexture(&_glTextureEaten, "/etc/Pacman3D/data/x_player.bmp", false);
    }

    // load player's icon texture
    filename = "/etc/Pacman3D/data/";
    filename += colors[_id];
    filename += "_icon.bmp";
    loadTexture(&_glIcon, filename.c_str(), MASK_COLOR);

    setPos(0.5, 0.5);  // put in arbitrary position
}

void Player::draw(bool asDot) 
{
    glPushMatrix();

    GLUquadricObj* quad = gluNewQuadric();
    gluQuadricNormals(quad, GL_SMOOTH);                  // generate smooth normals for the quad 
    gluQuadricTexture(quad, GL_TRUE);                    // enable texture coords for the quad 
    glTranslatef(pos[OB_X], pos[OB_Y], pos[OB_Z]);
    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);

    // bind corresponding texture and draw
    if (_role == PACMAN) {
        // set up sphere mapping
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
        glBindTexture(GL_TEXTURE_2D, _glTextureNormal);
        gluSphere(quad, RADIUS, 32, 16);
    }
    else {
        if (!_isAlive)
            glBindTexture(GL_TEXTURE_2D, _glTextureEaten);
        else if (_powerupEnabled)
            glBindTexture(GL_TEXTURE_2D, _glTexturePowerUp);
        else
            glBindTexture(GL_TEXTURE_2D, _glTextureNormal);

        if (asDot)
            gluSphere(quad, RADIUS, 32, 16);
        else {
            glTranslatef(0.0f, 0.0f, -RADIUS/2);
            gluCylinder(quad, RADIUS, 2*RADIUS/3, 0.5f, 32, 32);      // base radius, upper radius, height
        }
    }

    glPopMatrix();
}

