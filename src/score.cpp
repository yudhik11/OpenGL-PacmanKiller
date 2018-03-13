#include "score.h"
#include "main.h"
#include <cstring>

Score::Score(int flag)
{
    score = 0;
    digslen = 0;
}

void Score::update(int score,int flag) {
    this->score = score;
    char scorestr[50];
    sprintf(scorestr, "%d", score);
    digslen = strlen(scorestr);
    for (int i = digslen - 1; i >= 0; i--) {
        if (flag) digs[i] = Digit(scorestr[i], -3 - 0.3*(digslen - i), 3.5);
        else digs[i] = Digit(scorestr[i], 4 - 0.3*(digslen - i), 3.5);
    }
}

void Score::add(int d,int flag) {
    update(score+d,flag);
}

void Score::subtract(int d,int flag) {
    update(score-d,flag);
}
void Score::draw(glm::mat4 VP) {
    for (int i = 0; i < digslen; i++) digs[i].draw(VP);
}
