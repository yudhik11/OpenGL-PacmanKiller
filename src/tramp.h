#include "main.h"

#ifndef TRAMP
#define TRAMP
class Tramp
{
public:
    Tramp(){}
    Tramp(float x, float y, float radius, color_t color);
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
