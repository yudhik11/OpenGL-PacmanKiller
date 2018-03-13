#include "rectangle.h"
#include "main.h"
Rectangle::Rectangle(float x, float y,float x1, float y1,float length, float width, color_t color)
{
    this->position = glm::vec3(x, y, 0);
    const GLfloat vertex_buffer_data[] = {
        x1, y1, 0, // vertex 1
        x1, y1+width, 0, // vertex 2
        x1+length,  y1+width, 0, // vertex 3
        
        x1+length,  y1+width, 0, // vertex 3
        x1, y1, 0, // vertex 1
        x1+length, y1, 0, // vertex 1
    };

    this->object = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color, GL_FILL);
}

void Rectangle::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 1));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

//void Rectangle::set_position(float x, float y) {
//    this->position = glm::vec3(x, y, 0);
//}
