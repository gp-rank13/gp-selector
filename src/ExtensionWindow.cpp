// Rackspace and song selector extension for Gig Performer by @rank13

#include <juce_events/juce_events.h>
#include "ExtensionWindow.h"
#include "LibMain.h"
#include "IconPaths.h"
#include "Constants.h"

ExtensionWindow* ExtensionWindow::extension = nullptr;
LibMain* lib = new LibMain(nullptr);     
extern std::string extensionPath;

ExtensionWindow::ExtensionWindow ()
{
    LookAndFeel::setDefaultLookAndFeel(buttonsLnF);
    clockTimer.startTimer (5000);
    refreshTimer.startTimer(1000);
    bool zeroBasedNumbering = false;

    preferences.reset (new DynamicObject);
    preferences->setProperty("ImmediateSwitching", true);
    preferences->setProperty("LargeScrollArea", false);
    preferences->setProperty("DisplayNumbers", true);
    preferences->setProperty("ZeroBasedNumbers", false);
    preferences->setProperty("RemoveColorKeywordFromName", false);
    preferences->setProperty("ThickBorders", false);
    preferences->setProperty("FixedHeight", "0");

    header.reset (new Label ("header", ""));
    addAndMakeVisible (header.get());
    header->setEditable (false, false, false);
    header->setBounds (0, 0, getWidth(), 50);
    header->setLookAndFeel(headerRackspacesLnF);
    
    label.reset (new Label ("title", ""));
    addAndMakeVisible (label.get());
    label->setFont (Font (25.00f, Font::plain).withTypefaceStyle ("Regular"));
    label->setJustificationType (Justification::centredLeft);
    label->setEditable (false, false, false);
    label->setBounds (15, 10, 336, 25);

    highlight.reset (new Label ("subButtonHighlight", ""));
    highlight->setEditable (false, false, false);
    highlight->setLookAndFeel(highlightLnF);

    clock.reset (new Label ("clock", "00:00"));
    addAndMakeVisible (clock.get());
    clock->setFont (Font (25.00f, Font::plain).withTypefaceStyle ("Regular"));
    clock->setJustificationType (Justification::centred);
    clock->setEditable (false, false, false);
    clock->setLookAndFeel(headerLnF);
    clock->setVisible(false);

    Path p;
    p.loadPathFromData (sidePanelOpenPathData, sizeof (sidePanelOpenPathData));
    sidePanelOpenButton.reset (new ShapeButton ( "sidePanelOpenButton", Colours::white, Colours::lightgrey, Colours::white ));
    sidePanelOpenButton->setShape (p, true, true, false);
    sidePanelOpenButton->setClickingTogglesState(true);
    sidePanelOpenButton->setTooltip("Expand window to display large song/rackspace titles");
    sidePanelOpenButton->addListener (this);
    addAndMakeVisible (sidePanelOpenButton.get());

    Path p2;
    p2.loadPathFromData (sidePanelClosePathData, sizeof (sidePanelClosePathData));
    sidePanelCloseButton.reset (new ShapeButton ( "sidePanelCloseButton", Colours::white, Colours::lightgrey, Colours::white ));
    sidePanelCloseButton->setShape (p2, true, true, false);
    sidePanelCloseButton->setClickingTogglesState(true);
    sidePanelCloseButton->setTooltip("Close expanded window");
    sidePanelCloseButton->addListener (this);
    addAndMakeVisible (sidePanelCloseButton.get());
    sidePanelCloseButton->setVisible(false);

    Path p3;
    p3.loadPathFromData (pinUnpinnedPathData, sizeof (pinUnpinnedPathData));
    p3.applyTransform(juce::AffineTransform::rotation(3.14159));
    pinUnpinnedButton.reset (new ShapeButton ( "pinUnpinnedButton", Colours::white, Colours::lightgrey, Colours::white ));
    pinUnpinnedButton->setShape (p3, true, true, false);
    pinUnpinnedButton->setClickingTogglesState(true);
    pinUnpinnedButton->setTooltip("Keep window on top");
    pinUnpinnedButton->addListener (this);
    addAndMakeVisible (pinUnpinnedButton.get());

    Path p4;
    p4.loadPathFromData (pinPinnedPathData, sizeof (pinPinnedPathData));
    p4.applyTransform(juce::AffineTransform::rotation(3.14159));
    pinPinnedButton.reset (new ShapeButton ( "pinPinnedButton", Colours::white, Colours::lightgrey, Colours::white ));
    pinPinnedButton->setShape (p4, true, true, false);
    pinPinnedButton->setClickingTogglesState(true);
    pinPinnedButton->setTooltip("Stop keeping window on top");
    pinPinnedButton->addListener (this);
    addAndMakeVisible (pinPinnedButton.get());
    pinPinnedButton->setVisible(false);

    Path p5;
    p5.loadPathFromData (fullscreenActivatePathData, sizeof (fullscreenActivatePathData));
    p5.applyTransform(juce::AffineTransform::verticalFlip(0));
    fullscreenActivateButton.reset (new ShapeButton ( "fullscreenActivateButton", Colours::white, Colours::lightgrey, Colours::white ));
    fullscreenActivateButton->setShape (p5, true, true, false);
    fullscreenActivateButton->setClickingTogglesState(true);
    fullscreenActivateButton->setTooltip("Enter fullscreen");
    fullscreenActivateButton->addListener (this);
    addAndMakeVisible (fullscreenActivateButton.get());
    fullscreenActivateButton->setVisible(false);

    Path p6;
    p6.loadPathFromData (fullscreenDeactivatePathData, sizeof (fullscreenDeactivatePathData));
    p6.applyTransform(juce::AffineTransform::verticalFlip(0));
    fullscreenDeactivateButton.reset (new ShapeButton ( "fullscreenDeactivateButton", Colours::white, Colours::lightgrey, Colours::white ));
    fullscreenDeactivateButton->setShape (p6, true, true, false);
    fullscreenDeactivateButton->setClickingTogglesState(true);
    fullscreenDeactivateButton->setTooltip("Exit fullscreen");
    fullscreenDeactivateButton->addListener (this);
    addAndMakeVisible (fullscreenDeactivateButton.get());
    fullscreenDeactivateButton->setVisible(false);

    btnCurrent.reset (new TextButton ("btnCurrent"));
    btnCurrent->setLookAndFeel(minimalistSongLnF);
    btnCurrent->setButtonText ("");
    btnCurrent->addListener (this);   

    btnPrev.reset (new TextButton ("btnPrev"));
    btnPrev->setLookAndFeel(minimalistSongLnF);
    btnPrev->setButtonText ("");
    btnPrev->addListener (this);   

    btnNext.reset (new TextButton ("btnNext"));
    btnNext->setLookAndFeel(minimalistSongLnF);
    btnNext->setButtonText ("");
    btnNext->addListener (this);   

    btnModeSwitch.reset (new TextButton ("btnModeSwitch"));
    btnModeSwitch->setLookAndFeel(blankLnF);
    btnModeSwitch->setButtonText ("");
    btnModeSwitch->setTooltip("Switch between rackspaces and songs");
    btnModeSwitch->addListener (this);  
    addAndMakeVisible (btnModeSwitch.get());

    for (int i = 0; i < DEFAULT_RACKSPACES_SONGS; ++i) {
        std::string number = std::to_string(i);
        std::string name = std::string(2 - number.length(), '0') + std::to_string(i); // Pad with leading zero
        auto button = new TextButton(name); 
        buttons.add(button);
        buttons[i]->setClickingTogglesState(true);
        buttons[i]->setRadioGroupId(1);
        buttons[i]->getProperties().set("index", i);
        buttons[i]->getProperties().set("displayIndex", i + 1); // Default to non zero-based
        buttons[i]->getProperties().set("type", "button"); 
        buttons[i]->setTriggeredOnMouseDown(true);
        buttons[i]->addListener(this);  
    }
    buttons[0]->setToggleState (true, dontSendNotification);

    for (int i = 0; i < DEFAULT_VARIATIONS_SONGPARTS; ++i) {
        std::string number = std::to_string(i);
        auto button = new TextButton(number); 
        subButtons.add(button);
        subButtons[i]->setLookAndFeel(subButtonsLnF);
        subButtons[i]->setClickingTogglesState(true);
        subButtons[i]->setRadioGroupId(2);
        subButtons[i]->getProperties().set("index", i);
        subButtons[i]->getProperties().set("displayIndex", i + 1); // Default to non zero-based
        subButtons[i]->getProperties().set("type", "subButton"); 
        subButtons[i]->setTriggeredOnMouseDown(true);
        subButtons[i]->addListener(this);  
    }
    subButtons[0]->setToggleState (true, dontSendNotification);
    container.setBounds(5, 50, 100, 500);
    container.addAndMakeVisible(highlight.get());

    for (size_t i = 0; i < buttons.size(); ++i) {
        container.addAndMakeVisible(buttons[i]);
        buttons[i]->setVisible(false);
    }

    for (size_t i = 0; i < subButtons.size(); ++i) {
        container.addAndMakeVisible(subButtons[i]);
        subButtons[i]->setVisible(false);
    }
    draggableResizer.addMouseListener(this, true);
    draggableResizer.setMouseCursor(MouseCursor::LeftRightResizeCursor);
    draggableResizer.setBounds (250,50, 15, getHeight());
    containerRight.addAndMakeVisible (btnCurrent.get());
    containerRight.addAndMakeVisible (btnPrev.get());
    containerRight.addAndMakeVisible (btnNext.get());

    viewport.setBounds(5, 40, 250, 50 * buttons.size());
    viewport.setViewedComponent(&container, false);
    viewportRight.setViewedComponent(&containerRight, false);
    viewport.getVerticalScrollBar().setColour(ScrollBar::thumbColourId, Colour::fromString(BACKGROUND_COLOR));
    addAndMakeVisible(viewport);
    addAndMakeVisible(viewportRight);
    addAndMakeVisible(draggableResizer);
    setSize (Rectangle<int>::fromString(DEFAULT_WINDOW_POSITION).getWidth(), 
             Rectangle<int>::fromString(DEFAULT_WINDOW_POSITION).getHeight()
            );

    extensionWindow.reset(new MyDocumentWindow());
    extensionWindow->setContentNonOwned(this, true);
    extensionWindow->setResizable(true, true);
    extensionWindow->setUsingNativeTitleBar(true);

    #if JUCE_MAC
        extensionWindow->setResizeLimits(180, 250, 10000, 10000);
    #else
        extensionWindow->getPeer()->setIcon(getWindowIcon());
        extensionWindow->setResizeLimits(200, 250, 10000, 10000);
        extensionWindow->setResizeLimits(200, 250, 10000, 10000);
    #endif
}

ExtensionWindow::~ExtensionWindow()
{
    label = nullptr;
    btnClear = nullptr;
    setLookAndFeel (nullptr);
    clockTimer.stopTimer();
    refreshTimer.stopTimer();
}

void ExtensionWindow::paint (Graphics& g)
{
    g.fillAll (Colour::fromString(BACKGROUND_COLOR));
}

void ExtensionWindow::setWindowPositionAndSize(int x, int y, int w, int h) {
    extension->extensionWindow->setTopLeftPosition (x, y);
    extension->extensionWindow->setSize(w, h);
    extension->resized();
}

Rectangle<int> ExtensionWindow::getWindowPositionAndSize() {
    return extension->extensionWindow->getBounds();
}

Image ExtensionWindow::getWindowIcon() {
    Image img;
    String imageBase64 = WINDOW_ICON;
    MemoryOutputStream mo;
    auto result = Base64::convertFromBase64(mo, imageBase64);
    if (result) {
        img = ImageFileFormat::loadFrom(mo.getData(), mo.getDataSize());
    }
    return img;
}

void ExtensionWindow::resized()
{
    int minWindowWidth = MIN_WINDOW_WIDTH;
    int largeScrollAreaWidth = LARGE_SCROLL_AREA_WIDTH;
    int fixedButtonHeight = jmin((int)preferences->getProperty("FixedHeight"), MAX_BUTTON_HEIGHT);
    bool hasFixedHeight = fixedButtonHeight == 0 ? false : true;
    int minButtonHeight = hasFixedHeight ? MIN_BUTTON_HEIGHT : DEFAULT_BUTTON_HEIGHT;

    Point<int> viewPos = viewport.getViewPosition();
    int columns = 1;
    int buttonHeightRatio = 5; // Ratio of width:height
    auto& lnf = buttons[0]->getLookAndFeel();
    std::string lnfName = typeid(lnf).name();
    if (lnfName == typeid(gridButtonLookAndFeel).name()) {
        columns = 2;
        buttonHeightRatio = 3;
    }

    auto bounds = container.getBounds();
    bool largeScrollArea = preferences->getProperty("LargeScrollArea");
    auto buttonSize = (largeScrollArea) ? bounds.getWidth() - largeScrollAreaWidth : bounds.getWidth();
    int buttonHeight = ((int)(buttonSize/buttonHeightRatio) < minButtonHeight) ? minButtonHeight : (int)(buttonSize / buttonHeightRatio);
    if (hasFixedHeight) {
        buttonHeight = jmax(minButtonHeight, fixedButtonHeight);
    } else {
        buttonHeight = jmin(buttonHeight, MAX_BUTTON_HEIGHT);
    }
    auto padding = buttonHeight * 0.1;
    int rowHeight = 1;
    auto x = draggableResizer.getX();

    if (!displayRightPanel) {
       x = getWidth();
    }
    // Width of 15 provides a wider area to select the resizer on touchscreens. The displayed width is overridden in the Paint method of MyDraggableComponent.
    draggableResizer.setBounds (juce::jmax(minWindowWidth, x), 50, 15, getHeight()); 
    int buttonDisplayCount = 0;
    for (int i = 0; i < buttons.size(); ++i) {
        if (buttons[i]->isVisible()) {
            ++buttonDisplayCount;
        }
    }

    int subButtonDisplayCount = 0;
    for (size_t i = 0; i < subButtons.size(); ++i) {
        if (subButtons[i]->isVisible()) {
            ++subButtonDisplayCount;
        }
    }

    // Don't display if only 1 sub button (except if not switching immediately)
    bool switchRackSongImmediately = preferences->getProperty("SwitchImmediately");
    if (subButtonDisplayCount == 1 && switchRackSongImmediately) {
        subButtonDisplayCount = 0;
        subButtons[0]->setVisible(false);
        highlight->setVisible(false);
    } else {
        highlight->setVisible(true);
    }

    // Calculate how many rows to display (based on column count)
    auto rows = (buttonDisplayCount + subButtonDisplayCount) / columns;
    rows = rows + 0.5 - (rows < 0); 
    int rowCount = (int)rows;

    header->setBounds (0, 0, getWidth(), 50);
    clock->setBounds (getWidth()/2-50, 0, 100, 50);
    clock->setVisible(getWidth() > 350 && containerRight.isVisible() ? true : false);
    
    auto r = header->getBounds();
    if (sidePanelOpenButton->isVisible()) {
        sidePanelOpenButton->setBounds (r.removeFromRight (50).withSizeKeepingCentre (25, 25));
    } else {
        sidePanelCloseButton->setBounds (r.removeFromRight (50).withSizeKeepingCentre (25, 25));
    }
    if (pinUnpinnedButton->isVisible()) {
        pinUnpinnedButton->setBounds (r.removeFromRight (22).withSizeKeepingCentre (25, 25));
    } else {
        r.setY(r.getY() + 5);
        pinPinnedButton->setBounds (r.removeFromRight (22).withSizeKeepingCentre (13, 13));
        r.setY(r.getY() - 5);
    }
    if (displayRightPanel) {
        if (fullscreenActivateButton->isVisible()) {
            fullscreenActivateButton->setBounds(r.removeFromRight(48).withSizeKeepingCentre (25, 25));
        } else {
            fullscreenDeactivateButton->setBounds(r.removeFromRight(48).withSizeKeepingCentre (25, 25));
        }
    }
   
    int scrollbarBuffer = 2;
    int selectedButton = 999;
    for (size_t i = 0; i < buttons.size(); ++i) {
         buttons[i]->setBounds (buttonSize * (i % columns) + padding,
                                       buttonHeight * (i / columns) + padding + (i > selectedButton ? buttonHeight * subButtonDisplayCount : 0),
                                       buttonSize - padding - scrollbarBuffer,
                                       buttonHeight - padding);
        if (buttons[i]->getToggleState()) {  // Display sub buttons
            selectedButton = i;
            for (size_t j = 0; j < subButtonDisplayCount; ++j) {
               subButtons[j]->setBounds (buttonSize * (j % columns) + padding,
                                       buttonHeight * ((j+1) / columns) + (buttonHeight * (i / columns) + padding),
                                       buttonSize - padding - scrollbarBuffer,
                                       buttonHeight - padding);
            }
            auto highlightPadding = (buttonHeight - padding) * 0.3;
            highlight->setBounds (buttonSize * (i % columns) + padding,
                                    buttonHeight + (buttonHeight * (i / columns) + padding),
                                    buttonSize - padding - scrollbarBuffer,
                                    buttonHeight * subButtonDisplayCount - padding
            );
            highlight->getProperties().set("buttonHeight", buttonHeight - padding);
        }
    }

    container.setBounds(0, 50, juce::jmax (minWindowWidth-10, x - 10), (buttons[0]->getHeight() + padding) * rowCount );
    containerRight.setBounds(juce::jmax (minWindowWidth-10, x - 10), 50, getWidth()- juce::jmax (minWindowWidth, x), getHeight()-50);
    
    viewport.setBounds(0, 50, juce::jmax (minWindowWidth, x), getHeight()-50);
    viewport.setViewPosition(viewPos);
    viewportRight.setBounds(juce::jmax (minWindowWidth, x), 50, getWidth() - juce::jmax (minWindowWidth, x), getHeight());

    btnCurrent->setBounds (0 , containerRight.getHeight()/4, containerRight.getWidth(), containerRight.getHeight()/2);
    btnPrev->setBounds (10 , 10, containerRight.getWidth()-10, containerRight.getHeight()/4);
    btnNext->setBounds (10 , containerRight.getHeight()*3/4, containerRight.getWidth()-10, containerRight.getHeight()/4);
    btnModeSwitch->setBounds (15, 0, getWidth() > 230 ? 120 : 60, 50);

    draggableResizer.setVisible(displayRightPanel);
    containerRight.setVisible(displayRightPanel);
}

void ExtensionWindow::refreshUI() {
    if (extension == nullptr) return;
    // Reset all buttons
    for (int i = 0; i < extension->buttons.size(); ++i) {
        extension->buttons[i]->setToggleState(false, dontSendNotification);
        extension->buttons[i]->setVisible(false);
    }
    // Reset all sub buttons
    for (size_t i = 0; i < extension->subButtons.size(); ++i) {
        extension->subButtons[i]->setToggleState(false, dontSendNotification);
        extension->subButtons[i]->setVisible(false);
    }
    // Reset large previous/current/next titles
    extension->btnPrev->setButtonText("");
    extension->btnCurrent->setButtonText("");
    extension->btnNext->setButtonText("");

    if (lib->inSetlistMode()) {
            updateButtonLabel(SONG_TITLE);
            setTitleBarName(SONG_WINDOW_TITLE);
            updateButtonNames(lib->getSongNames());
            updateSubButtonNames(lib->getSongPartNames(lib->getCurrentSongIndex()));
            selectButton(lib->getCurrentSongIndex());
            selectSubButton(lib->getCurrentSongpartIndex());
    } else {
            updateButtonLabel(RACKSPACE_TITLE);
            setTitleBarName(RACKSPACE_WINDOW_TITLE);
            updateButtonNames(lib->getRackspaceNames());
            updateSubButtonNames(lib->getVariationNames(lib->getCurrentRackspaceIndex()));
            selectButton(lib->getCurrentRackspaceIndex());
            selectSubButton(lib->getCurrentVariationIndex());
    }
    extension->resized();
}

void ExtensionWindow::setTitleBarName(const String& name) {
    extension->extensionWindow->setName(name);
}

void ExtensionWindow::toggleDisplayNumbering() {
    bool status = extension->preferences->getProperty("DisplayNumbers");
    extension->preferences->setProperty("DisplayNumbers", !status); 
    refreshUI();
}

void ExtensionWindow::setZeroBasedNumbering(bool zeroBased) {
    extension->preferences->setProperty("ZeroBasedNumbers", zeroBased); 
    int offset = zeroBased ? 0 : 1;
    for (int i = 0; i < extension->buttons.size(); ++i) {
        extension->buttons[i]->getProperties().set("displayIndex", i + offset);
    }
    extension->resized();
}

void ExtensionWindow::toggleZeroBasedNumbering() {
    bool status = extension->preferences->getProperty("ZeroBasedNumbers");
    extension->preferences->setProperty("ZeroBasedNumbers", !status); 
    setZeroBasedNumbering(!status);
}

void ExtensionWindow::setImmediateSwitching(bool immediateSwitch) {
    extension->preferences->setProperty("ImmediateSwitching", immediateSwitch);     
}

void ExtensionWindow::toggleImmediateSwitching() {
    bool status = extension->preferences->getProperty("ImmediateSwitching");
    extension->preferences->setProperty("ImmediateSwitching", !status);     
}

void ExtensionWindow::setLargeScrollArea(bool largeScrollArea) {
    extension->preferences->setProperty("LargeScrollArea", largeScrollArea); 
    extension->resized();    
}

void ExtensionWindow::toggleLargeScrollArea() {
    bool status = extension->preferences->getProperty("LargeScrollArea");
    extension->preferences->setProperty("LargeScrollArea", !status); 
    extension->resized();   }

void ExtensionWindow::toggleThickBorders() {
    bool status = extension->preferences->getProperty("ThickBorders");
    extension->preferences->setProperty("ThickBorders", !status); 
    refreshUI();
}

String ExtensionWindow::buttonName(int index) {
    String name = "";
    if (index < buttons.size()) {
        name = extension->buttons[index]->getButtonText();
    }
    return name;
}

bool ExtensionWindow::isButtonSelected(int index) {
    if (extension == nullptr) return false;
    bool selected = false;
    if (index < extension->subButtons.size() && index >= 0) {
        return extension->buttons[index]->getToggleState();
    }
    return false;
}

int ExtensionWindow::getButtonSelected() {
    int selected = 0;
    for (size_t i = 0; i < extension->buttons.size(); ++i) {
        if (extension->buttons[i]->getToggleState()) {
            selected = i;
            break;
        }
    }
    return selected;
}

int ExtensionWindow::getVisibleButtonCount() {
    int visible = 0;
    for (auto i = 0; i < extension->buttons.size(); ++i) {
        if (extension->buttons[i]->isVisible()) {
            ++visible;
        }
    }
    return visible;
}

int ExtensionWindow::getVisibleSubButtonCount() {
    int visible = 0;
    for (auto i = 0; i < extension->subButtons.size(); ++i) {
        if (extension->subButtons[i]->isVisible()) {
            ++visible;
        }
    }
    return visible;
}

void ExtensionWindow::selectButton(int index) {
    if (extension == nullptr) return;
    if (index < extension->buttons.size() && index >= 0) {
        extension->buttons[index]->setToggleState(true, dontSendNotification);
        Rectangle<int> viewportBounds = extension->viewport.getViewArea();
        Rectangle<int> buttonBounds = extension->buttons[index]->getBounds();
        auto pad = buttonBounds.getWidth() / 40;
        pad = pad + 0.5 - (pad < 0); 
        int padding = (int)pad;
        int viewY = viewportBounds.getY();
        int btnY = buttonBounds.getY();
        if (btnY < viewY) {
            extension->viewport.setViewPosition(0, btnY - padding);
        }
        updatePrevCurrNext(index);
    }
}

void ExtensionWindow::updatePrevCurrNext(int index) {
    if (index < extension->buttons.size() && index >= 0) {
        String text = extension->buttons[index]->getButtonText();
        extension->btnCurrent->setButtonText(text);
        if (index > 0) {
            text = extension->buttons[index-1]->getButtonText();
            extension->btnPrev->setButtonText(text);
        } else {
            extension->btnPrev->setButtonText("");
        }
        if (index < extension->buttons.size()-1) {
            text = extension->buttons[index+1]->getButtonText();
            extension->btnNext->setButtonText(text);
        } else {
            extension->btnNext->setButtonText("");
        }
    }
}

bool ExtensionWindow::isSubButtonSelected(int index) {
    if (extension == nullptr) return false;
    bool selected = false;
    if (index < extension->subButtons.size() && index >= 0) {
        return extension->subButtons[index]->getToggleState();
    }
    return false;
}

bool ExtensionWindow::isSubButtonsCollapsed() {
    bool collapsed = true;
    int buttonCount = extension->subButtons.size();
    if (buttonCount > 1) { // If only 1 button it will be collapsed by default
        for (size_t i = 0; i < buttonCount; ++i) {
            if (extension->subButtons[i]->isVisible()) {
                collapsed = false;
                break;
            }
        }
    }
    return collapsed;
}

void ExtensionWindow::selectSubButton(int index) {
    if (extension == nullptr) return;
    if (index < extension->subButtons.size() && index >= 0) {
        extension->subButtons[index]->setToggleState(true, dontSendNotification);
        updateViewportPositionForSubButtons();
    }
}

void ExtensionWindow::addButtons(int count) {
    int buttonCount = extension->buttons.size();
    int index;
    int offset = extension->preferences->getProperty("ZeroBasedNumbers") ? 0 : 1;
    for (auto i = 0; i < count; ++i) {
        index = buttonCount + i;
        std::string number = std::to_string(index);
        auto button = new TextButton(number); 
        extension->buttons.add(button);
        extension->buttons[index]->setLookAndFeel(extension->buttonsLnF);
        extension->buttons[index]->setClickingTogglesState(true);
        extension->buttons[index]->setRadioGroupId(1);
        extension->buttons[index]->getProperties().set("index", index);
        extension->buttons[index]->getProperties().set("displayIndex", index + offset);
        extension->buttons[index]->getProperties().set("type", "button"); 
        extension->buttons[index]->setTriggeredOnMouseDown(true);
        extension->buttons[index]->addListener(extension);  
        extension->container.addAndMakeVisible(extension->buttons[index]);
    }
}

void ExtensionWindow::updateButtonNames(std::vector<std::string> buttonNames) {
    if (extension == nullptr) return;
    int newButtonCount = buttonNames.size();
    int currentButtonCount = extension->buttons.size();
    bool border = extension->preferences->getProperty("ThickBorders");
    bool displayNumber = extension->preferences->getProperty("DisplayNumbers");
    String textColor = extension->preferences->getProperty("TextColor");
    if (newButtonCount > currentButtonCount) {
        addButtons(newButtonCount-currentButtonCount);
        currentButtonCount = newButtonCount;
    }
    for (auto i = 0; i < currentButtonCount; ++i) {
        if (i < newButtonCount) {
            String name = buttonNames[i];
            String color = DEFAULT_BUTTON_COLOR;
            extension->buttons[i]->setButtonText(name);
            extension->buttons[i]->getProperties().set("name", name);
            extension->buttons[i]->setVisible(true);
            if (extension->preferences->getProperty("ApplyColorToRacksAndSongs")) {
                StringArray keys = extension->buttonColors.getAllKeys();
                for (int j = 0; j < keys.size(); ++j ) {
                    if (name.contains(keys[j])) {
                        color = extension->buttonColors.getValue(keys[j],"");
                        if (extension->preferences->getProperty("RemoveColorKeywordFromName")) {
                            name = name.replace(keys[j], "");
                            name = name.replace("  ", " ");
                            extension->buttons[i]->setButtonText(name);
                        }
                    }
                }
            }
            extension->buttons[i]->getProperties().set("color", color);
            extension->buttons[i]->getProperties().set("textColor", textColor);
            extension->buttons[i]->getProperties().set("displayNumber", displayNumber);
            extension->buttons[i]->getProperties().set("thickBorder", border);
        } else {
            extension->buttons[i]->setButtonText("");
            extension->buttons[i]->setVisible(false);
            extension->buttons[i]->getProperties().set("color", DEFAULT_BUTTON_COLOR);
            extension->buttons[i]->getProperties().set("textColor", DEFAULT_BUTTON_TEXT_COLOR);
            extension->buttons[i]->getProperties().set("name", "");
        }
    } 
    extension->resized();
 }

void ExtensionWindow::compareButtonNames(std::vector<std::string> newButtonNames) {
    int buttonCount = extension->buttons.size();
    int newButtonCount = newButtonNames.size();
    int visibleButtons = getVisibleButtonCount();
    if (visibleButtons != newButtonCount) {
        refreshUI();
    } else {
        for (auto i = 0; i < newButtonCount; ++i) {
            if (i < buttonCount && newButtonNames[i] != extension->buttons[i]->getProperties()["name"]) {
                refreshUI();
            }
        }
    }
}

void ExtensionWindow::addSubButtons(int count) {
    int buttonCount = extension->subButtons.size();
    int index;
    int offset = extension->preferences->getProperty("ZeroBasedNumbers") ? 0 : 1;
    for (auto i = 0; i < count; ++i) {
        index = buttonCount + i;
        std::string number = std::to_string(index);
        auto button = new TextButton(number); 
        extension->subButtons.add(button);
        extension->subButtons[index]->setLookAndFeel(extension->subButtonsLnF);
        extension->subButtons[index]->setClickingTogglesState(true);
        extension->subButtons[index]->setRadioGroupId(2);
        extension->subButtons[index]->getProperties().set("index", index);
        extension->subButtons[index]->getProperties().set("displayIndex", index + offset);
        extension->subButtons[index]->getProperties().set("type", "subButton"); 
        extension->subButtons[index]->setTriggeredOnMouseDown(true);
        extension->subButtons[index]->addListener(extension);  
        extension->container.addAndMakeVisible(extension->subButtons[index]);
    }
}

void ExtensionWindow::updateSubButtonNames(std::vector<std::string> buttonNames) {
    if (extension == nullptr) return;
    int newButtonCount = buttonNames.size();
    int currentButtonCount = extension->subButtons.size();
    bool border = extension->preferences->getProperty("ThickBorders");
    String borderColor = extension->preferences->getProperty("BorderColor");
    String textColor = extension->preferences->getProperty("TextColor");

    if (newButtonCount > currentButtonCount) {
        addSubButtons(newButtonCount-currentButtonCount);
        currentButtonCount = newButtonCount;
    }
    for (auto i = 0; i < currentButtonCount; ++i) {
        if (i < newButtonCount) {
            String name = buttonNames[i];
            String color = DEFAULT_SUBBUTTON_COLOR;
            extension->subButtons[i]->setButtonText(name);
            extension->subButtons[i]->getProperties().set("name", name);
            extension->subButtons[i]->setVisible(true);
            if (extension->preferences->getProperty("ApplyColorToVariationsAndParts")) {
                StringArray keys = extension->buttonColors.getAllKeys();
                for (int j = 0; j < keys.size(); ++j ) {
                    if (name.contains(keys[j])) {
                        color = extension->buttonColors.getValue(keys[j],"");
                        if (extension->preferences->getProperty("RemoveColorKeywordFromName")) {
                            name = name.replace(keys[j], "");
                            name = name.replace("  ", " ");
                            extension->subButtons[i]->setButtonText(name);
                        }
                    }
                }
            }
            extension->subButtons[i]->getProperties().set("color", color);
            extension->subButtons[i]->getProperties().set("textColor", textColor);
            extension->subButtons[i]->getProperties().set("thickBorder", border);
            extension->subButtons[i]->getProperties().set("borderColor", borderColor);
        } else {
            extension->subButtons[i]->setButtonText("");
            extension->subButtons[i]->setVisible(false);
            extension->subButtons[i]->getProperties().set("color", DEFAULT_SUBBUTTON_COLOR);
            extension->subButtons[i]->getProperties().set("textColor", DEFAULT_SUBBUTTON_TEXT_COLOR);
            extension->subButtons[i]->getProperties().set("name", "");
        }
    }
    extension->resized();
 }

 void ExtensionWindow::compareSubButtonNames(std::vector<std::string> newButtonNames) {
    int buttonCount = extension->subButtons.size();
    int visibleButtons = getVisibleSubButtonCount();
    int newButtonCount = newButtonNames.size();
    if (visibleButtons > 0) { // Ignore if collapsed
        if (visibleButtons != newButtonCount) {
            refreshUI();
        } else {
            for (auto i = 0; i < newButtonCount; ++i) {
                if (i < buttonCount && newButtonNames[i] != extension->subButtons[i]->getProperties()["name"]) {
                    refreshUI();
                }
            }
        }
    }
}

std::vector<std::string> ExtensionWindow::getSubButtonNamesByIndex(int index) {
    std::vector<std::string> names;
    if (lib->inSetlistMode()) {
        names = lib->getSongPartNames(index);
    } else {
        names = lib->getVariationNames(index);
    } 
    return names;
} 

void ExtensionWindow::updateButtonLnF(std::string LnFname) {
    auto& lnf = extension->buttons[0]->getLookAndFeel();
    std::string lnfName = typeid(lnf).name();

    if (lnfName == typeid(buttonLookAndFeel).name()) {
        for (size_t i = 0; i < extension->buttons.size(); ++i) {
            extension->buttons[i]->setLookAndFeel(extension->gridButtonsLnF);  
        }
    } else {
        for (size_t i = 0; i < extension->buttons.size(); ++i) {
            extension->buttons[i]->setLookAndFeel(extension->buttonsLnF);  
        }
    }
    extension->resized();
 }

void ExtensionWindow::updateButtonLabel(const String& text) {
    if (text == "Songs"){ 
        extension->header->setLookAndFeel(extension->headerSongsLnF);
    } else {
        extension->header->setLookAndFeel(extension->headerRackspacesLnF);
    }
    extension->resized();
}

void ExtensionWindow::updateViewportPositionForSubButtons() {
    Rectangle<int> viewportBounds = extension->viewport.getViewArea();
    Rectangle<int> buttonBounds = extension->buttons[getButtonSelected()]->getBounds();
    auto pad = buttonBounds.getWidth() / 40;
    pad = pad + 0.5 - (pad < 0); 
    int padding = (int)pad;
    int viewY = viewportBounds.getY();
    int viewH = viewportBounds.getHeight();
    int btnH = buttonBounds.getHeight();
    int btnY = padding + (getButtonSelected() * (btnH + padding));
    int subButtonCount = getVisibleSubButtonCount();
    int totalH = (btnH + padding) * (subButtonCount + 2);
    if ((btnY + totalH) > (viewY + viewH)) {
        int adjY = (viewH - totalH > 0) ? (viewH - totalH) : 0;
        extension->viewport.setViewPosition(0, (btnY - adjY) > 0 ? (btnY - adjY) : 0);
    } 
}

void ExtensionWindow::buttonClicked (Button* buttonThatWasClicked)
{
   if (buttonThatWasClicked == sidePanelOpenButton.get() || buttonThatWasClicked == sidePanelCloseButton.get())
    {
        displayRightPanel = !displayRightPanel;
        if (displayRightPanel) {
            auto bounds = draggableResizer.getBounds();
            setSize(container.getWidth() + 500, getHeight());
            sidePanelCloseButton->setVisible(true);
            sidePanelOpenButton->setVisible(false);
            fullscreenActivateButton->setVisible(true);
            clock->setVisible(true);
        } else {
            setSize(container.getWidth() + 10, getHeight());
            sidePanelCloseButton->setVisible(false);
            sidePanelOpenButton->setVisible(true);
            fullscreenActivateButton->setVisible(false);
            clock->setVisible(false);
        }
        resized();
      
    } else if (buttonThatWasClicked == btnPrev.get()) {
        if (lib->inSetlistMode()) {
            bool success = lib->switchToSong(juce::jmax(0, lib->getCurrentSongIndex()-1), 0);
        } else {
            bool success = lib->switchToRackspace(juce::jmax(0, lib->getCurrentRackspaceIndex()-1));
        }
    } else if (buttonThatWasClicked == btnNext.get()) {
        if (lib->inSetlistMode()) {
            bool success = lib->switchToSong(juce::jmin(buttons.size()-1,lib->getCurrentSongIndex()+1), 0);
        } else {
            bool success = lib->switchToRackspace(juce::jmin(buttons.size()-1,lib->getCurrentRackspaceIndex()+1));
        }
    } else if (buttonThatWasClicked == pinUnpinnedButton.get() || buttonThatWasClicked == pinPinnedButton.get()) {
        bool newPinnedStatus = !(extension->extensionWindow->isAlwaysOnTop());
        pinUnpinnedButton->setVisible(!newPinnedStatus);
        pinPinnedButton->setVisible(newPinnedStatus);
        extension->extensionWindow->setAlwaysOnTop(newPinnedStatus);
        if (newPinnedStatus) {
            Rectangle<int> window = getWindowPositionAndSize();
            lib->consoleLog("Pinned GP Selector (x,y,w,h): " + std::to_string(window.getX()) + "," + std::to_string(window.getY()) + "," + std::to_string(window.getWidth()) + "," + std::to_string(window.getHeight()));
        }
        resized();
    } else if (buttonThatWasClicked == fullscreenActivateButton.get() || buttonThatWasClicked == fullscreenDeactivateButton.get()) {
        bool newFullscreenStatus = !extension->extensionWindow->isFullScreen();
        #ifdef _WIN32
            newFullscreenStatus = !(Desktop::getInstance().getKioskModeComponent() == getTopLevelComponent());
            Desktop::getInstance().setKioskModeComponent(newFullscreenStatus ? getTopLevelComponent() : nullptr, false);
            if (!newFullscreenStatus) extension->extensionWindow->getPeer()->setIcon(getWindowIcon());
        #else
            extension->extensionWindow->setFullScreen(newFullscreenStatus);
        #endif

        fullscreenActivateButton->setVisible(!newFullscreenStatus);
        fullscreenDeactivateButton->setVisible(newFullscreenStatus);
        if (newFullscreenStatus) {
            sidePanelCloseButton->setColours(Colours::grey, Colours::grey, Colours::grey);
            sidePanelCloseButton->setTooltip("Disabled while in fullscreen");
            sidePanelCloseButton->removeListener(extension);
        } else {
            sidePanelCloseButton->setColours(Colours::white, Colours::lightgrey, Colours::white);
            sidePanelCloseButton->setTooltip("Close expanded window");
            sidePanelCloseButton->addListener(extension);
        }
        resized();
    } else if (buttonThatWasClicked->getProperties()["type"] == "button") {
        bool switchRackSongImmediately = preferences->getProperty("ImmediateSwitching");
        bool inSetlist = lib->inSetlistMode();
        size_t currentGPIndex = (inSetlist ? lib->getCurrentSongIndex() : lib->getCurrentRackspaceIndex());
        int buttonIndex = buttonThatWasClicked->getProperties()["index"];
        std::vector<std::string> blank;

        // Ensure other buttons are deselected
        for (size_t i = 0; i < buttons.size(); ++i) {
            if (buttonIndex != i) {
                buttons[i]->setToggleState(false, dontSendNotification);
            }
        }
        if (isSubButtonsCollapsed() || (buttonIndex != prevButtonSelected)) {
            // Expand
            updateSubButtonNames(getSubButtonNamesByIndex(buttonIndex));
            updateViewportPositionForSubButtons();
            if (buttonIndex != currentGPIndex && switchRackSongImmediately) {
                selectSubButton(0); // Force selection of first variation/part to avoid sync issues
            }
             // Ensure no button is selected on new song/rackspace if pref is set
            if (buttonIndex != currentGPIndex && !switchRackSongImmediately) {
                for (size_t i = 0; i < subButtons.size(); ++i) {
                    if (subButtons[i]->getToggleState()) {
                        subButtons[i]->setToggleState(false, dontSendNotification);
                    }
                }
            }
        } else {         
            // Collapse
            updateSubButtonNames(blank);
        }
        if (buttonIndex != currentGPIndex && switchRackSongImmediately) {
            if (inSetlist) {
                bool success = lib->switchToSong(buttonIndex, 0);
            } else {
                bool success = lib->switchToRackspace(buttonIndex, 0);
            }
            updatePrevCurrNext(buttonIndex);
        }
        prevButtonSelected = buttonIndex;
    } else if (buttonThatWasClicked->getProperties()["type"] == "subButton") {
        bool switchRackSongImmediately = preferences->getProperty("ImmediateSwitching");
        int subButtonIndex = buttonThatWasClicked->getProperties()["index"];
        int buttonIndex = getButtonSelected();
        if (lib->inSetlistMode()) {
            bool success = lib->switchToSong(buttonIndex, subButtonIndex);
        } else {
            bool success = lib->switchToRackspace(buttonIndex, subButtonIndex);
        }
        // Ensure other buttons are toggled off
        for (size_t i = 0; i < subButtons.size(); ++i) {
            if (i != subButtonIndex && subButtons[i]->getToggleState()) {
                subButtons[i]->setToggleState(false, dontSendNotification);
            }
        }

        // Ensure large song/rackspace labels are in sync
        if (!switchRackSongImmediately) {
            updatePrevCurrNext(buttonIndex);
        }
    } else if (buttonThatWasClicked == btnModeSwitch.get()) {
        lib->inSetlistMode() ? lib->switchToPanelView() : lib->switchToSetlistView();
    }
}

void ExtensionWindow::displayWindow(bool display) {
    if (extension->extensionWindow->isVisible() != display) {
        extension->extensionWindow->setVisible(display);
        lib->setWidgetValue(WIDGET_SELECTOR, (display == true ? 1.0 : 0.0));
    }
    if (display)
            extension->extensionWindow->toFront(true);
}

void ExtensionWindow::scrollWindow(double value) {
    Point<int> viewportPosition = extension->viewport.getViewPosition();
    Rectangle<int> viewportBounds = extension->viewport.getViewArea();
    Rectangle<int> containerBounds = extension->container.getBounds();
    int deltaH = containerBounds.getHeight() - viewportBounds.getHeight();
    deltaH = (deltaH < 0) ? 0 : deltaH;
    int newY = (int) (value * (double) deltaH);
    viewportPosition.setY(newY);
    extension->viewport.setViewPosition(viewportPosition);
}

void ExtensionWindow::initialize() {
    MessageManager::getInstance()->callAsync([]() {
        if (extension == nullptr) {
            extension = new ExtensionWindow();
            extension->extensionWindow->setTopLeftPosition(
                                            Rectangle<int>::fromString(DEFAULT_WINDOW_POSITION).getX(), 
                                            Rectangle<int>::fromString(DEFAULT_WINDOW_POSITION).getY()
                                        );
        }
        jassert(extension != nullptr);
        extension->extensionWindow->setVisible(false);
    });
}

void ExtensionWindow::finalize() {
    delete extension;
    extension = nullptr;
}

void ExtensionWindow::songChanged(int songIndex, std::vector<std::string> songNames) {
    if (extension == nullptr) return;
    MessageManager::getInstance()->callAsync([songIndex, songNames]() {
        updateButtonNames(songNames);
        if (!isButtonSelected(songIndex)) { // If selected in GP directly, ensure buttons are in sync
            selectButton(songIndex);
            updateSubButtonNames(lib->getSongPartNames(songIndex));
            selectSubButton(lib->getCurrentSongpartIndex());
        } else {
            updateSubButtonNames(lib->getSongPartNames(songIndex));
        }
    });
}

void ExtensionWindow::ExtensionWindow::songPartChanged(int songPartIndex, int songIndex) {
    if (extension == nullptr) return;
    MessageManager::getInstance()->callAsync([songPartIndex, songIndex]() {
        if (!isSubButtonSelected(songPartIndex)) {
            updateSubButtonNames(lib->getSongPartNames(songIndex));
            selectSubButton(songPartIndex);
        }
    });
}

void ExtensionWindow::setlistChanged(int songIndex, std::vector<std::string> songNames) {
    if (extension == nullptr) return;
        ExtensionWindow::updateButtonNames(songNames);
        ExtensionWindow::selectButton(songIndex);
}

void ExtensionWindow::rackspaceChanged(int rackspaceIndex, std::vector<std::string> rackspaceNames) {
    if (extension == nullptr) return;
    MessageManager::getInstance()->callAsync([rackspaceIndex, rackspaceNames]() {
        if (rackspaceIndex >= 0) {
            updateButtonNames(rackspaceNames);
            if (!isButtonSelected(rackspaceIndex)) { // If selected in GP directly, ensure buttons are in sync
                selectButton(rackspaceIndex);
                updateSubButtonNames(lib->getVariationNames(rackspaceIndex));
                selectSubButton(lib->getCurrentVariationIndex());
            } else {
                updateSubButtonNames(lib->getVariationNames(rackspaceIndex));
            }
        }
    });
}

void ExtensionWindow::variationChanged(int variationIndex, int rackspaceIndex) {
    if (extension == nullptr) return;
    MessageManager::getInstance()->callAsync([variationIndex, rackspaceIndex]() {
        if (!isSubButtonSelected(variationIndex)) {
            selectSubButton(variationIndex);
            updateSubButtonNames(lib->getVariationNames(rackspaceIndex));
        }
    });
}

void ExtensionWindow::readPreferencesFile() {
    if (extension == nullptr) return;
    std::string prefsFilePath = extensionPath + separator() + PREF_FILENAME;
    File file = File(prefsFilePath);
    String prefsFileText = file.loadFileAsString();
    StringArray lines = StringArray::fromLines(prefsFileText);
    StringArray keyValue;
    StringPairArray defaults;
    StringPairArray colors;
    String line;
    String prefSection;
    for (int i = 0; i < lines.size(); ++i) { 
        line = lines[i].toStdString();
        if (line.contains("[")) { // Preference Heading/Section
            prefSection = line.removeCharacters("[]");
        } else if (line.trim() != "") { // Process Preferences, assuming key/value pairs
            keyValue = StringArray::fromTokens(line,"=","");
            if (prefSection.contains("Defaults")) {
                defaults.set(keyValue[0].trim(), keyValue[1].trim());
            } else if (prefSection.contains("Colors")) {
                colors.set(keyValue[0].trim(), keyValue[1].trim());
            } 
        }
    }
    processPreferencesDefaults(defaults);
    processPreferencesColors(colors);
}

void ExtensionWindow::processPreferencesDefaults(StringPairArray prefs) {
    extension->preferences->setProperty("DisplayNumbers", prefs.getValue("DisplayNumbers", "") == "false" ? false : true);
    setZeroBasedNumbering(prefs.getValue("ZeroBasedNumbers", "") == "true" ? true : false);
    extension->preferences->setProperty("ImmediateSwitching", prefs.getValue("ImmediateSwitching", "") == "false" ? false : true);
    setLargeScrollArea(prefs.getValue("LargeScrollArea", "") == "true" ? true : false);
    removeColorKeywordFromName(prefs.getValue("RemoveColorKeywordFromName", "") == "true" ? true : false); 
    applyColorToRacksAndSongs(prefs.getValue("ApplyColorToRacksAndSongs", "") == "false" ? false : true); 
    applyColorToVariationsAndParts(prefs.getValue("ApplyColorToVariationsAndParts", "") == "false" ? false : true); 
    StringArray positionSize = StringArray::fromTokens(prefs.getValue("PositionAndSize", DEFAULT_WINDOW_POSITION), ",", "");
    setWindowPositionAndSize(positionSize[0].getIntValue(), positionSize[1].getIntValue(), positionSize[2].getIntValue(), positionSize[3].getIntValue());
    extension->preferences->setProperty("ThickBorders", prefs.getValue("ThickBorders", "") == "true" ? true : false);
    extension->preferences->setProperty("BorderColor", prefs.getValue("BorderColor", DEFAULT_BORDER_COLOR));
    extension->preferences->setProperty("TextColor", prefs.getValue("TextColor", DEFAULT_BUTTON_TEXT_COLOR));
    extension->preferences->setProperty("FixedHeight", prefs.getValue("FixedHeight", "0"));
}

void ExtensionWindow::processPreferencesColors(StringPairArray prefs) {
    extension->buttonColors.addArray(prefs);
}

void ExtensionWindow::removeColorKeywordFromName(bool remove) {
    extension->preferences->setProperty("RemoveColorKeywordFromName", remove); 
}

void ExtensionWindow::applyColorToRacksAndSongs(bool apply) {
    extension->preferences->setProperty("ApplyColorToRacksAndSongs", apply);
}

void ExtensionWindow::applyColorToVariationsAndParts(bool apply) {
    extension->preferences->setProperty("ApplyColorToVariationsAndParts", apply);
}

void ExtensionWindow::updateClock(const String& formattedTime) {
    extension->clock->setText(formattedTime, dontSendNotification);
}

void MyDocumentWindow::closeButtonPressed () { 
    ExtensionWindow::displayWindow(false);
}

void ClockTimer::timerCallback() {
    ExtensionWindow::updateClock(Time::getCurrentTime().toString(false, true, false, true));
}

void RefreshTimer::timerCallback() {
    ExtensionWindow::compareButtonNames(lib->inSetlistMode() ? lib->getSongNames() : lib->getRackspaceNames());
    ExtensionWindow::compareSubButtonNames(lib->inSetlistMode() ? lib->getSongPartNames(ExtensionWindow::getButtonSelected()) : lib->getVariationNames(ExtensionWindow::getButtonSelected()));
}

#if 0

#endif

