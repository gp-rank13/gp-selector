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
  ComponentDragger dragger;
  ComponentBoundsConstrainer constrainer;
  void mouseDown (const MouseEvent& e) override
  {
      dragger.startDraggingComponent (this, e);
  }
  void mouseDrag (const MouseEvent& e) override
  {
      dragger.dragComponent (this, e, &constrainer);
  }
  void paint(Graphics& g) override 
  { 
      if (isMouseOver(true)) {
               g.setColour (Colour (0xff3B4D60));
      } else { 
            g.setColour (Colour (0xff555555));
      } 
      const juce::Rectangle<float> area = getLocalBounds().toFloat();
      g.fillRect(area.withWidth(5)); // Override displayed width of separator
  }

  void mouseEnter (const MouseEvent&) override {
      repaint();
  }

  void mouseExit (const MouseEvent&) override {
      repaint();
  }
};

class MyDocumentWindow : public DocumentWindow
{
  public:
  MyDocumentWindow() :
  DocumentWindow("GP Selecter", Colours::lightgrey, DocumentWindow::allButtons, true)
  {
    setWantsKeyboardFocus(true);
  }
  virtual ~MyDocumentWindow() { }
  virtual void closeButtonPressed () override;
};

class ExtensionWindow  :  public juce::Component,
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
  void static readPreferencesFile();
  void static songChanged(int songIndex, std::vector<std::string> songNames);
  void static songPartChanged(int songPartIndex, int songIndex);
  void static setlistChanged(int songIndex, std::vector<std::string> songNames);
  void static rackspaceChanged(int rackspaceIndex, std::vector<std::string> rackspaceNames);
  void static variationChanged(int variationIndex, int rackspaceIndex);
  void static updateButtonLabel(const String& label);
  void static addButtons(int count);
  void static updateButtonNames(std::vector<std::string> buttonNames);
  void static compareButtonNames(std::vector<std::string> buttonNames);
  void static addSubButtons(int count);
  void static updateSubButtonNames(std::vector<std::string> buttonNames);
  void static compareSubButtonNames(std::vector<std::string> buttonNames);
  std::vector<std::string> getSubButtonNamesByIndex(int index);
  bool static isButtonSelected(int index);
  int static getButtonSelected();
  int static getVisibleButtonCount();
  int static getVisibleSubButtonCount();
  void static selectButton(int index);
  void static updatePrevCurrNext(int index);
  bool static isSubButtonSelected(int index);
  bool static isSubButtonsCollapsed(); 
  void static selectSubButton(int index);
  void static selectButtonAndSubButton(int index, std::vector<std::string> buttonNames);
  void static updateClock(const String& formattedTime);
  void static setDisplayNumbering(bool display);
  void static setZeroBasedNumbering(bool zeroBased);
  void static setImmediateSwitching(bool immediateSwitch);
  void static setLargeScrollArea(bool largeScrollArea);
  void static updateViewportPositionForSubButtons();
  void static toggleDisplayNumbering();
  void static toggleZeroBasedNumbering();
  void static toggleImmediateSwitching();
  void static toggleLargeScrollArea();  
  void static toggleThickBorders();  
  String buttonName(int index);
  void static displayWindow(bool display);
  void static scrollWindow(double value);
  void static setTitleBarName(const String& name);
  void static processPreferencesDefaults(StringPairArray prefs);
  void static processPreferencesColors(StringPairArray prefs);
  void static removeColorKeywordFromName(bool remove);
  void static applyColorToRacksAndSongs(bool apply);
  void static applyColorToVariationsAndParts(bool apply);
  void static refreshUI();
  void static updateButtonLnF(std::string LnFname);
  void static setWindowPositionAndSize(int x, int y, int w, int h);
  Rectangle<int> getWindowPositionAndSize();
  Image static getWindowIcon();
  void mouseDrag (const MouseEvent& e) override
    {
        resized();
    }
 
  static ExtensionWindow* extension;
  MyDraggableComponent draggableResizer;
  static bool zeroBasedNumbering;
  SharedResourcePointer<buttonLookAndFeel> buttonsLnF;
  SharedResourcePointer<gridButtonLookAndFeel> gridButtonsLnF;
  SharedResourcePointer<headerSongs> headerSongsLnF;
  SharedResourcePointer<headerRackspaces> headerRackspacesLnF;
  SharedResourcePointer<headerLookAndFeel> headerLnF;
  SharedResourcePointer<subButtonLookAndFeel> subButtonsLnF;
  SharedResourcePointer<minimalistSong> minimalistSongLnF;
  SharedResourcePointer<subButtonHighlightLookAndFeel> highlightLnF;
  SharedResourcePointer<blankButtonLookAndFeel> blankLnF;

 private:
  TooltipWindow tooltipWindow;
  std::unique_ptr<MyDocumentWindow> extensionWindow;
  Viewport viewport;
  Viewport viewportRight;
  Component container;
  Component containerRight;
  OwnedArray<TextButton> buttons;
  OwnedArray<TextButton> subButtons;
  StringPairArray buttonColors;
  ClockTimer clockTimer;
  RefreshTimer refreshTimer;
  bool displayRightPanel = false;
  bool displayWindowOnLoad = false;
  std::unique_ptr<int> switchImmediately;
  int prevButtonSelected = 0;
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
  std::unique_ptr<ShapeButton> fullscreenActivateButton;
  std::unique_ptr<ShapeButton> fullscreenDeactivateButton;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ExtensionWindow)
};



