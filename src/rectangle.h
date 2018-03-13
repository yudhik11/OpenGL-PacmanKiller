#include "main.h"

#ifndef RECTANGLE
#define RECTANGLE
class Rectangle{
public:
    Rectangle(){}
    Rectangle(float x, float y,float x1, float y1, float length, float width, color_t color);
    float rotation;
    glm::vec3 position;
   // void set_position(float x, float y);
    void draw(glm::mat4 VP);
private:
    VAO *object;
};
#endif