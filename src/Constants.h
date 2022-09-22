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

#endif