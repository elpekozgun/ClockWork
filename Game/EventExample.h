#include "Core/Event.h"


class EventTracker
{
public:
    CW::Delegate<EventTracker, float> OnEvent;

    EventTracker()
    {
        OnEvent.Set(this, &EventTracker::OnPhysics);
    }


private:
    void OnPhysics(float time)
    {
        cout << "physics updated in: " << time << " ms.\n";
    }
};