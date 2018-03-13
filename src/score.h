#ifndef SCORE_H
#define SCORE_H

#include "digit.h"

class Score
{
public:
    Score(){}
    Score(int flag);
    void draw(glm::mat4 VP);
    void update(int score,int flag);
    void add(int d,int flag);
    void subtract(int d, int flag );
    int score;
private:
    int digslen;
    Digit digs[50];
};

#endif // SCORE_H
