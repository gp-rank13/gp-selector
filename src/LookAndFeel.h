// Rackspace and song selector extension for Gig Performer by @rank13

#pragma once
using namespace juce;

class buttonLookAndFeel : public LookAndFeel_V4 {
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
    String btnColour = button.getProperties()["colour"];
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
      g.setColour (Colours::white);
      g.drawRoundedRectangle (buttonArea, cornerSize, 1.f);  
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
      int numberWidth = (int) font1.getStringWidthFloat("00");  // Base width on 2 digits
      auto buttonNumber = button.getProperties()["displayIndex"];
      // Button Name
      Font font2 (juce::jmax(button.getHeight () * 0.4f, 16.f));
		  g.setFont (font2);
      g.setColour (Colour(0xffe5e5e5));
			g.drawFittedText (button.getButtonText (),
				(leftIndent*4) + (numberWidth*1.5), yIndent, textWidth-(leftIndent*20), button.getHeight () - yIndent * 2,
				Justification::left, rows, 1.0f);
    }
	}

  void drawButtonBackground (juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
                              bool isButtonHighlighted, bool isButtonDown) {
    auto buttonArea = button.getLocalBounds().toFloat().reduced (0.5f);
    float cornerSize = 5.f;
    
    Colour myColour = Colour(0xff3f3f3f);
    String btnColour = button.getProperties()["colour"];
    myColour = Colour::fromString(btnColour);

    if (button.getToggleState()) {
      g.setColour (Colour(0xff6A6A6A));
      
    } else if (isButtonHighlighted && !isButtonDown) {
      g.setColour (Colour(0xff2f2f2f));
    } else if (isButtonDown) {
      g.setColour (Colour(0xff9a9a9a));
    } else {
      g.setColour (myColour);
    }   
    g.fillRoundedRectangle (buttonArea, cornerSize);     
  }
};

class subButtonHighlightLookAndFeel : public LookAndFeel_V4 {
public:
  void drawLabel (Graphics& g, Label& label) {
    auto labelArea = label.getLocalBounds().toFloat().reduced (0.5f);
    float cornerSize = 5.f;
    g.setColour (Colours::white);
    g.fillRoundedRectangle (labelArea, cornerSize);
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
    String btnColour = button.getProperties()["colour"];
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
    g.fillAll (Colour (0xff935a31));
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
    String title = (labelArea.getWidth() < 230 ? "Racks" : "Rackspaces");
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