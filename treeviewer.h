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

#ifndef TREEVIEWER_H
#define TREEVIEWER_H

#include <stdlib.h>
#include <gtk/gtk.h>
#include "treeviewer.h"
#include "buttons.h"
#include "dir_read.h"


struct GUI_ELEMENTE{
	GtkWidget *mainwindow;
	GtkWidget *treeview;
	GtkWidget *button_refresh;
	GtkWidget *button_exit;
	GtkWidget *statusbar;
};

enum {
  COLUMN_ID,
  COLUMN_Pfad,
  N_COLUMNS
};

void gui_init (void);

GtkListStore *gui_model_fill_data (gint anzahl_zeilen);

gboolean gui_modell_fill_row (GtkListStore *store,gint zeile);

GtkProgressBar *gui_get_progressbar (void);

void gui_pulse_progressbar (void);

//erzeugt den store
GtkListStore * gui_get_list_store(void);

//gibt den Refresh_button zurück
GtkWidget *gui_get_button_refresh (void);
//gibt den Viewer zurück
GtkTreeView *gui_get_gtk_tree_viewer (void);



#endif // TREEVIEWER_H
