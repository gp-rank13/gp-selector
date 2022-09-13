# Rackspace and Song Selector Extension for Gig Performer

## Overview

An extension for [Gig Performer](https://gigperformer.com) that provides an additional resizable window for selecting rackspaces/variations or songs/parts. It has two display modes: a compact list of rackspaces/songs, or an expanded window with a large display of the current and previous/next rackspace or song.

The visibility of the window can also be controlled via widgets (e.g. it could be triggered by an external MIDI controller).

![gp-selector](https://user-images.githubusercontent.com/107261652/175796291-79d16e7a-5957-4ed1-b38f-5214405c74d2.gif)


> _Disclaimer: This is a user-created extension and is not supported by Deskew Technologies. Extensions can cause unexpected behaviour in Gig Performer, and in extreme situations could cause it to crash. Please test it thoroughly before using it in any live performances!_

## Installation

Download the extension (library) files for either MacOS or Windows from the [Releases](https://github.com/gp-rank13/gp-selector/releases) page.  

Mac users will need to move the _libGPSelector.dylib_ file into this folder and then restart Gig Performer:
```
/Users/Shared/Gig Performer/Extensions
```
Windows users will need to move the _GPSelector.dll_ file into this folder and then restart Gig Performer:
```
C:\Users\Public\Documents\Gig Performer\Extensions
```
When opening Gig Performer, a prompt will appear asking to confirm whether to install the extension.

## Usage
### Menu Bar

After installing the extension, the selector window can be displayed or hidden via a new **_Extensions_** entry in the Gig Performer menu bar. 

<img width="320" alt="gp-selector-menu-1 3" src="https://user-images.githubusercontent.com/107261652/189471671-aff86742-6581-4e07-a660-6c99a48b1fc2.png">

Additional menu bar items are available to:
* **Toggle Zero-Based Numbers**: Switch between the rackspace/song numbering starting from 0 or starting from 1.
* **Toggle Immediate Switching**: Control whether selecting a rackspace/song will immediately select it and the first variation/part. When disabled, this will allow a specific variation/part to be selected from any rackspace or song (without it's first variation/part automatically being activated).
* **Toggle Large Scroll Area**: Add a blank area to the right of the buttons to be used for scrolling the list with your finger on touchscreens. This reduces the likelihood of accidentally selecting a button when scrolling.


>Any changes to the default options will need to be re-applied every time Gig Performer is opened, unless you use the optional preferences file (refer to the details below).

### Header Icons

<img width="108" alt="gp-selector-toolbar-icons" src="https://user-images.githubusercontent.com/107261652/189511391-2905a110-011a-486f-b69f-0eb1422cad20.png">

The icons in the upper-right corner of the extension header provide functionality to:
* **Pin** the extension window to the top of all open windows.
* **Refresh** the list of rackspaces/songs and their corresponding variations/parts. This is useful if the list gets out of sync with Gig Performer.
* **Expand** the window to display the large rackspace/song names (current/previous/next). The previous and next names can be clicked to move to this rackspace/song.

### Widgets
A button widget can be used to open and close the selector window. In the widget properties _Advanced_ tab, enter the name _GP_SELECTOR_. 

<img width="200" alt="gp-selector-widget" src="https://user-images.githubusercontent.com/107261652/175796753-3a9d52b3-4ef8-48fd-bc8c-f18691ca07a4.png">

A knob or fader widget can be used to scroll the list of songs/rackspaces. In the widget properties _Advanced_ tab, enter the name _GP_SELECTOR_SCROLL_. Use the widget properties _Value_ tab to reverse the direction of the scrolling.

> It is recommended to use the Global Rackspace for these widgets.

### Custom Button Colors

Custom colors can be defined for the variation and song part buttons. This is managed via the optional preferences file (refer below), in which a set of keywords can be listed, along with a corresponding color value. Any variation or song part names that contain/match one of these keywords will have the color applied to the button.

<img width="180" alt="gp-selector-custom-colors" src="https://user-images.githubusercontent.com/107261652/189472228-be4a7592-e6a3-4622-98c1-8aec16dd28ea.png"> <img width="182" alt="gp-selector-custom-colors-prefs" src="https://user-images.githubusercontent.com/107261652/189472733-694038d4-d1cb-447e-a1e9-073cc8fff3ce.png">

### Preferences File

The optional preferences file included in the release download (_GPSelectorPrefs.txt_) can be added to the Extensions folder and will apply a number of default settings for the extension, including window position and size, as well as allow custom button colors to be defined.

```
[Defaults]
ZeroBasedNumbers = false
ImmediateSwitching = true
LargeScrollArea = false
RemoveColorKeywordFromName = false
PositionAndSize = 100,100,300,600

[Colors]
Clean = FF346573
Crunch = FFBF6509
Lead = FF960303
```
The option **RemoveColorKeywordFromName** will remove the keyword you use in the Colors section from the displayed variation or song part name e.g. a variation called "My Clean Guitar" will become "My Guitar". 

>Use the Panel widgets in Gig Performer to find the color codes. Transparency won’t be needed, so the first two characters should always be "FF" (which is the alpha/transparency setting).

>Switching between Panels view and Setlist view will reload the color settings from the preferences file. This allows colors to be added/updated without needing to restart Gig Performer.

**PositionAndSize** defines the default window position and size (x, y, w, h). The first two digits are the x,y coordinates of window's upper left corner. The second two digits set the window's width (w) and height (h).

>Click the 'Pin' icon in the header and it will display the window's current position and size in Gig Performer's _Script Logger_ (open via _Window_ menu). This provides a simple method to obtain your preferred default position and size.


## Building the Extension

To customize/build the extension, refer to the [Gig Performer SDK](https://github.com/gigperformer/gp-sdk) GitHub page.