# Install `Inno Setup` development environment

The Inno Setup QuickStart Pack includes Inno Setup itself and additionally includes an option to install the Inno Script Studio script editor. This script editor is not official and is not required for general usage, but makes Inno Setup easier to use.

[Inno Setup Downloads page](https://www.jrsoftware.org/isdl.php)  
[innosetup-qsp-6.0.3.exe](https://www.jrsoftware.org/download.php/ispack.exe)

After the setup you can use the Inno Setup directly of the Inno Setup Studio for an easier project edition. 

## `Inno Setup Studio`

Probably Inno Setup Studio will need to setup the compiler path, go to `Tools > Options > Compiler Settings > Compiler Directory` and setup it with this directory `"C:\Program Files (x86)\Inno Setup 6"`

## Install the `Inno Download Plugin` 

Go to: [Inno Download Plugin webpage](https://mitrichsoftware.wordpress.com/inno-setup-tools/inno-download-plugin/) and install the following file: [Download version 1.5.1](https://bitbucket.org/mitrich_k/inno-download-plugin/downloads/idpsetup-1.5.1.exe)

# Create a project folder

Create a project folder with the following elements

* package: A folder containing all the files needed for the application;
* output: A folder where the compiler will place the setup file 
* iss project file: The Inno Setup source code;
* `ico` file for the setup
* needed setup files just for archive

# `7-zip` unpacker

* Download the `7-zip` standalone console version from: [7-Zip Extra: standalone console version, 7z DLL, Plugin for Far Manager](https://www.7-zip.org/a/7z1900-extra.7z)
* Extract the file `7za.exe` to the projects folder

# Create a new setup project

* Open the Inno Setup Compiler
* Create a new project using the `wizard`
* Fill in the following data:
    * Application Name: `Sapphire Core`
    * Application Version: `1.2.0.0`
    * Application Publisher: `2020 Sapphire Core Developers`
    * Application Website: `https://sappcoin.com/`
* Setup the destination folder:
    * Program Files Folder
    * Folder name: `Sapphire`
* Add application files:
    * Select the file `sap-pt.exe` as main executable from the `package` folder
    * Add the folder package in the other application files
* Add all application shortcuts
* Install mode: `Administrative` and add the possibility to choose at startup
* Select the setup languages
* Compiler settings
    * Select the output folder previous created
    * Edit the output base filename to: `sap-1.1.0-win64-setup.exe`
    * Add the icon file for the application setup
* Save the resulting `Inno Setup` project file (*.iss file)

## Adjust the project

# How to install the Inno Setup compiler on Ubuntu 18.04 in headless mode using Wine

* You need to configure your wine to run headless by installing X virtual framebuffer
	* Install the Xvfb: `sudo apt-get install xvfb`
	* Run Xvfb to start the X virtual buffer: `Xvfb :0 -screen 0 1024x768x16 &`
* Enable the 32bit architecture and update the package list: `sudo dpkg --add-architecture i386 && sudo apt update`
* Install Wine: `sudo apt install wine64 wine32`
* Check wine instalattion: `wine --version` => `wine-3.0 (Ubuntu 3.0-1ubuntu1)`
* Install Inno Setup: `DISPLAY=:0.0 wine innosetup-6.1.2.exe /VERYSILENT /NORESTART /ALLUSERS /SUPPRESSMSGBOXES /SP /LOG=setup.log`
* Install Inno Download Plugin 1.5.1 by Mitrich Software: `DISPLAY=:0.0 wine idpsetup-1.5.1.exe /VERYSILENT /NORESTART /ALLUSERS /SUPPRESSMSGBOXES /SP /LOG=setup.log`
