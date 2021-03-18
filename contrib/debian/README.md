
Debian
====================
This directory contains files used to package trittiumd/trittium-qt
for Debian-based Linux systems. If you compile trittiumd/trittium-qt yourself, there are some useful files here.

## pivx: URI support ##


trittium-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install trittium-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your trittium-qt binary to `/usr/bin`
and the `../../share/pixmaps/pivx128.png` to `/usr/share/pixmaps`

trittium-qt.protocol (KDE)

