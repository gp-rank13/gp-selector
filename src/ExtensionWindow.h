// Rackspace and song selector extension for Gig Performer by @rank13

#pragma once

#include <juce_core/juce_core.h>
#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "LookAndFeel.h"
#include "Timer.h"

using namespace juce;

class MyDraggableComponent : public Component 
{
public:
  MyDraggableComponent() { constrainer.setMinimumOnscreenAmounts (0xffffff, 0xffffff, 0xffffff, 0xffffff); }
  ComponentDragger myDragger;
  ComponentBoundsConstrainer constrainer;
  void mouseDown (const MouseEvent& e) override
  {
      myDragger.startDraggingComponent (this, e);
  }
  void mouseDrag (const MouseEvent& e) override
  {
      myDragger.dragComponent (this, e, &constrainer);
  }
  void paint(Graphics& g) override 
  { 
      if (isMouseOver(true)) {
              g.fillAll( Colour (0xff3B4D60)); 
      } else { 
          g.fillAll( Colour (0xff555555)); 
      } 
  }

  void mouseEnter (const MouseEvent&) override {
      repaint();
  }

  void mouseExit (const MouseEvent&) override {
      repaint();
  }
};

class docWindow : public DocumentWindow
{
  public:
  docWindow() :
  DocumentWindow("GP Selecter", Colours::lightgrey, DocumentWindow::allButtons, true)
  {
  setWantsKeyboardFocus(true);
  }
  virtual ~docWindow() { }
  virtual void closeButtonPressed () override;
};

class ExtensionWindow  : public juce::Component,
                   public juce::Button::Listener                 
{
public:
  ExtensionWindow ();
  virtual ~ExtensionWindow() override;
  void closeButtonPressed();
  void static initialize(); // Call this to show the component
  void static finalize(); // Call this when library is about to be unloaded to free resources
  void paint (Graphics& g) override;
  void resized() override;
  void buttonClicked (Button* buttonThatWasClicked) override;
  void closeWindow();
  void static updateButtonLabel(const String& label);
  void static updateButtonNames(std::vector<std::string> buttonNames);
  void static updateSubButtonNames(std::vector<std::string> buttonNames);
  std::vector<std::string> getSubButtonNamesByIndex(int index);
  void static updateButtonNamesAndColours(std::vector<std::string> buttonNames, std::vector<std::string> buttonColours);
  void static updateButtonLevel2Names(std::vector<std::vector<std::string>> buttonNames);
  bool static isButtonSelected(int index);
  int static getButtonSelected();
  void static selectButton(int index);
  void static updatePrevCurrNext(int index);
  bool static isSubButtonSelected(int index);
  bool static isSubButtonsCollapsed(); 
  void static selectSubButton(int index);
  void static selectButtonAndSubButton(int index, std::vector<std::string> buttonNames);
  void static updateClock(const String& formattedTime);
  void static setZeroBasedNumbering(bool zeroBased);
  void static setImmediateSwitching();
  String buttonName(int index);
  void static displayWindow(bool display);
  void static setTitleBarName(const String& name);
  void static proccessPreferences(std::vector<std::string> keyValue);
  void static refreshUI();
  void static updateButtonLnF(std::string LnFname);
  void mouseDrag (const MouseEvent& e) override
    {
        resized();
    }
 
  SharedResourcePointer<buttonLookAndFeel> LnFbuttons;
  SharedResourcePointer<gridButtonLookAndFeel> LnFgridButtons;
  SharedResourcePointer<headerSongs> LnFSongs;
  SharedResourcePointer<headerRackspaces> LnFRackspaces;
  SharedResourcePointer<headerLookAndFeel> headerLnF;
  SharedResourcePointer<subButtonLookAndFeel> LnFsubButtons;
  SharedResourcePointer<minimalistSong> minLnF;
  SharedResourcePointer<subButtonHighlightLookAndFeel> highlightLnF;
  SharedResourcePointer<blankButtonLookAndFeel> blankLnF;
  MyDraggableComponent draggableResizer;
  static ExtensionWindow* myInstance;
  static bool zeroBasedNumbering;
 
 private:
  TooltipWindow tooltipWindow;
  std::unique_ptr<docWindow> myWindow;
  Viewport myViewport;
  Viewport myViewportRight;
  Component container;
  Component containerRight;
  OwnedArray<TextButton> buttons;
  OwnedArray<TextButton> subButtons;
  MyTimer clockTimer;
  bool displayRightPanel = false;
  bool displayWindowOnLoad = false;
  std::unique_ptr<int> switchImmediately;
  int prevButtonSelected = 0;
  juce::OwnedArray<juce::OwnedArray<juce::TextButton>> buttonsLevel2;

  std::unique_ptr<Label> label;
  std::unique_ptr<Label> highlight;
  std::unique_ptr<Label> header;
  std::unique_ptr<Label> clock;
  std::unique_ptr<TextButton> btnClear;
  std::unique_ptr<TextButton> btnCurrent;
  std::unique_ptr<TextButton> btnPrev;
  std::unique_ptr<TextButton> btnNext;
  std::unique_ptr<TextButton> btnModeSwitch;
  std::unique_ptr<DynamicObject> preferences;
  std::unique_ptr<juce::NamedValueSet> myVar;
  std::unique_ptr<ShapeButton> burgerButton;
  std::unique_ptr<ShapeButton> sidePanelOpenButton;
  std::unique_ptr<ShapeButton> sidePanelCloseButton;
  std::unique_ptr<ShapeButton> pinUnpinnedButton;
  std::unique_ptr<ShapeButton> pinPinnedButton;
  std::unique_ptr<ShapeButton> refreshButton;

  struct Content : public Component
    {
        void paint (Graphics& g) override
        {
            g.fillAll (Colours::hotpink);
        }
    };
    
  SidePanel sidePanel { "panel", 250, true, new Content() };

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ExtensionWindow)
};



