# Rackspace and Song Selector Extension for Gig Performer

## Overview

An extension for [Gig Performer](https://gigperformer.com) that provides an additional resizable window for selecting rackspaces/variations or songs/parts. It has two display modes: a compact list of rackspaces/songs, or an expanded window with a large display of the current and previous/next rackspace or song.

The visibility of the window can also be controlled via widgets (e.g. it could be triggered by an external MIDI controller).

![gp-selector](https://user-images.githubusercontent.com/107261652/175796291-79d16e7a-5957-4ed1-b38f-5214405c74d2.gif)


> _Disclaimer: This is a user-created extension and is not supported by Deskew Techologies. Extensions can cause unexpected behaviour in Gig Performer, and in extreme situations could cause it to crash. Please test it thoroughly before using it in any live performances!_

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
When opening Gig Perfomer, a prompt will appear asking to confirm whether to install the extension.

## Usage

After installing the extension, the selector window can be displayed or hidden via a new _Extensions_ entry in the Gig Performer menu bar. Additional menu bar items are available to:
* Display zero-based rackspace/song numbers (e.g. the numbering starts from 0 instead of 1).
* Prevent immediate switching to the selected rackspace/song's first variation/part, which always allows a specific variation/part to be selected.

<img width="300" alt="gp-selector-menu" src="https://user-images.githubusercontent.com/107261652/175796818-e19670fe-7e12-4910-8064-6ce1ff013975.png">

_Note: Any changes to the default options will need to be re-applied every time Gig Performer is opened._

A button widget can be used to open and close the selector window. In the widget properties _Advanced_ tab, enter the name _GP_SELECTOR_. It's recommended to use the Global Rackspace for this widget.

<img width="200" alt="gp-selector-widget" src="https://user-images.githubusercontent.com/107261652/175796753-3a9d52b3-4ef8-48fd-bc8c-f18691ca07a4.png">

## Building the Extension

To customize/build the extension, refer to the [Gig Performer SDK](https://github.com/gigperformer/gp-sdk) GitHub page.