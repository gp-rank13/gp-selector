/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.1.6

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include <juce_core/juce_core.h>
#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>
//[/Headers]

//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class LogWindow : public juce::Component, public juce::Button::Listener
{
  public:
    //==============================================================================
    LogWindow();
    ~LogWindow() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void static initialize(); // Call this to initialize the component
    void static log(const juce::String &text);
    void static finalize(); // Call this when library is about to be unloaded to
                            // free resources
    void static showWindow();
    void static hideWindow();
    //[/UserMethods]

    void paint(juce::Graphics &g) override;
    void resized() override;
    void buttonClicked(juce::Button *buttonThatWasClicked) override;

  private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    static LogWindow *sfInstance;

    std::unique_ptr<juce::DocumentWindow> fWindow;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::Label> label;
    std::unique_ptr<juce::TextEditor> txtLog;
    std::unique_ptr<juce::TextButton> btnClear;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LogWindow)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
