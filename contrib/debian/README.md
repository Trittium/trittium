
Debian
====================
This directory contains files used to package jiyod/jiyo-qt
for Debian-based Linux systems. If you compile jiyod/jiyo-qt yourself, there are some useful files here.

## jiyo: URI support ##


jiyo-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install jiyo-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your jiyoqt binary to `/usr/bin`
and the `../../share/pixmaps/jiyo128.png` to `/usr/share/pixmaps`

jiyo-qt.protocol (KDE)

