#include "magnet.h"
#include "main.h"
Magnet::Magnet(float x, float y,float radius, color_t color,int flag)
{
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    gravity=0.02;
    GLfloat vertex_buffer_data[3900];
    int ptr=0;
    float ang=0.0;
    if (!flag)ang=-2*PI/5;
    else ang=3*PI/5;
    for (int i=1;i<=1300;i++){
        if (i%3==0){
            vertex_buffer_data[ptr++]=0;
            vertex_buffer_data[ptr++]=0;
            vertex_buffer_data[ptr++]=0;
            ang+=PI/360;
        }
        else{
            vertex_buffer_data[ptr++]=radius*cos(ang);
            vertex_buffer_data[ptr++]=radius*sin(ang);
            vertex_buffer_data[ptr++]=0;
            ang-=PI/360;
        }
    }
    this->object = create3DObject(GL_TRIANGLES, 1300, vertex_buffer_data, color, GL_FILL);
}

void Magnet::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 1));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}
