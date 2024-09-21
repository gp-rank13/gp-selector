// Rackspace and song selector extension for Gig Performer by @rank13

#pragma once

#include "Constants.h"

using namespace juce;

class buttonLookAndFeel : public LookAndFeel_V4 {
public:
	void drawButtonText (Graphics& g, TextButton& button,
		bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
	{
		Font font (button.getHeight () * 0.25f);
		g.setFont (font);
    Colour textColor = Colour::fromString(button.getProperties()["textColor"].toString());
    Colour selectedTextColor = Colours::white;
    if (textColor != Colour::fromString(DEFAULT_BUTTON_TEXT_COLOR)) selectedTextColor = textColor;
		g.setColour (button.findColour (button.getToggleState () ? TextButton::textColourOnId
			: TextButton::textColourOffId)
      .withMultipliedAlpha (0.5f));
    
    const auto buttonWidth = (double) button.getWidth();
    const auto buttonHeight = (double) button.getHeight();
    const String buttonText = button.getButtonText();
    const auto yIndent = buttonHeight * 0.1;
		const auto leftIndent = buttonWidth > 160 ? yIndent * 2 : 5;
    auto availableWidth = buttonWidth - leftIndent;
    const int rows = 1;
		
    // Button number
    float numberWidth = 0.0;
    bool displayNumber = button.getProperties()["displayNumber"];
    if (displayNumber) {
      Font font1 (juce::jmax(button.getHeight () * 0.4f, 16.f));
      g.setFont (font1);
      auto buttonNumber = button.getProperties()["displayIndex"];
      numberWidth = font1.getStringWidthFloat(button.getName()); 
      g.drawFittedText (buttonNumber,
        leftIndent, yIndent, numberWidth, buttonHeight - yIndent * 2,
        Justification::right, rows, 0.5f);
    }

    // Button Name
    Font font2 (juce::jmax(button.getHeight () * 0.4f, 16.f));
    g.setFont (font2);
    g.setColour (button.getToggleState () ? selectedTextColor : textColor);
    g.drawFittedText (buttonText,
      leftIndent + (numberWidth * 1.5), yIndent, availableWidth - (numberWidth * 1.5), buttonHeight - yIndent * 2,
      Justification::left, rows, 1.0f);
	}

  void drawButtonBackground (juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
                              bool isButtonHighlighted, bool isButtonDown) {
    auto buttonArea = button.getLocalBounds().toFloat();
    float borderSize = buttonArea.getHeight() * ((button.getProperties()["thickBorder"]) ? 0.08 : 0.04);
    float cornerSize = buttonArea.getHeight() * 0.08;
    Colour buttonColor = Colour::fromString(button.getProperties()["color"].toString());

    if (button.getToggleState()) {
      if (buttonColor == Colour::fromString(DEFAULT_BUTTON_COLOR)) {
        g.setColour (Colour(0xff6A6A6A));
      } else {
        g.setColour (buttonColor);
      }      
    } else if (isButtonHighlighted && !isButtonDown) {
      if (buttonColor == Colour::fromString(DEFAULT_BUTTON_COLOR)) {
        g.setColour (buttonColor.withBrightness(buttonColor.getBrightness() + 0.1f));
      } else {
        g.setColour (buttonColor.withBrightness(buttonColor.getBrightness() - 0.1f));
      }
    } else if (isButtonDown) {
      g.setColour (Colour(0xff9a9a9a));
    } else {
      g.setColour (buttonColor);
    }   
    
    g.fillRoundedRectangle (buttonArea, cornerSize);
    if (button.getToggleState()) {
      g.setColour (Colours::white);
      buttonArea = buttonArea.withSizeKeepingCentre(buttonArea.getWidth() - borderSize, buttonArea.getHeight() - borderSize);      
      g.drawRoundedRectangle (buttonArea, cornerSize, borderSize);  
    }               
  }
};

class subButtonLookAndFeel : public LookAndFeel_V4 {
public:
	void drawButtonText (Graphics& g, TextButton& button,
		bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
	{
		Font font (button.getHeight () * 0.25f);
		g.setFont (font);
    Colour textColor = Colour::fromString(button.getProperties()["textColor"].toString());
    Colour selectedTextColor = Colours::white;
    if (textColor != Colour::fromString(DEFAULT_SUBBUTTON_TEXT_COLOR)) selectedTextColor = textColor;
		g.setColour (button.findColour (button.getToggleState () ? TextButton::textColourOnId
			: TextButton::textColourOffId)
      .withMultipliedAlpha (0.5f));
    const auto buttonWidth = (double) button.getWidth();
    const auto buttonHeight = (double) button.getHeight();
    const auto padding = buttonHeight * 0.3;
    const String buttonText = button.getButtonText();
    const auto yIndent = buttonHeight * 0.1;
		const auto leftIndent = buttonWidth > 160 ? padding + (yIndent * 4) : padding + 5;

    // Button Name
    Font font2 (juce::jmax(buttonHeight * 0.4, 16.0));
    g.setFont (font2);
    g.setColour (button.getToggleState() ? selectedTextColor : textColor);
    auto availableWidth = buttonWidth - leftIndent;
    g.drawFittedText (buttonText,
      leftIndent, yIndent, availableWidth, buttonHeight - yIndent * 2,
      Justification::left, 1, 1.0f);
	}

  void drawButtonBackground (juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
                              bool isButtonHighlighted, bool isButtonDown) {
    auto buttonArea = button.getLocalBounds().toFloat();
    auto highlightArea = button.getLocalBounds().toFloat();;
    auto padding = buttonArea.getHeight() * 0.3;
    buttonArea.setLeft(padding);
    float borderSize = buttonArea.getHeight() * (button.getProperties()["thickBorder"] ? 0.08 : 0.04);
    float cornerSize = buttonArea.getHeight() * 0.08;
    Colour buttonColor = Colour::fromString(button.getProperties()["color"].toString());
    Colour borderColour = Colour::fromString(button.getProperties()["borderColor"].toString());

    if (button.getToggleState()) {
      if (buttonColor == Colour::fromString(DEFAULT_SUBBUTTON_COLOR)) {
        g.setColour (Colour(0xff6A6A6A));
      } else {
        g.setColour (buttonColor);
      }      
    } else if (isButtonHighlighted && !isButtonDown) {
      if (buttonColor == Colour::fromString(DEFAULT_SUBBUTTON_COLOR)) {
        g.setColour (buttonColor.withBrightness(buttonColor.getBrightness() + 0.1f));
      } else {
        g.setColour (buttonColor.withBrightness(buttonColor.getBrightness() - 0.1f));
      }
    } else if (isButtonDown) {
      g.setColour (Colour(0xff9a9a9a));
    } else {
      g.setColour (buttonColor);
    }   
    g.fillRoundedRectangle (buttonArea, cornerSize);  
    if (button.getToggleState()) {
      g.setColour (borderColour);
      buttonArea = buttonArea.withSizeKeepingCentre(buttonArea.getWidth() - borderSize, buttonArea.getHeight() - borderSize);
      g.drawRoundedRectangle (buttonArea, cornerSize, borderSize);  

      // Button Highlight
      highlightArea.setX(padding/4.0);
      highlightArea.setWidth(padding/6.0);
      g.setColour (Colours::white);
      g.fillRoundedRectangle (highlightArea, borderSize);  
    }            
  }
};

class subButtonHighlightLookAndFeel : public LookAndFeel_V4 {
public:
  void drawLabel (Graphics& g, Label& label) {
    auto labelArea = label.getLocalBounds().toFloat();
    auto highlightArea = label.getLocalBounds().toFloat();
    auto padding = (float)label.getProperties()["buttonHeight"] * 0.3;
    float cornerSize = 5.f;
    g.setColour (Colours::grey);
    highlightArea.setX(padding/4.0);
    highlightArea.setWidth(padding/6.0);
    g.fillRoundedRectangle (highlightArea, cornerSize);
  }
};

class blankButtonLookAndFeel : public LookAndFeel_V4 {
public:
	void drawButtonText (Graphics& g, TextButton& button,
		bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) {};

  void drawButtonBackground (juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
                               bool isButtonHighlighted, bool isButtonDown) {};
};

class gridButtonLookAndFeel : public LookAndFeel_V4 {
public:
	void drawButtonText (Graphics& g, TextButton& button,
		bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
	{
		Font font (button.getHeight () * 0.25f);
		g.setFont (font);
		g.setColour (button.findColour (button.getToggleState () ? TextButton::textColourOnId
			: TextButton::textColourOffId)
      .withMultipliedAlpha (0.5f));
    
		const int yIndent = button.proportionOfHeight (0.1f) * 2;
		const int cornerSize = jmin (button.getHeight (), button.getWidth ()) / 2;
		const int leftIndent = cornerSize / (button.isConnectedOnLeft () ?
                  yIndent * 2 : yIndent);
		const int rightIndent = cornerSize / (button.isConnectedOnRight () ? 
                  yIndent * 2 : yIndent);
		const int textWidth = button.getWidth () - leftIndent - rightIndent;
    const int rows = 1;
		if (textWidth > 100) {
      // Button number
      Font font1 (juce::jmax(button.getHeight () * 0.4f, 16.f));
		  g.setFont (font1);
      int numberWidth = (int) font1.getStringWidthFloat(button.getName());  // Base width on 2 digits
      auto buttonNumber = button.getProperties()["displayIndex"];
      g.drawFittedText (buttonNumber,
      leftIndent*4, yIndent, numberWidth, button.getHeight () - yIndent * 2,
      Justification::right, rows, 0.5f);
      
      // Button Name
      Font font2 (juce::jmax(button.getHeight () * 0.4f, 16.f));
		  g.setFont (font2);
      g.setColour  (button.getToggleState () ? Colours::white : Colour(0xffc5c5c5));
			g.drawFittedText (button.getButtonText (),
				(leftIndent*4) + (numberWidth*1.5), yIndent, textWidth-(leftIndent*20), button.getHeight () - yIndent * 2,
				Justification::left, rows, 1.0f);
    }
	}

  void drawButtonBackground (juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
                              bool isButtonHighlighted, bool isButtonDown) {
    auto buttonArea = button.getLocalBounds().toFloat().reduced (0.5f);
    float cornerSize = 5.f;
    Colour myColour = Colour(0xff1f1f1f);
    String btnColour = button.getProperties()["color"];
    myColour = Colour::fromString(btnColour);

    if (button.getToggleState()) {
      g.setColour (Colour(0xff6a6a6a));
      
    } else if (isButtonHighlighted && !isButtonDown) {
      g.setColour (Colour(0xff2f2f2f));
    } else if (isButtonDown) {
      g.setColour (Colour(0xff9a9a9a));      
    } else {
      g.setColour (myColour);
      
    }   
    g.fillRoundedRectangle (buttonArea, cornerSize);
    if (button.getToggleState()) {
      g.setColour (Colours::red);
      g.drawRoundedRectangle (buttonArea, cornerSize, 1.f);  
    }               
  }
};

class headerLookAndFeel : public LookAndFeel_V4 {
public:
  void drawLabel (Graphics& g, Label& label) {
    auto labelArea = label.getLocalBounds();
    g.setFont (Font (25.00f, Font::plain).withTypefaceStyle ("Regular"));
    g.setColour (Colours::white);
    g.drawFittedText (label.getText(),
				0, 0, label.getWidth(), label.getHeight (),
				Justification::centred, 1, 1.0f);
  }
};

class headerSongs : public LookAndFeel_V4 {
public:
  void drawLabel (Graphics& g, Label& label) {
    g.fillAll (Colour (0xff894B28));
    auto labelArea = label.getLocalBounds();
    g.setFont (Font (25.00f, Font::plain).withTypefaceStyle ("Regular"));
    g.setColour (Colours::white);
    g.drawFittedText ("Songs",
				15, 0, label.getWidth(), label.getHeight (),
				Justification::centredLeft, 1, 1.0f);
  }
};

class headerRackspaces : public LookAndFeel_V4 {
public:
  void drawLabel (Graphics& g, Label& label) {
    g.fillAll (Colour (0xff2c5e1e));
    auto labelArea = label.getLocalBounds();
    g.setFont (Font (25.00f, Font::plain).withTypefaceStyle ("Regular"));
    g.setColour (Colours::white);
    String title = (labelArea.getWidth() < 240 ? "Racks" : "Rackspaces");
    g.drawFittedText (title,
				15, 0, label.getWidth(), label.getHeight (),
				Justification::centredLeft, 1, 1.0f);
  }
};

class minimalistSong : public LookAndFeel_V4 {
public:
	void drawButtonText (Graphics& g, TextButton& button,
		bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
	{
		const int yIndent = button.proportionOfHeight (0.1f);
		const int cornerSize = jmin (button.getHeight (), button.getWidth ()) / 2;
		const int leftIndent = cornerSize / (button.isConnectedOnLeft () ?
                  yIndent * 2 : yIndent);
		const int rightIndent = cornerSize / (button.isConnectedOnRight () ? 
                  yIndent * 2 : yIndent);
		const int textWidth = button.getWidth () - leftIndent - rightIndent;

    g.setColour (button.findColour (button.getToggleState () ? TextButton::textColourOnId
      : TextButton::textColourOffId)
      .withMultipliedAlpha (button.isEnabled () ? 1.0f : 0.5f));
		if (textWidth > 100) {
      if (button.getName() == "btnCurrent") {
    		Font font (button.getHeight () * 0.5f);
        g.setFont (font);
        g.setColour (button.findColour (button.getToggleState () ? TextButton::textColourOnId
          : TextButton::textColourOffId)
          .withMultipliedAlpha (1.0f));
      }
      else if (button.getName() == "btnNext") {
        Font font (button.getHeight () * 0.5f);
        g.setFont (font);
        g.setColour (button.findColour (button.getToggleState () ? TextButton::textColourOnId
          : TextButton::textColourOffId)
          .withMultipliedAlpha (0.8f));
      } else {
        Font font (button.getHeight () * 0.5f);
        g.setFont (font);
        g.setColour (button.findColour (button.getToggleState () ? TextButton::textColourOnId
          : TextButton::textColourOffId)
          .withMultipliedAlpha (0.5f));
      }
      g.drawFittedText (button.getButtonText (),
				leftIndent, yIndent, textWidth, button.getHeight () - yIndent * 2,
				Justification::centred, 2, 0.8f);
    }
	}

  void drawButtonBackground (juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
                               bool isButtonHighlighted, bool isButtonDown) {
     if (button.getName() == "btnCurrent") {
        g.fillAll (Colour (0xff1C1C1C));
     }
  }
};