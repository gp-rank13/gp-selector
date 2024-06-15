// Rackspace and song selector extension for Gig Performer by @rank13

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

// Define the file separator (Windows vs Mac) to be used when reading the preferences file
const inline char separator()
{
#ifdef _WIN32
    return '\\';
#else
    return '/';
#endif
}

// Window title bar and header labels
const std::string RACKSPACE_TITLE = "Rackspaces";
const std::string RACKSPACE_WINDOW_TITLE = "Rackspace Selector";
const std::string SONG_TITLE = "Songs";
const std::string SONG_WINDOW_TITLE = "Song Selector";

// Widget names to open/close the selector window, and scroll the rackspace/song lists
const std::string WIDGET_SELECTOR = "GP_SELECTOR";
const std::string WIDGET_SCROLL = "GP_SELECTOR_SCROLL";

// List of menu items
const std::vector<std::string> menuNames = { "Show Selector", "Hide Selector", "Toggle Zero-Based Numbers", "Toggle Immediate Switching", "Toggle Large Scroll Area", "Toggle Thick Borders"};

// Preferences file
const std::string PREF_FILENAME = "GPSelectorPrefs.txt";

// Default number of rackspaces/song and variations/song part buttons that are created by the extension
const int DEFAULT_RACKSPACES_SONGS = 50;
const int DEFAULT_VARIATIONS_SONGPARTS = 10;

// GUI defaults
const std::string DEFAULT_WINDOW_POSITION = "100,100,300,600";
const juce::String DEFAULT_BUTTON_COLOR = "FF3F3F3F";
const juce::String DEFAULT_SUBBUTTON_COLOR = "FF353535";
const juce::String DEFAULT_BORDER_COLOR = "FFE5E5E5";
const juce::String BACKGROUND_COLOR = "FF2A2A2A";
const juce::String WINDOW_ICON = "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";
#endif
