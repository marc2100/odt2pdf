/*
Copyright (c) 2014 "Marcus Pries"

This file is part of odt2pdf-gtk.

mysqlviewer is free software: you can redistribute it and/or modify
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

///@file treeviewer.h
///Hier sind alle Funktionen deklariert die global verfügbar sein müssen zu finden.
///Unter anderem die wichte struct GUI_ELEMTE @see GUI_ELEMENTE.


#ifndef TREEVIEWER_H
#define TREEVIEWER_H

#include <stdlib.h>
#include <gtk/gtk.h>
#include "treeviewer.h"
#include "buttons.h"
#include "dir_read.h"

///@struct GUI_ELEMENTE
///beinhaltet alle wichtigen GUI Elemente
struct GUI_ELEMENTE{
	GtkWidget *mainwindow;			///<das Main-Window
	GtkWidget *treeview;				///<den Treeviewer
	GtkWidget *button_refresh;	///<den Refresh-Button
	GtkWidget *button_exit;			///<den Exit-Button
	GtkWidget *button_work;			///<den Ausführen-Button
	GtkWidget *statusbar;				///<die Statusbar
};

enum {
  COLUMN_ID,
  COLUMN_Pfad,
  N_COLUMNS
};

///initiiert die GUI, und startet damit das eigentliche Programm
void gui_init (void);

///erzeugt einen GtkListStore und füllt ihn mit Daten
///@return GtkListStore *
GtkListStore *gui_model_fill_data (void);

///Gibt einen pointer auf den button_refresh zurück
///@return GtkWidget *button_refresh
GtkWidget *gui_get_button_refresh (void);

///Gibt einen pointer auf den button_exit zurück
///@return GtkWidget *button_exit
GtkWidget *gui_get_button_exit (void);

///Gibt einen pointer auf den button_work zurück
///@return GtkWidget *button_work
GtkWidget *gui_get_button_work (void);

///Gibt einen pointer auf den Viewer zurück
///@return GtkWidget *treeviewer
GtkTreeView *gui_get_gtk_tree_viewer (void);

///wird bei der ausgabe der sortierten Anzeige, für jedes Element aufgerufen, um den cmd für unoconv zu bauen
gboolean treemodel_ausgabe_unoconv (GtkTreeModel *model,GtkTreePath *path,GtkTreeIter *iter,gpointer data);

///wird bei der ausgabe der sortierten Anzeige, für jedes Element aufgerufen, um den cmd für pdftk zu bauen
gboolean treemodel_ausgabe_pdftk (GtkTreeModel *model,GtkTreePath *path,GtkTreeIter *iter,gpointer data);

#endif // TREEVIEWER_H
