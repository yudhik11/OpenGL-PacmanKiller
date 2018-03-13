#include "main.h"

#ifndef MAGNET
#define MAGNET
class Magnet
{
public:
    Magnet(){}
    Magnet(float x, float y, float radius, color_t color,int flag);
    glm::vec3 position;
    void draw(glm::mat4 VP);
    float rotation;
   // void set_position(float x, float y);
    void tick();
    double speed,speed_y,gravity;
    bounding_box_t bounding_box();
private:
    VAO *object;
};
#endif
