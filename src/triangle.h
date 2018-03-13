#include "main.h"

#ifndef TRAINGLE
#define TRAINGLE
class Triangle{
public:
    Triangle(){}
    Triangle(float x, float y,float num, float height, float width, color_t color);
    float rotation;
    glm::vec3 position;
    float length,speed;
    void tick();

   // void set_position(float x, float y);
    void draw(glm::mat4 VP);
private:
    VAO *object;
};
#endif
