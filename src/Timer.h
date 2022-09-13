// Rackspace and song selector extension for Gig Performer by @rank13

#pragma once
#include <juce_events/juce_events.h>

class ClockTimer : public Timer
{
public:
    void timerCallback() override;
};

class RefreshTimer : public Timer
{
public:
    void timerCallback() override;
};