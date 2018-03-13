#include "ball.h"
#include "main.h"
float min(float a,float b){
    return a<b?a:b;
}
float max(float a,float b){
    return a>b?a:b;
}
Ball::Ball(float x, float y) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    speed = 0.00;
    speed_y=0.00;
    gravity=0.01;
    radius=0.3;
    must=0;
    GLfloat vertex_buffer_data[6480];
    int ptr=0;
    float ang=0;
    for (int i=1;i<=2160;i++){
        if (i%3==0){
            vertex_buffer_data[ptr++]=0;
            vertex_buffer_data[ptr++]=0;
            vertex_buffer_data[ptr++]=0;
            ang-=PI/360;
        }
        else{
            vertex_buffer_data[ptr++]=radius*cos(ang);
            vertex_buffer_data[ptr++]=radius*sin(ang);
            vertex_buffer_data[ptr++]=0;
            ang+=PI/360;
        }
    }
    color_t colors[7]={COLOR_VIOLET,COLOR_INDIGO,COLOR_BLUE,COLOR_GREEN,COLOR_YELLOW,COLOR_ORANGE,COLOR_RED};
    this->object = create3DObject(GL_TRIANGLES, 2160, vertex_buffer_data, colors, GL_FILL,7);
}

void Ball::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 1));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Ball::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Ball::tick() {
    this->position.x += speed;
    this->position.y += speed_y;
    if (this->position.y > -2) 
        speed_y-=gravity;
    else
        speed_y-=gravity/2;
    speed_y=min(speed_y,0.35);
    if (this->position.y <= -1.35 && this->position.x>=1.8 && this->position.x<=3.4){
        speed_y=-speed_y+0.05;
        speed_y=min(0.45,speed_y);
    }
    int flag=0;
    if (this->position.x>=-0.7 && this->position.x<=0.7){
        flag=1;
        float insx=this->position.x;
        float theta = asin(insx/0.7);
        float insy=-2.3-(insx/tan(theta));
 //      float insy=-2.3-sqrtf(0.49-insx*insx);
        if (this->position.y < insy){
            this->position.y=insy;
            speed_y=0;
        }
    }
    if (!flag && this->position.y <= -2 && (this->position.x<-0.7 || this->position.x>0.7)){
        this->position.y =-2;
        speed_y=0;
    }
    if (this->position.x>4){
        this->position.x=4;
        speed=0;
    }
    if (this->position.x<-4){
        this->position.x=-4;
        speed=0;
    }
}

bounding_box_t Ball::bounding_box() {
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = { x, y, 0.4, 0.4 };
    return bbox;
}
