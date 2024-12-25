#ifndef TIMER_H
#define TIMER_H
#include <stdlib.h>
#include "../raylib/include/raylib.h"

typedef struct {
    float lifetime;
} Timer;

void TimerStart(Timer* timer, float lifeteme);
void TimerUpdate(Timer* timer);
bool TimerDone(Timer* timer);

#endif
