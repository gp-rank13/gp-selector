// Rackspace and song selector extension for Gig Performer by @rank13

#include <juce_events/juce_events.h>
#include "ExtensionWindow.h"
#include "LibMain.h"
#include "IconPaths.h"
#include "Constants.h"

ExtensionWindow* ExtensionWindow::extension = nullptr;
LibMain* lib = new LibMain(nullptr);     
StringPairArray buttonColors;

ExtensionWindow::ExtensionWindow ()
{
    LookAndFeel::setDefaultLookAndFeel(buttonsLnF);
    clockTimer.startTimer (1000);
    bool zeroBasedNumbering = false;

    preferences.reset (new DynamicObject);
    preferences->setProperty("SwitchImmediately", true);
    preferences->setProperty("LargeScrollArea", false);

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
    p5.loadPathFromData (refreshPathData, sizeof (refreshPathData));
    p5.applyTransform(juce::AffineTransform::verticalFlip(0));
    refreshButton.reset (new ShapeButton ( "refreshButton", Colours::white, Colours::lightgrey, Colours::white ));
    refreshButton->setShape (p5, true, true, false);
    refreshButton->setClickingTogglesState(true);
    refreshButton->setTooltip("Refresh list of songs/rackspaces");
    refreshButton->addListener (this);
    addAndMakeVisible (refreshButton.get());

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
    btnModeSwitch->addListener (this);  
    addAndMakeVisible (btnModeSwitch.get());

    for (int i = 0; i < MAX_RACKSPACES_SONGS; ++i) {
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

    for (int i = 0; i < MAX_VARIATIONS_SONGPARTS; ++i) {
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

    for (size_t i = 0; i < buttons.size(); ++i) {
        container.addAndMakeVisible(buttons[i]);
        buttons[i]->setVisible(false);
    }

    for (size_t i = 0; i < subButtons.size(); ++i) {
        container.addAndMakeVisible(subButtons[i]);
        subButtons[i]->setVisible(false);
    }
    container.addAndMakeVisible(highlight.get());
    draggableResizer.addMouseListener(this, true);
    draggableResizer.setMouseCursor(MouseCursor::LeftRightResizeCursor);
    draggableResizer.setBounds (250,50, 15, getHeight());
    containerRight.addAndMakeVisible (btnCurrent.get());
    containerRight.addAndMakeVisible (btnPrev.get());
    containerRight.addAndMakeVisible (btnNext.get());

    viewport.setBounds(5, 40, 250, 50 * buttons.size());
    viewport.setViewedComponent(&container, false);
    viewportRight.setViewedComponent(&containerRight, false);
    viewport.getVerticalScrollBar().setColour(ScrollBar::thumbColourId, Colour (0xff2a2a2a));
    addAndMakeVisible(viewport);
    addAndMakeVisible(viewportRight);
    addAndMakeVisible(draggableResizer);
    setSize (300, 600);

    extensionWindow.reset(new MyDocumentWindow());
    extensionWindow->setContentNonOwned(this, true);
    extensionWindow->setResizable(true, true);
    extensionWindow->setUsingNativeTitleBar(true);

    #if JUCE_MAC
        extensionWindow->setResizeLimits(180, 250, 10000, 10000);
    #else
        Image* smallImg = new Image(Image::ARGB, 22, 22, false);
        Image img;
        String gImageBase64 = "iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAYAAABzenr0AAAABGdBTUEAALGPC/xhBQAAACBjSFJNAAB6JgAAgIQAAPoAAACA6AAAdTAAAOpgAAA6mAAAF3CculE8AAAACXBIWXMAAAsTAAALEwEAmpwYAAABWWlUWHRYTUw6Y29tLmFkb2JlLnhtcAAAAAAAPHg6eG1wbWV0YSB4bWxuczp4PSJhZG9iZTpuczptZXRhLyIgeDp4bXB0az0iWE1QIENvcmUgNS40LjAiPgogICA8cmRmOlJERiB4bWxuczpyZGY9Imh0dHA6Ly93d3cudzMub3JnLzE5OTkvMDIvMjItcmRmLXN5bnRheC1ucyMiPgogICAgICA8cmRmOkRlc2NyaXB0aW9uIHJkZjphYm91dD0iIgogICAgICAgICAgICB4bWxuczp0aWZmPSJodHRwOi8vbnMuYWRvYmUuY29tL3RpZmYvMS4wLyI+CiAgICAgICAgIDx0aWZmOk9yaWVudGF0aW9uPjE8L3RpZmY6T3JpZW50YXRpb24+CiAgICAgIDwvcmRmOkRlc2NyaXB0aW9uPgogICA8L3JkZjpSREY+CjwveDp4bXBtZXRhPgpMwidZAAAKNElEQVRYCU1XWY8c1Rk91V3Ve0/P6llsz2qPNSYZDFgQRdg4ASNQHkgE4iHCCorES5SXRDwkfyBSRJaXKA9B5CUKiIeEyAIihcXgGGPjhbEDxpgZz2bPvnZXd3V3VXflnNvTI9dMdVXdqnu/823n+65Vr9dDy7IQBAFqtRpKpRLy+Tw2NjawtrZmrnrWu0gkAn3bvEajUfNsAQh56uB6CMPQXGOxGDKZDLLZLHK5HFpaWpBOpxGPx5FMJqH3tiZoQdu2US6XUalUjDAtoqO5mMYlHLAQtSmYdxECiHCszm8NCF4FVHMETmfzWXKkpN4JUFOm7fs+EokEzn9yHq/87vdwHJuT6maiJtXrNZ7UaEdIhLoGZQ/tHR0oF11UaLFsewcCzvGqVeKLoG5g8nbHWhLWEBhFterj5Zd/iePHjxswVrFYDAXi1Kmf4Mq1LzA6Mog6tdBkozGvUS4Ys0LEI9S25OKRJ57kO6DsFhFPpZBKZ3Dj6iWsLS8jdOIoE7BPm9RkRALXYaxCK9/8ZgpHj4zjzTffMIobC3ieByfmIJOKmw8TibjRWlLiESBBrRME4FC3ZHsrevv64Lkuerp7YNOPXrGIA4fGUL07AyfXggqtUUHEnAIiN4VhHSWvjHQybtxdpbVMDDT8TtMRac+ePYgRpcxp039OrQrHLVB4HSkCSUa5WGkLq59/ho7hg9ic/Bp+fgudBw7BX7qDzPIVBMUh1NxtCvOwVapgPUjgq0IJAwMHMDZ20MiQZRUPiitbUS808rNeyGAOQdS219HW1orB8e8i197O2IiB8nkSCY/11WX4Z/6OVP/9qAwdxMBDD2P8iaeQyTLSW7JI0jVOjNo6Dny69P33P8Crr76GPXu6jDWkeMi/XQAykQ5lQ3FpAY+eOI6xIw+iyskOF4lwXO4kRlqHVoonmEpZtLbkkKT5O7u60Nvbiy5aMcd0i+rDe47+/n5cvnwZN27cRGtrzqS7rGC79KUsENICtu0gv7qCE987gWdfOIXFxUWsrK5imcFV3VxXziIuDXv6EBBNZ+iiBQxYgpPQlTvzuD1xlRbIwaILpdK3H3jQWENxlkmnoIBXQBYKBaOYrRvjAg7adgS1xVk8f+ov6KY2Anfl0/Oo/vtv2J92kLYCxOo+0DeOatcA2jOdSNMvFoV3UvOFa59j6sWfIjIMbNzmSQADk5NwSDzT09O4+tGH6N43QBCBITtZ22YaGgCyViW/jaPfP4mDh0ZJIjYq9FPxz7/B/SceQ7weIE1tFYwpdwajq2dgMzNqkUOIMEZaW1vR3taGYIwE9ciPEM+9hRO/eh1JgltdWcbb//wHOmy6hf/il62trYYFmkGoAKy7efTu3YsYA07BU1haRG+SHEAiSfkVpKhtzlvD5uEnsbb/JdiMj86NJYwSQJZBFydDpgpAsDSJnsdeQu+Rh7BNQRc+OYev/vpb7H3kaSy6Hmp09a4LlAoVxoDFIEw59BvvBaqF3G2TBZNkoQS9mYqESJOIsmC8+GV80daFLHk9y286qL2OkEGVsYEqXZn6wXNwacH5mRlceO1PGHnwONaqZSpjoUx3uyKxeIxswUPMxx9TKAorDLyVFVQYLDnSrb1FADQbKQpJuYDo2ysFJGsB+trb0J4Tr1twCbxMzsjM8NunX0Q+nsTduTl8dvot7EOJdcOGozjjOhFea5wvVxBvg7MtDiZYofJztzF7ewpx3rf09CI6yLQLAzj0d4wOTMqNtFaavh0eGka1sG6q5kpApltcQOsLJ7HYN0hiWsY31yfQPvEOonu/hVKlytS0DZcQr8lnUyMEQOSjMQ3AK2Dyf9cxNTWFgKjTJ39G1rnFoHQYdCQpApHGowdJPgP9CJiWkwR889Yt5NdWMTX+Pawys+bmZpE4+y/s6TuAwK+SjiWjUTVFKAZDQ2ajUvHeuMJp6cDClQuYJoDZmVnUDh9BhQi1QBghQ+Znke0/iMOHD2OVHLG5vISF+XksLS3R5xVsM/ndkofKxbMYiddZlNhD7KooKUb5xlVV0gzwR1YQGYEMF9ydYh5/gzWS0kZoYemB5xBs3kItGmMIMsr53bWJCbz3wYdwv/4CNUOrWoAFizm/OT2F++6eg5Vub/QDhCBSqu9Akf7WDqVHoju1Wh8wFM3iYYoVbeJTLswqyUXdsSOYH34K/toVVNvGsTp/G9euX4fHDArXN2AxmNR8gNy/zC5qeOIMcp1DKDGQyZMIdtbW+irROuXGRgww/w0HmA/5MV9WaerQc1G+RC34Psd0Kz7+DBaOvIgCBfsE2NPdjTRzn941tGtTEZ9u6v7sXfQ7AQqMb5ViJjh8WjFonnyWJYzwhvKNGJAFqhSuSb7qOQtNeOcmPIKI0awdJJvoD3+MjV/8AW4sQV5PI5VMwGegRkhcKlh1zttbnUbFTqJENSsUqlMgDBBdKYNDuwBsaahTftFHCrgK/VMOaiw8e2DfOI8NcnbrM8+T7dKwjj3OHuAoecFCihbY5Dv1EHG2ch6vW1aWHRGU+eYsCwTXYxKiwnHJ0J/JOCrNZqXxoLSQqcp86fHqcYEifVhu24fqtTO4/fpr2FxZgkNfqyJqAfUN1QgbUwqP0QI1juXpQ1dzQwLYWafArNgm85U5LhmSteuCZmvNMZMJZU5sAIiiZEXh+mzVOwZRmb+JL1/5Nb46+xE8UnWC1lBrLQA2XRAnACuZxhYjzqX/i9S4xKtLS6ZHxtB93zjdwpbdiG8AkOxdJhQAHYoBma9GIXXWhlrowy+za3LYfLIezP7x55gfexxdJ55G69AIPC6qjkeZYLd1YImEHSUt58kJnuXApUJHxx8wPcPsMllzfZ0uTxgLyPoGQEN041fmUQB17R9EvVigVnFE1aqzVngsJomHn0WUpXn53Tew0jsMf2MFk15jM+MHhN/Rj0i6BbEexoIsZccwOTXJXs5Gngxp0pV2kAsMAA003aABOUIvjz9xEkGVTSP9vDAzjX0DA6ZS6ll7gZDFJKTANHc+diqDcmGbQGONHE+wprC3SHNHNTx6CB+f/S8+/M97GBzcz22DmFFyduJAQpvn+uYGyiwa2qioWl28dNnkuLiB5cwQijYfi3fvoM7nCjubgFWvTlDbNLnNVixKQHJJyMjXNxKoFqyrq4Nr1rHB/sAs17SAtFV/ruv6Zh4J5nipWMLbb7+DG1/eNJNloaufTyDDvI+x6bjLtnz0O4+y28nhzsdnEGNAxrk5KV24aNjUIiO2crekveWFi5doRYJnr6BWfXpmDj1sYMUbOqzTp0+HMusca/e5c+dNjRZibckEzGMKCbGaB42reGjj4pOGqRJishYDUHsJmtJs4fSsvk/ryrrquNXiNfIMOHbsmKmkalDJoA0TjYyMYGhoyAjhl7vHvTGib/Wsa3NjKqGcZL5vAG/sK6WxTjUdGtcmVffNNfROaxkAQtl8qXsdzbhoXncF3wOi+U4CdOjanK9n3d8rWDJ0aqyp+P8Be/ct73WWG78AAAAASUVORK5CYII=";
        MemoryOutputStream mo;
        auto result = Base64::convertFromBase64(mo, gImageBase64);
        if (result) {
            img = ImageFileFormat::loadFrom(mo.getData(), mo.getDataSize());
        }
        extensionWindow->getPeer()->setIcon(img);
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
}

void ExtensionWindow::paint (Graphics& g)
{
    g.fillAll (Colour (0xff2a2a2a));
}

void ExtensionWindow::resized()
{
    int minWindowWidth = 180;
    int largeScrollAreaWidth = 50;
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
    //auto buttonSize = (bounds.getWidth()) / columns;
    bool largeScrollArea = preferences->getProperty("LargeScrollArea");
    auto buttonSize = (largeScrollArea) ? bounds.getWidth() - largeScrollAreaWidth : bounds.getWidth();
    auto pad = buttonSize / 40;
    pad = pad + 0.5 - (pad < 0); 
    int padding = (int)pad;
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
        if (subButtonDisplayCount == 1) {
            //subButtons[0]->setToggleState(false, dontSendNotification);
        }
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
    refreshButton->setBounds (r.removeFromRight (25).withSizeKeepingCentre (25, 25));
    if (pinUnpinnedButton->isVisible()) {
        pinUnpinnedButton->setBounds (r.removeFromRight (35).withSizeKeepingCentre (25, 25));
    } else {
        pinPinnedButton->setBounds (r.removeFromTop (10));
        pinPinnedButton->setBounds (r.removeFromRight (35).withSizeKeepingCentre (13, 13));

    }
   
    int scrollbarBuffer = 2;
    int subButtonHeight = (buttonSize/buttonHeightRatio/columns) * subButtonDisplayCount;
    int selectedButton = 999;
    for (size_t i = 0; i < buttons.size(); ++i) {
         buttons[i]->setBounds (buttonSize * (i % columns) + padding,
                                       buttonSize/buttonHeightRatio * (i / columns) + padding + (i > selectedButton ? subButtonHeight : 0),
                                       buttonSize - padding - scrollbarBuffer,
                                       buttonSize/buttonHeightRatio - padding);
        if (buttons[i]->getToggleState()) {  // Display sub buttons
            selectedButton = i;
            for (size_t j = 0; j < subButtonDisplayCount; ++j) {
                subButtons[j]->setBounds (buttonSize * (j % columns) + (padding*4),
                                       buttonSize/buttonHeightRatio * ((j+1) / columns) + (buttonSize/buttonHeightRatio * (i / columns) + padding),
                                       buttonSize - (padding*4) - scrollbarBuffer,
                                       buttonSize/buttonHeightRatio - padding);
            }
            highlight->setBounds (padding*2, 
                                    buttonSize/buttonHeightRatio/columns + (buttonSize/buttonHeightRatio * (i / columns) + padding),
                                    padding/2,
                                    subButtonHeight - padding
            );
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
    if (lib->inSetlistMode()) {
            updateButtonLabel(SONG_TITLE);
            setTitleBarName(SONG_WINDOW_TITLE);
            updateButtonNames(lib->getSongNames());
            selectButton(lib->getCurrentSongIndex());
            updateSubButtonNames(lib->getSongPartNames(lib->getCurrentSongIndex()));
            selectSubButton(lib->getCurrentSongpartIndex());
    } else {
            updateButtonLabel(RACKSPACE_TITLE);
            setTitleBarName(RACKSPACE_WINDOW_TITLE);
            updateButtonNames(lib->getRackspaceNames());
            selectButton(lib->getCurrentRackspaceIndex());
            updateSubButtonNames(lib->getVariationNames(lib->getCurrentRackspaceIndex()));
            selectSubButton(lib->getCurrentVariationIndex());
    }
    extension->resized();
}

void ExtensionWindow::setTitleBarName(const String& name) {
    extension->extensionWindow->setName(name);
}

void ExtensionWindow::setZeroBasedNumbering(bool zeroBased) {
    extension->preferences->setProperty("ZeroBasedNumbers", zeroBased); 
    int offset = (zeroBased == true) ? 0 : 1;
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
    extension->resized();    
}

String ExtensionWindow::buttonName(int index) {
    String name = "";
    if (index < buttons.size()) {
        name = extension->buttons[index]->getButtonText();
    }
    return name;
}

bool ExtensionWindow::isButtonSelected(int index) {
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

void ExtensionWindow::selectButton(int index) {
    if (extension == nullptr) return;
    if (index < extension->buttons.size() && index >= 0) {
        extension->buttons[index]->setToggleState(true, dontSendNotification);
        Rectangle<int> viewportBounds = extension->viewport.getViewArea();
        Rectangle<int> buttonBounds = extension->buttons[index]->getBounds();

        auto pad = buttonBounds.getWidth() / 40;
        pad = pad + 0.5 - (pad < 0); 
        int padding = (int)pad;
        int viewY = viewportBounds.getY() + viewportBounds.getHeight();
        int btnY = buttonBounds.getY() + buttonBounds.getHeight();
        if (btnY > viewY) {
            extension->viewport.setViewPosition(0, viewportBounds.getY() + (buttonBounds.getY() + buttonBounds.getHeight() - viewY + padding));
        } else if (buttonBounds.getY() < viewportBounds.getY()) {
            extension->viewport.setViewPosition(0, buttonBounds.getY() - padding);
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

    if (index < extension->subButtons.size() && index >= 0) {
        extension->subButtons[index]->setToggleState(true, dontSendNotification);
    }
}

void ExtensionWindow::updateButtonNames(std::vector<std::string> buttonNames) {
    if (extension == nullptr) return;
    int buttonCount = buttonNames.size();

    for (size_t i = 0; i < extension->buttons.size(); ++i) {
        if (i < buttonCount) {
            extension->buttons[i]->setButtonText(buttonNames[i]);
            extension->buttons[i]->setVisible(true);
            extension->buttons[i]->getProperties().set("colour", "0xff3f3f3f");
        } else {
            extension->buttons[i]->setButtonText("");
            extension->buttons[i]->setVisible(false);
        }
    } 
    extension->resized();
 }

void ExtensionWindow::updateSubButtonNames(std::vector<std::string> buttonNames) {
    if (extension == nullptr) return;
    int buttonCount = buttonNames.size();
    for (auto i = 0; i < extension->subButtons.size(); ++i) {
        if (i < buttonCount) {
            extension->subButtons[i]->setButtonText(buttonNames[i]);
            extension->subButtons[i]->setVisible(true);
            StringArray keys = buttonColors.getAllKeys();
            String color = "ff353535";
            String name = buttonNames[i];
            for (int j = 0; j < keys.size(); ++j ) {
                if (name.contains(keys[j])) {
                    color = buttonColors.getValue(keys[j],"");
                    if (extension->preferences->getProperty("RemoveColorKeywordFromName")) {
                        name = name.replace(keys[j], "");
                        name = name.replace("  ", " ");
                        extension->subButtons[i]->setButtonText(name);
                    }
                }
            }
            extension->subButtons[i]->getProperties().set("colour", color);
        } else {
            extension->subButtons[i]->setButtonText("");
            extension->subButtons[i]->setVisible(false);
            extension->subButtons[i]->getProperties().set("colour", "FF353535");
        }
    }
    extension->resized();
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

void ExtensionWindow::updateButtonNamesAndColours(std::vector<std::string> buttonNames, std::vector<std::string> buttonColours) {
    if (extension == nullptr) return;
    int buttonCount = buttonNames.size();
    for (size_t i = 0; i < extension->buttons.size(); ++i) {
        if (i < buttonCount) {
            extension->buttons[i]->setButtonText(buttonNames[i]);
            extension->buttons[i]->setVisible(true);
            String colour = buttonColours[i];
            extension->buttons[i]->getProperties().set("colour", colour);
        } else {
            extension->buttons[i]->setButtonText("");
            extension->buttons[i]->setVisible(false);
        }
    }
 }

void ExtensionWindow::updateButtonLnF(std::string LnFname) {
    if (extension == nullptr) return;
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

void ExtensionWindow::updateButtonLevel2Names(std::vector<std::vector<std::string>> buttonNames) {
    if (extension == nullptr) return;
    int level1Count = buttonNames.size();
    int level2Count;
    for (size_t i = 0; i < level1Count; ++i) {
        level2Count = buttonNames[i].size();        
    }
}

void ExtensionWindow::updateButtonLabel(const String& text) {
    if (extension == nullptr) return;
    if (text == "Songs"){ 
        extension->header->setLookAndFeel(extension->headerSongsLnF);
    } else {
        extension->header->setLookAndFeel(extension->headerRackspacesLnF);
    }
    extension->resized();
}
void ExtensionWindow::buttonClicked (Button* buttonThatWasClicked)
{
   if (buttonThatWasClicked == sidePanelOpenButton.get() || buttonThatWasClicked == sidePanelCloseButton.get())
    {
        displayRightPanel = !displayRightPanel;
        if (displayRightPanel) {
            auto bounds = draggableResizer.getBounds();
            //draggableResizer.setBounds(bounds.getX() - 20, bounds.getY(), bounds.getWidth(), bounds.getHeight());
            setSize(container.getWidth() + 500, getHeight());
            sidePanelCloseButton->setVisible(true);
            sidePanelOpenButton->setVisible(false);
            clock->setVisible(true);
        } else {
            setSize(container.getWidth() + 10, getHeight());
            sidePanelCloseButton->setVisible(false);
            sidePanelOpenButton->setVisible(true);
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
        resized();
    } else if (buttonThatWasClicked == refreshButton.get()) {
        refreshUI();
    } else if (buttonThatWasClicked->getProperties()["type"] == "button") {
        bool switchRackSongImmediately = preferences->getProperty("SwitchImmediately");
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
        bool switchRackSongImmediately = preferences->getProperty("SwitchImmediately");
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
            extension->extensionWindow->setTopLeftPosition(100, 100);
        }
        jassert(extension != nullptr);
        extension->extensionWindow->setVisible(false);
    });
}

void ExtensionWindow::finalize()
{
    delete lib;
    lib = nullptr;

    delete extension;
    extension = nullptr;
}

void ExtensionWindow::processPreferencesDefaults(StringPairArray prefs) {
    setZeroBasedNumbering(prefs.getValue("ZeroBasedNumbers", "") == "true" ? true : false);
    extension->preferences->setProperty("ImmediateSwitching", prefs.getValue("ImmediateSwitching", "") == "false" ? false : true);
    setLargeScrollArea(prefs.getValue("LargeScrollArea", "") == "true" ? true : false);
    removeColorKeywordFromName(prefs.getValue("RemoveColorKeywordFromName", "") == "true" ? true : false); 
}

void ExtensionWindow::processPreferencesColors(StringPairArray prefs) {
    buttonColors.addArray(prefs);
}

void ExtensionWindow::removeColorKeywordFromName(bool remove) {

    extension->preferences->setProperty("RemoveColorKeywordFromName", remove); 

}

void ExtensionWindow::updateClock(const String& formattedTime) {
    extension->clock->setText(formattedTime, dontSendNotification);
}

void MyDocumentWindow::closeButtonPressed () { 
    ExtensionWindow::displayWindow(false);
}

void MyTimer::timerCallback() {
    ExtensionWindow::updateClock(Time::getCurrentTime().toString(false, true, false, true));
}

#if 0

#endif

