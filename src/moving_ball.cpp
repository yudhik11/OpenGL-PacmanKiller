#include "moving_ball.h"
#include "main.h"

Moving_Ball::Moving_Ball(float x, float y, color_t color,int flag)
{
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    if (color.r == COLOR_RED.r &&
            color.g == COLOR_RED.g &&
            color.b == COLOR_RED.b ) this->bscore = 25;
    else this->bscore = 50;
    speed= (rand()/(double)RAND_MAX);
    if (speed < 0.5) speed+=0.3;
    speed*=0.05;
    if (rand()/(double)RAND_MAX > 0.5) speed *=-1;
    if (speed<0) this->position.x=4;
    //speed_y=0.00;
    gravity=0.02;
    GLfloat vertex_buffer_data[6500];
    int ptr=0;
    float ang=0;
    plank=0;
    radius=0.3;
    if (rand()/(double)RAND_MAX > 0.5) radius=0.45;
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
    if (flag) this->object = create3DObject(GL_TRIANGLES, 2160, vertex_buffer_data, color, GL_FILL);
    if (!flag){
        plank=1;
        vertex_buffer_data[ptr++]=2*radius;
        vertex_buffer_data[ptr++]=radius+0.1;
        vertex_buffer_data[ptr++]=0;
        vertex_buffer_data[ptr++]=2*radius;
        vertex_buffer_data[ptr++]=radius;
        vertex_buffer_data[ptr++]=0;
        vertex_buffer_data[ptr++]=-2*radius;
        vertex_buffer_data[ptr++]=radius;
        vertex_buffer_data[ptr++]=0;

        vertex_buffer_data[ptr++]=2*radius;
        vertex_buffer_data[ptr++]=radius+0.1;
        vertex_buffer_data[ptr++]=0;
        vertex_buffer_data[ptr++]=-2*radius;
        vertex_buffer_data[ptr++]=radius;
        vertex_buffer_data[ptr++]=0;
        vertex_buffer_data[ptr++]=-2*radius;
        vertex_buffer_data[ptr++]=radius+0.1;
        vertex_buffer_data[ptr++]=0;
        rotation=rand()%30+30;
        if (x>0)
            rotation*=-1;
        float ang=(rotation*M_PI)/180;
//        rotation=ang;
        if (speed>0){
            cd[1][1]=position.x+radius*(2*cos(ang)-sin(ang));
            cd[1][2]=position.y+radius*(cos(ang) + 2*sin(ang));
            cd[10][1]=position.x-radius*(2*cos(ang) + sin(ang));
            cd[10][2]=position.y+radius*(cos(ang) - 2*sin(ang));
        }
        else{
            cd[1][1]=position.x+radius*(2*cos(ang)-sin(ang));
            cd[1][2]=position.y+radius*(cos(ang) + 2*sin(ang));
            cd[10][1]=position.x-radius*(2*cos(ang) + sin(ang));
            cd[10][2]=position.y+radius*(cos(ang) - 2*sin(ang));
        }
        float difx=(-cd[1][1]+cd[10][1]);difx/=9;
        float dify=(-cd[1][2]+cd[10][2]);dify/=9;
//        printf("%f %f\n",difx,dify);
//        printf("%f %f\n",cd[1][1],cd[1][2]);
//        printf("%f %f\n",cd[10][1],cd[10][2]);
        for (int i=1;i<9;i++){
            cd[i+1][1]=cd[1][1]+i*difx;
            cd[i+1][2]=cd[1][2]+i*dify;
        }
  //      for (int i=1;i<=10;i++)  printf("centre: %f %f\n",cd[i][1],cd[i][2]);

        this->object = create3DObject(GL_TRIANGLES, 2166, vertex_buffer_data, color, GL_FILL);
    }
}

void Moving_Ball::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 1));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

//void Moving_Ball::set_position(float x, float y) {
//    this->position = glm::vec3(x, y, 0);
//}

void Moving_Ball::tick() {
    this->position.x += speed;
    if (this->plank==1){
        for (int i=1;i<11;i++) cd[i][1]+=speed;
    }
    if (this->position.x >4 && this->speed >0) {
        this->position.x=-4;
        if (plank){
            for (int i=1;i<=10;i++) cd[i][1]-=8;
        }
    }
    if (this->position.x <-4 && this->speed <0) {
        this->position.x=4;
        if (plank){
            for (int i=1;i<=10;i++) cd[i][1]+=8;
        }
    }
    //this->position.y += speed_y;
}

bounding_box_t Moving_Ball::bounding_box() {
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = { x, y, 0.4, 0.4 };
    return bbox;
}

