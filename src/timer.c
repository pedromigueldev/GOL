#include "timer.h"

void TimerStart(Timer* timer, const float lifeteme)
{
    if (NULL != timer)
        timer->lifetime = lifeteme;
}
void TimerUpdate(Timer* timer)
{
    if (NULL != timer && timer->lifetime > 0) {
        timer->lifetime -= GetFrameTime();
    }
}
bool TimerDone(Timer* timer)
{
    if (NULL != timer)
        return timer->lifetime <= 0;
    return false;
}
