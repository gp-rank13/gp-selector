// Rackspace and song selector extension for Gig Performer by @rank13

#pragma once
#include <juce_events/juce_events.h>

class MyTimer : public Timer
{
public:
    void timerCallback() override;
};