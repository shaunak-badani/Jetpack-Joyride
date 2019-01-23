#include "main.h"

#ifndef BALL_H
#define BALL_H


class Ball {
public:
    Ball() {}
    Ball(float x, float y, double speed, GLfloat vertex_buffer_data[], int n, color_t color);
    glm::vec3 position;
    float rotation;
    float width,height; // For rectangular objects
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speedX,speedY;
private:
    VAO *object;
};

#endif // BALL_H
