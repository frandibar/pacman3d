#ifndef PILL_H_INCLUDED
#define PILL_H_INCLUDED

#include "MappedObject.h"
#include "lib/utilities.h"

class Pill : public MappedObject
{
public:
    static const float RADIUS;

    Pill(unsigned int vertexId);
    unsigned int getVertexId() const { return _vertexId; }
    tElementState getState() const { return _state; }
    void setState(tElementState state) { _state = state; }
    void draw();

protected:
    unsigned int _vertexId;
    tElementState _state;
};

class PowerUp : public Pill
{
public:
    static const float RADIUS;

    PowerUp(unsigned int vertexId) : Pill(vertexId) {
        pos[OB_Z] = RADIUS;
    }
    void draw();
};

class Bonus : public Pill
{
public:
    static const float RADIUS;

    Bonus(unsigned int vertexId) : Pill(vertexId) {
        pos[OB_Z] = RADIUS;
    }
    void draw();
};

#endif

