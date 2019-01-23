#include "main.h"

#ifndef EYES_H
#define EYES_H

float camera_rotation_angle = 90.0f;


// Eye - Location of camera. Don't change unless you are sure!!
glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
// Target - Where is the camera looking at.  Don't change unless you are sure!!
glm::vec3 target (0, 0, 0);
// Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
glm::vec3 up (0, 1, 0);

#endif