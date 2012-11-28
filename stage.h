
#pragma once

#include <allegro5/allegro.h>

class Stage
{
  public:
    // Stage control
    virtual void Begin() = 0;
    virtual void Pause() = 0;
    virtual void Resume() = 0;
    virtual void Finish() = 0;

    virtual void Event(ALLEGRO_EVENT *e) = 0;

    virtual void Update() = 0;
    virtual void Render() = 0;
};
