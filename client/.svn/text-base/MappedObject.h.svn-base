#ifndef MAPPED_OBJECT_H_INCLUDED
#define MAPPED_OBJECT_H_INCLUDED

#define NDIMS 3

enum tAxis { OB_X, OB_Y, OB_Z };

// info needed to determine position of object 
// (0,0) is the top left corner, y (width) grows right, x (height) grows down
class MappedObject
{
public:
    MappedObject() {}           // constructor
    virtual ~MappedObject() {};

    void setPos(float x, float y, float z = 0.5);
    void rotate(tAxis axis, float angle);
    virtual void draw() = 0;

    // member variables
    float pos[NDIMS];
    float x_vec[NDIMS], 
          y_vec[NDIMS], 
          z_vec[NDIMS];
};

#endif

