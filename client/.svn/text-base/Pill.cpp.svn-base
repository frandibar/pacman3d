#include "Pill.h"
#include "Maze.h"

#include <GL/gl.h>
#include <GL/glu.h>


const float Pill::RADIUS = 0.1;
const float PowerUp::RADIUS = 0.15;
const float Bonus::RADIUS = 0.15;

Pill::Pill(unsigned int vertexId) : _vertexId(vertexId), _state(APPEARED)
{
    unsigned int row = _vertexId / Maze::instance().getWidth(); 
    unsigned int col = _vertexId % Maze::instance().getWidth(); 
    
    pos[OB_X] = row + 0.5;
    pos[OB_Y] = col + 0.5;
    pos[OB_Z] = RADIUS;
}

void Pill::draw()
{
    if (_state != APPEARED) return;

    glPushMatrix();

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_COLOR_MATERIAL);
    GLUquadricObj* quad = gluNewQuadric();
    gluQuadricNormals(quad, GL_SMOOTH);           // generate smooth normals for the quad 
    glTranslatef(pos[OB_X], pos[OB_Y], pos[OB_Z]);
    glColor3f(1.0f, 1.0f, 0.0f); // yellow
    gluSphere(quad, RADIUS, 32, 16);
    glPopAttrib();

    glPopMatrix();
}


void PowerUp::draw()
{
    if (_state != APPEARED) return;

    glPushMatrix();

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_COLOR_MATERIAL);
    GLUquadricObj* quad = gluNewQuadric();
    gluQuadricNormals(quad, GL_SMOOTH);           // generate smooth normals for the quad 
    glTranslatef(pos[OB_X], pos[OB_Y], pos[OB_Z]);
    glColor3f(1.0f, 1.0f, 1.0f); // white
    gluSphere(quad, RADIUS, 32, 16);
    glPopAttrib();

    glPopMatrix();
}

void Bonus::draw()
{
    if (_state != APPEARED) return;

    glPushMatrix();

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_COLOR_MATERIAL);
    GLUquadricObj* quad = gluNewQuadric();
    gluQuadricNormals(quad, GL_SMOOTH);           // generate smooth normals for the quad 
    
    glTranslatef(pos[OB_X], pos[OB_Y], pos[OB_Z]);
    glColor3f(0.0f, 0.0f, 1.0f); // blue
    gluSphere(quad, RADIUS, 32, 16);
    glPopAttrib();

    glPopMatrix();
}
