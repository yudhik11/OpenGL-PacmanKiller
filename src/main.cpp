#include "main.h"
#include "score.h"
#include "timer.h"
#include "ball.h"
#include "moving_ball.h"
#include "pond.h"
#include "rectangle.h"
#include "tramp.h"
#include "triangle.h"
#include "magnet.h"
using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

Ball ball1;
Moving_Ball mball[26];
Rectangle ground1,ground2,rtramp;
Pond pond;
Tramp ctramp;
Triangle proc;
Magnet msmalll,mbigl;
Magnet msmallr,mbigr;
Score score,level;

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
extern bool drag_pan;
extern double drag_oldx, drag_oldy;
Timer t60(1.0 / 60);
int mflagl=0,mflagr=0;

float sqr(float a,int i){
    return a*a;
}

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    // glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    // glm::vec3 target (0, 0, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    // glm::vec3 up (0, 1, 0);

    // Compute Camera matrix (view)
    // Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    ground2.draw(VP);
    ground1.draw(VP);
    rtramp.draw(VP);
    ctramp.draw(VP);
    if (level.score>2 && mflagl){
        mbigl.draw(VP);
        msmalll.draw(VP);
    }
    if (level.score>3 && mflagr){
        mbigr.draw(VP);
        msmallr.draw(VP);
    }
    for (int i=1;i<=tballs;i++){
            mball[i].draw(VP);
    }
    pond.draw(VP);
    ball1.draw(VP);
    if (level.score>1){
        proc.draw(VP);
    }
    score.draw(VP);
    level.draw(VP);
}

void tick_input(GLFWwindow *window) {
    int A  = glfwGetKey(window, GLFW_KEY_A);
    int D = glfwGetKey(window, GLFW_KEY_D);
    int W = glfwGetKey(window, GLFW_KEY_W);
    int space = glfwGetKey(window, GLFW_KEY_SPACE);
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up = glfwGetKey(window, GLFW_KEY_UP);
    int down = glfwGetKey(window, GLFW_KEY_DOWN);
    if (A) {
        if ( (ball1.position.y > -1.7) ||
             (ball1.position.x >= 3.5 || (ball1.position.x >= -4 && ball1.position.x <= 1.8)))
        ball1.position.x-=0.06;
        ball1.rotation+=4;
        if (ball1.speed>0) ball1.speed-=0.005;
    }
    if (D) {
        if ( (ball1.position.y > -1.7) ||
             (ball1.position.x<=1.7 || (ball1.position.x >=3.4 && ball1.position.x <= 4)))
        ball1.position.x+=0.06;
        ball1.rotation-=4;
        if (ball1.speed<0) ball1.speed+=0.005;

    }
    if (W || space){
        if (ball1.speed_y==0 && ball1.position.y<-2){
            ball1.speed_y+=0.15;
            ball1.speed_y=min(0.2,ball1.speed_y);
        }
        else if (ball1.speed_y ==0 && ball1.position.y < 0){
            ball1.speed_y+=0.25;
            ball1.speed_y=min(0.3,ball1.speed_y);
        }
    }
//    int drag_pan = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
//    if (drag_pan) {
//        if (drag_oldx == -1 || drag_oldy == -1) {
//            glfwGetCursorPos(window, &drag_oldx, &drag_oldy);
//        } else {
//            double newx, newy;
//            glfwGetCursorPos(window, &newx, &newy);
//            int width, height;
//            glfwGetWindowSize(window, &width, &height);
//            screen_center_x -= 8 * (newx - drag_oldx) / (width * screen_zoom);
//            screen_center_y += 8 * (newy - drag_oldy) / (height * screen_zoom);
//            if (screen_center_x - 4/screen_zoom < -4) screen_center_x = -4 + 4/screen_zoom;
//            if (screen_center_y - 4/screen_zoom < -4) screen_center_y = -4 + 4/screen_zoom;
//            if (screen_center_x + 4/screen_zoom > 4) screen_center_x = 4 - 4/screen_zoom;
//            if (screen_center_y + 4/screen_zoom > 4) screen_center_y = 4 - 4/screen_zoom;
//            drag_oldx = newx;
//            drag_oldy = newy;
//            reset_screen();
//        }
//    }
    int mouse_clicked = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    if (left && screen_center_x - 4/screen_zoom > -4)
       {
           screen_center_x -= 0.1;
           reset_screen();
       }
       if (down && screen_center_y - 4/screen_zoom > -4)
       {
           screen_center_y -= 0.1;
           reset_screen();
       }
       if (right && screen_center_x + 4/screen_zoom < 4)
       {
           screen_center_x += 0.1;
           reset_screen();
       }
       if (up && screen_center_y + 4/screen_zoom < 4)
       {
           screen_center_y += 0.1;
           reset_screen();
       }

       if (mouse_clicked) {
           if (drag_oldx == -1 && drag_oldy == -1) {
               glfwGetCursorPos(window, &drag_oldx, &drag_oldy);
           }
           else {
               int w, h;
               double new_x, new_y;
               glfwGetCursorPos(window, &new_x, &new_y);
               glfwGetWindowSize(window, &w, &h);
               float pos_x, pos_y;
               pos_x = 8 * (new_x - drag_oldx) / (w * screen_zoom);
               ball1.position.x = pos_x;
               // drag_oldx = new_x;
               // drag_oldy = new_y;
           }
       }
       int R = glfwGetKey(window, GLFW_KEY_R);
       if (R){
           screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
           reset_screen();
       }
}


void tick_elements() {
    if (ball1.position.x>-1 && ball1.position.x<-0.7 && ball1.position.y==-2) ball1.position.x=-0.7;
    if (ball1.position.x<1 && ball1.position.x>0.7 && ball1.position.y==-2) ball1.position.x=0.7;
    if (ball1.position.x>0.7 && ball1.position.x<1 && ball1.position.y<-2) {
        ball1.speed_y=0.2;
        ball1.rotation+=3;
    }
    if (ball1.position.x<-0.7 && ball1.position.x>-1 && ball1.position.y<-2) {
        ball1.speed_y=0.2;
        ball1.rotation-=3;
    }
    if (ball1.position.y < -2){
        if (ball1.position.x < 0.7 && ball1.position.x >0) {
            ball1.speed =-0.02;
            ball1.rotation+=3;
        }
        else if(ball1.position.x < 0 && ball1.position.x >-0.7) {
            ball1.speed =0.02;
            ball1.rotation-=3;
        }
    }

    else if (ball1.position.y<=-2 && ball1.position.y>=-2.4 && ball1.speed_y>=0) ball1.speed=0;
    if (level.score>2 && mflagl && ball1.position.y > 0.7 && ball1.position.y<3.3) ball1.speed-=0.005;
    if (level.score>3 && mflagr && ball1.position.y > 0.7 && ball1.position.y<3.3) ball1.speed+=0.005;
    ball1.tick();
    if (level.score>1){
        if (level.score>4)
            proc.tick();
        if (ball1.position.y<=-1.7 && ball1.position.x<=(proc.position.x+0.8+ball1.radius) && ((ball1.position.x+ball1.radius)>=proc.position.x)){
            ball1.position.x=-3.7;
            proc.position.x=-2;
            //level.subtract(1,1);
            score.subtract(50,0);
            level.update(score.score/200+1,1);
        }
    }
    for (int i=1;i<=tballs;i++){

        if (ball1.position.y>mball[i].position.y && mball[i].plank && ball1.speed_y<0){
            //printf("%d\n",i);
            for (int j=1;j<=10;j++){
//                printf("%f %f\n",(sqr(ball1.position.x-mball[i].cd[j][1],2)+sqr(ball1.position.y-mball[i].cd[j][2],2)),sqr(ball1.radius,2));
//                printf("cd : j: %d %f %f\n",j,mball[i].cd[j][1],mball[i].cd[j][2]);
                if ((sqr(ball1.position.x-mball[i].cd[j][1],2)+sqr(ball1.position.y-mball[i].cd[j][2],2)) < sqr(ball1.radius,2)+0.1){
                 //   printf("detected j: %d\n",j);
                    float shift=0;
//                    if (mball[i].speed < 0 ) {
//                        shift=mball[i].position.x+4;
//                        mball[i].position.x-=shift;
//                        for (int k=1;k<=10;k++) mball[i].cd[k][1]-=shift;
//                    }
//                    else {
//                        shift=4-mball[i].position.x;
//                        mball[i].position.x+=shift;
//                        for (int k=1;k<=10;k++) mball[i].cd[k][1]+=shift;
//                    }
//                    mball[i].speed*=-1;
                    float v=ball1.speed_y;
                    ball1.speed_y+=-1*v*cos((2*mball[i].rotation)*(PI/180.0));
                    float temp=v*sin((2*mball[i].rotation)*(PI/180.0));
                    //score.add(25,0);
                    ball1.speed=temp/2;
                    //printf("ball1.speed : %f",ball1.speed );
                    level.update(score.score/200+1,1);
                    break;
                }
            }
        }
        if ((ball1.position.y>mball[i].position.y) &&
                ( sqr((ball1.position.x-mball[i].position.x),2) + sqr((ball1.position.y-mball[i].position.y),2)) < sqr(ball1.radius+mball[i].radius,2) &&
            ball1.speed_y<0
            ){
            score.add(mball[i].bscore,0);
            level.update(score.score/200+1,1);
            if (mball[i].speed < 0 ) {
                mball[i].position.x=-4;
            }
            else mball[i].position.x=4;
            mball[i].speed*=-1;
            ball1.speed_y=0.2;
        }
        else mball[i].tick();
    }
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models
    ground1 = Rectangle(0   ,0  ,-8 ,-8   ,16  ,6-0.5-0.3  ,COLOR_BROWN);
    ground2 = Rectangle(0   ,0  ,-8 ,-2.8 ,16  ,0.5        ,COLOR_GREEN);
    rtramp  = Rectangle(0   ,0  , 2.0 ,-2.3 ,1.2  ,0.8      ,COLOR_DBLUE);
    ctramp  = Tramp(2.6,-1.5,0.6,COLOR_MRED);
    ball1   = Ball(-3.7, 	-2);
    pond 	= Pond(0,	-2.3,	1,COLOR_BLUE);
    proc    = Triangle(-2,-2.3,4,0.2,0.3,COLOR_RED);
    score   = Score(0);
    level   = Score(1);
    msmalll = Magnet(-3,2,0.7,COLOR_BLACK,0);
    mbigl   = Magnet(-3,2,1.3,COLOR_MRED,0);
    msmallr = Magnet(3,2,0.7,COLOR_BLACK,1);
    mbigr   = Magnet(3,2,1.3,COLOR_MRED,1);

    for (int i=1;i<=tballs;i++){
        if (i%3){
            if (i&01) mball[i]   = Moving_Ball(-4, (rand()/(double)RAND_MAX)*2.5, COLOR_RED,1);
            else mball[i]   = Moving_Ball(4, (rand()/(double)RAND_MAX)*2.5, COLOR_YELLOW,1);
        }
        else{
            if (i&01) mball[i]   = Moving_Ball(-4, (rand()/(double)RAND_MAX)*2.0, COLOR_RED,0);
            else mball[i]   = Moving_Ball(4, (rand()/(double)RAND_MAX)*2.0, COLOR_YELLOW,0);
        }
    }


    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // BLACK color of the scene
    glClearColor (COLOR_BLACK.r / 256.0, COLOR_BLACK.g / 256.0, COLOR_BLACK.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


char outsc[1024];
int main(int argc, char **argv) {
    srand(time(0));
    int width  = 600;
    int height = 600;
    int mtimel  = 255;
    int mtimer  = 755;
    window = initGLFW(width, height);

    initGL (window, width, height);
    sprintf(outsc,"Pacman Killer (Yudhik)");
    glfwSetWindowTitle(window,outsc);
    score.update(0,0);
    level.update(1,1);

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers

        if (t60.processTick()) {
            mtimel--;
            mtimer--;
            if (mtimel==0){
                mflagl=1;
            }
            if (mtimer==0){
                mflagr=1;
            }
            if (mtimel==-200){
                mtimel=rand()%255+255;
                mflagl=0;
            }
            if (mtimer==-250){
                mtimer=rand()%166+333;
                mflagr=0;
            }
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
           (abs(a.y - b.y) * 2 < (a.height + b.height));
}

void reset_screen() {
    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 4 / screen_zoom;
    float right  = screen_center_x + 4 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}
