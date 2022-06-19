# Rackspace and Song Selector Extension for Gig Performer

## Overview

An extension for [Gig Performer](https://gigperformer.com) that provides an additional window for selecting rackspaces/variations or songs/parts. It has two display modes: a compact list of rackspaces/songs, or an expanded window with a large display of the current and previous/next rackspaces or songs.

The visibility of the window can also be controlled via widgets (e.g. it could be triggered by an external MIDI controller).

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

## Building the Extension

To customize/build the extension, refer to the [Gig Performer SDK](https://github.com/gigperformer/gp-sdk) GitHub page.