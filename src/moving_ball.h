#include "main.h"

#ifndef MOVING_BALL_H
#define MOVING_BALL_H

class Moving_Ball
{
public:
    Moving_Ball(){}
    Moving_Ball(float x, float y, color_t color,int flag);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
   // void set_position(float x, float y);
    void tick();
    double speed,speed_y,gravity,radius;
    int plank,bscore;
    float cd[11][3];
    bounding_box_t bounding_box();
private:
    VAO *object;
};
#endif
