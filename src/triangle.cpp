#include "triangle.h"
#include "main.h"
Triangle::Triangle(float x, float y,float num, float width, float height, color_t color)
{
    speed=0.02;
    this->position = glm::vec3(x, y, 0);
    GLfloat vertex_buffer_data[40];
    for (int i=0;i<num;i++){
        vertex_buffer_data[9*i]     =i*width;
        vertex_buffer_data[9*i+1]   =0;
        vertex_buffer_data[9*i+2]   =0;

        vertex_buffer_data[9*i+3]   =i*width+width/2;
        vertex_buffer_data[9*i+4]   =height;
        vertex_buffer_data[9*i+5]   =0;

        vertex_buffer_data[9*i+6]   =(i+1)*width;
        vertex_buffer_data[9*i+7]   =0;
        vertex_buffer_data[9*i+8]   =0;

    };

    this->object = create3DObject(GL_TRIANGLES, 4*3, vertex_buffer_data, color, GL_FILL);
}

void Triangle::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 1));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}
void Triangle::tick() {
    this->position.x += speed;
    if (this->position.x >=-1.8 || this->position.x <=-4) this->speed=-this->speed;
    //this->position.y += speed_y;
}
