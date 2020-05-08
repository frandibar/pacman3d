#include "MappedObject.h"

#include <cmath>

// constant used for converting to radians 
const float PI_OVER_180 = 0.0174532925;   // PI/180

void MappedObject::rotate(tAxis axis, float angle) 
{
    float dsin = sin(angle * PI_OVER_180);
    float dcos = cos(angle * PI_OVER_180);

    if (axis == OB_X) {
        float tmp   = dcos * x_vec[OB_Z] - dsin * x_vec[OB_Y];
        x_vec[OB_Y] = dsin * x_vec[OB_Z] + dcos * x_vec[OB_Y];
        x_vec[OB_Z] = tmp;

        tmp         = dcos * y_vec[OB_Z] - dsin * y_vec[OB_Y];
        y_vec[OB_Y] = dsin * y_vec[OB_Z] + dcos * y_vec[OB_Y];
        y_vec[OB_Z] = tmp;

        tmp         = dcos * z_vec[OB_Z] - dsin * z_vec[OB_Y];
        z_vec[OB_Y] = dsin * z_vec[OB_Z] + dcos * z_vec[OB_Y];
        z_vec[OB_Z] = tmp;
    } 
    else if (axis == OB_Y) {
        float tmp   = dcos * x_vec[OB_X] - dsin * x_vec[OB_Z];
        x_vec[OB_Z] = dsin * x_vec[OB_X] + dcos * x_vec[OB_Z];
        x_vec[OB_X] = tmp;

        tmp         = dcos * y_vec[OB_X] - dsin * y_vec[OB_Z];
        y_vec[OB_Z] = dsin * y_vec[OB_X] + dcos * y_vec[OB_Z];
        y_vec[OB_X] = tmp;

        tmp         = dcos * z_vec[OB_X] - dsin * z_vec[OB_Z];
        z_vec[OB_Z] = dsin * z_vec[OB_X] + dcos * z_vec[OB_Z];
        z_vec[OB_X] = tmp;

    } 
    else if (axis == OB_Z) {
        float tmp   = dcos * x_vec[OB_X] - dsin * x_vec[OB_Y];
        x_vec[OB_Y] = dsin * x_vec[OB_X] + dcos * x_vec[OB_Y];
        x_vec[OB_X] = tmp;

        tmp         = dcos * y_vec[OB_X] - dsin * y_vec[OB_Y];
        y_vec[OB_Y] = dsin * y_vec[OB_X] + dcos * y_vec[OB_Y];
        y_vec[OB_X] = tmp;

        tmp         = dcos * z_vec[OB_X] - dsin * z_vec[OB_Y];
        z_vec[OB_Y] = dsin * z_vec[OB_X] + dcos * z_vec[OB_Y];
        z_vec[OB_X] = tmp;
    }
}

void MappedObject::setPos(float x, float y, float z) 
// This function must be called only once on initialization
{
    pos[OB_X] = x;
    pos[OB_Y] = y;
    pos[OB_Z] = z;

    x_vec[OB_X] = -1.0; x_vec[OB_Y] =  0.0; x_vec[OB_Z] = 0.0; 
    y_vec[OB_X] =  0.0; y_vec[OB_Y] = -1.0; y_vec[OB_Z] = 0.0; 
    z_vec[OB_X] =  0.0; z_vec[OB_Y] =  0.0; z_vec[OB_Z] = 1;

    rotate(OB_X, 90.0);
}

