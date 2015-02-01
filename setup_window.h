/*
Copyright (c) 2014 "Marcus Pries"

This file is part of odt2pdf-gtk.

odt2pdf-gtk is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

///@file setup_window.h
///Funktionen für das Setupfenster
#ifndef SETUP_WINDOW_H
#define SETUP_WINDOW_H

#include <stdlib.h>
#include <gtk/gtk.h>
#include "treeviewer.h"
#include "buttons.h"
#include "dir_read.h"


///@struct SETUP_WINDOW
///beinhaltet alle GUI Elemente des Setup-Fensters
struct SETUP_WINDOW{
	GtkWidget *setup_window;			///<window
	GtkWidget *statusbar_setup;		///<statusbar
	GtkWidget *entry_pdf_name;		///<ausgabename für Datei
	GtkWidget *filechooser_quelle;///<filechooser für quelle
	GtkWidget *filechooser_ziel;	///<filechoosser für ziel
	GtkWidget *setup_save_button;	///<speichern Button
};

///Callback für den Speichernbutton
void setup_save (GtkWidget *widget, gpointer data);

///erstellt das Setupfenster und lässt es anzeigen
void setup_run (gpointer data);

#endif // SETUP_WINDOW_H
