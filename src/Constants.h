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

// Widget name to open/close the selector window
const std::string WIDGET_SELECTOR = "GP_SELECTOR";
// List of menu items
const std::vector<std::string> menuNames = { "Show Selector", "Hide Selector", "Toggle Zero-Based Numbers", "Toggle Immediate Switching"}; //, "Change LnF"};

// Preferences file
const std::string PREF_FILENAME = "GPSelectorPrefs.txt";

// Maximum number of rackspaces/songs, variations/song parts that are supported by the extension
const int MAX_RACKSPACES_SONGS = 100;
const int MAX_VARIATIONS_SONGPARTS = 20;

#endif