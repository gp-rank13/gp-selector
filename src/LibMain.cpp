// Rackspace and song selector extension for Gig Performer by @rank13

#include <regex>
#include "LibMain.h"

namespace gigperformer {
   namespace sdk {
      GigPerformerAPI *CreateGPExtension(LibraryHandle handle) {
         return new LibMain(handle);
      }
   } // namespace sdk
} // namespace gigperformer

bool isGigFileLoading = true;
bool isFirstGigFileOpened = false;
std::string extensionPath;

int LibMain::GetMenuCount()
{
   return menuNames.size();
}

 std::string  LibMain::GetMenuName(int index)
{
   std::string text;
   if (index >= 0 && index < menuNames.size())
       text =  menuNames[index];
         
   return text;      
}

void LibMain::InvokeMenu(int index)
{
   if (index >= 0 && index < menuNames.size())
         {
            switch (index)
               {
                  case 0:
                    ExtensionWindow::displayWindow(true);
                    break;
                  case 1:
                    ExtensionWindow::displayWindow(false);
                    break;
                  case 2:
                    ExtensionWindow::toggleDisplayNumbering();
                  case 3:
                    ExtensionWindow::toggleZeroBasedNumbering();
                    break;
                  case 4:
                    ExtensionWindow::toggleImmediateSwitching();
                    break;
                  case 5:
                    ExtensionWindow::toggleLargeScrollArea();
                    break;
                  case 6:
                    ExtensionWindow::toggleThickBorders();
                    break;
                  default:
                    break;   
               }
         }
}

std::vector<std::string> LibMain::getSongNames() {
    std::vector<std::string> songNames;
    std::string songName;
    for (int i = 0; i < getSongCount(); ++i) { 
        songName = getSongName(i);
        songNames.push_back(songName);
    }
    return songNames;
}

std::vector<std::string> LibMain::getSongPartNames(int songIndex) {
    std::vector<std::string> songPartNames;
    std::string songPartName;
    for (int i = 0; i < getSongpartCount(songIndex); ++i) { 
        songPartName = getSongpartName(songIndex, i);
        songPartNames.push_back(songPartName);
    }
    return songPartNames;
}

std::vector<std::string> LibMain::getRackspaceNames() {
    std::vector<std::string> rackspaceNames;
    std::string rackspaceName;
    for (int i = 0; i < getRackspaceCount(); ++i) { 
        rackspaceName = getRackspaceName(i);
        rackspaceNames.push_back(rackspaceName);
    }
    return rackspaceNames;
}

std::vector<std::string> LibMain::getVariationNames(int rackspaceIndex) {
    std::vector<std::string> variationNames;
    std::string variationName;
    for (int i = 0; i < getVariationCount(rackspaceIndex); ++i) { 
        variationName = getVariationName(rackspaceIndex, i);
        variationNames.push_back(variationName);
    }
    return variationNames;
}

std::vector<std::vector<std::string>> LibMain::getAllVariationNames() {
    std::vector<std::vector<std::string>> allVariationNames;
    std::vector<std::string> variationNames;
    for (int i = 0; i < getRackspaceCount(); ++i) { 
        variationNames = getVariationNames(i);
        allVariationNames.push_back(variationNames);
    }
    return allVariationNames;
}

void LibMain::OnStatusChanged(GPStatusType status) {
    switch (status) {
        case GPStatus_GigStartedLoading:
            isGigFileLoading = true;
            break;
        case GPStatus_GigFinishedLoading:
            isGigFileLoading = false;
            if (!isFirstGigFileOpened) {
                ExtensionWindow::readPreferencesFile();
                isFirstGigFileOpened = true;
            }
            ExtensionWindow::refreshUI();
            break;
        default:
            break;
    }
}

void LibMain::OnOpen() {
    extensionPath = getPathToMe();
    ExtensionWindow::initialize();
}

void LibMain::OnClose() {
    ExtensionWindow::finalize();
}

void LibMain::OnRackspaceActivated() {
    if (isGigFileLoading) return;
    if (!inSetlistMode()) {
        ExtensionWindow::rackspaceChanged(getCurrentRackspaceIndex(), getRackspaceNames()); 
    }
}

void LibMain::OnVariationChanged(int oldIndex, int newIndex) {
    if (isGigFileLoading) return;
    if (newIndex >= 0 && oldIndex != newIndex && !inSetlistMode()) {
        ExtensionWindow::variationChanged(newIndex, getCurrentRackspaceIndex());
    }
}

void LibMain::OnSongChanged(int oldIndex, int newIndex) {
    if (isGigFileLoading) return;
    if (newIndex >= 0 && inSetlistMode()) {
        ExtensionWindow::songChanged(newIndex, getSongNames());
    }
}

void LibMain::OnSongPartChanged(int oldIndex, int newIndex) {
    if (isGigFileLoading) return;
    if (newIndex >= 0 && oldIndex != newIndex && inSetlistMode()) {
        ExtensionWindow::songPartChanged(newIndex, getCurrentSongIndex());
    }
}

void LibMain::OnSetlistChanged(const std::string &newSetlistName) {
    if (isGigFileLoading) return;
    if (inSetlistMode()) {
        ExtensionWindow::setlistChanged(getCurrentSongIndex(), getSongNames());
    }
}

void LibMain::OnModeChanged(int mode) {
    if (isGigFileLoading) return;
    ExtensionWindow::refreshUI();
}

// Handle widget changes
void LibMain::OnWidgetValueChanged(const std::string &widgetName, double newValue) {
    if (widgetName == WIDGET_SELECTOR) {
        if (newValue == 1.0) {
         ExtensionWindow::displayWindow(true);
        } else {
            ExtensionWindow::displayWindow(false);
        }
    } else if (widgetName == WIDGET_SCROLL) {
            ExtensionWindow::scrollWindow(newValue);
    }
}

std::string LibMain::GetProductDescription()  // This MUST be defined in your class
    {
        return XMLProductDescription;
    }