#include "ball.h"

#ifndef FIRELINE_H
#define FIRELINE_H

class FireLine : public Ball {
    public :
        float width, height, angle;
        FireLine(float x, float y, double speed, GLfloat vertex_buffer_data[], int n, color_t color, float width, float height, float angle): 
            Ball(x, y, speed, vertex_buffer_data, n, color) {
                this->width = width;
                this->height = height;
                this->angle = angle;
            }
};

#endif