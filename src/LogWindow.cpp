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

//[Headers] You can add your own extra header files here...
#include <juce_events/juce_events.h>
//[/Headers]

#include "LogWindow.h"

//[MiscUserDefs] You can add your own user definitions and misc code here...
LogWindow *LogWindow::sfInstance = nullptr;

class MyDocumentWindow : public juce::DocumentWindow
{
  public:
    MyDocumentWindow() : DocumentWindow("HelloGP-JUCE", juce::Colours::black, DocumentWindow::allButtons, true)
    {
    }

    virtual ~MyDocumentWindow()
    {
    }
    virtual void closeButtonPressed() override
    {
        setVisible(false);
    };
};
//[/MiscUserDefs]

//==============================================================================
LogWindow::LogWindow()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    label.reset(new juce::Label("new label", TRANS("Hello Gig Performer From an Extension - JUCE + CMake")));
    addAndMakeVisible(label.get());
    label->setFont(juce::Font(15.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    label->setJustificationType(juce::Justification::centredLeft);
    label->setEditable(false, false, false);
    label->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label->setBounds(8, 8, 376, 24);

    txtLog.reset(new juce::TextEditor("txtLog"));
    addAndMakeVisible(txtLog.get());
    txtLog->setMultiLine(true);
    txtLog->setReturnKeyStartsNewLine(true);
    txtLog->setReadOnly(false);
    txtLog->setScrollbarsShown(true);
    txtLog->setCaretVisible(true);
    txtLog->setPopupMenuEnabled(true);
    txtLog->setText(juce::String());

    btnClear.reset(new juce::TextButton("btnClear"));
    addAndMakeVisible(btnClear.get());
    btnClear->setButtonText(TRANS("Clear"));
    btnClear->addListener(this);

    //[UserPreSize]
    //[/UserPreSize]

    setSize(600, 400);

    //[Constructor] You can add your own custom stuff here..
    fWindow.reset(new MyDocumentWindow());
    fWindow->setContentNonOwned(this, true);
    fWindow->setResizable(true, true);
    fWindow->setUsingNativeTitleBar(true);
    //[/Constructor]
}

LogWindow::~LogWindow()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    label = nullptr;
    txtLog = nullptr;
    btnClear = nullptr;

    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void LogWindow::paint(juce::Graphics &g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll(juce::Colour(0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void LogWindow::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    txtLog->setBounds(16, 40, getWidth() - 33, getHeight() - 83);
    btnClear->setBounds(getWidth() - 121, getHeight() - 35, 102, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void LogWindow::buttonClicked(juce::Button *buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btnClear.get())
    {
        //[UserButtonCode_btnClear] -- add your button handler code here..
        txtLog->clear();
        //[/UserButtonCode_btnClear]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
// other code here...
void LogWindow::initialize()
{
    juce::MessageManager::getInstance()->callAsync([]() {
        if (sfInstance == nullptr)
        {
            sfInstance = new LogWindow();
            sfInstance->fWindow->setTopLeftPosition(100, 100);
            LogWindow::log("HelloGP-JUCE library loaded and initialized.\n");
        }

        jassert(sfInstance != nullptr);
        sfInstance->fWindow->setVisible(true);
    });
}

void LogWindow::log(const juce::String &text)
{
    if (sfInstance != nullptr)
    {
        juce::MessageManager::getInstance()->callAsync([text]() {
            sfInstance->txtLog->insertTextAtCaret(text);
            sfInstance->txtLog->insertTextAtCaret("\n");
        });
    }
}

void LogWindow::finalize()
{
    if (sfInstance != nullptr)
    {
        delete sfInstance;
        sfInstance = nullptr;
    }
}

void LogWindow::showWindow()
{
    sfInstance->fWindow->setVisible(true);
    sfInstance->fWindow->toFront(true);
}

void LogWindow::hideWindow()
{
    sfInstance->fWindow->setVisible(false);
}

//[/MiscUserCode]

//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="LogWindow" componentName=""
                 parentClasses="public juce::Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <LABEL name="new label" id="40cd3376be361c4a" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="8 8 376 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Hello Gig Performer From an Extension - JUCE + CMake"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <TEXTEDITOR name="txtLog" id="42efa646cdabd3e8" memberName="txtLog" virtualName=""
              explicitFocusOrder="0" pos="16 40 33M 83M" initialText="" multiline="1"
              retKeyStartsLine="1" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <TEXTBUTTON name="btnClear" id="a27dcb21b6042210" memberName="btnClear" virtualName=""
              explicitFocusOrder="0" pos="121R 35R 102 24" buttonText="Clear"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

//[EndFile] You can add extra defines here...
//[/EndFile]
