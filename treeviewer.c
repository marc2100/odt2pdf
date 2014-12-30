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

#include "treeviewer.h"
//globale Variablen
struct GUI_ELEMENTE gui_global;
//globaler Eintrag für Spaltennamen
gchar * const string_viewer_header[] = {"ID","Pfad"};
//lokale funtionen
//füllt das Modell mit daten
void fill_modell ();


void gui_init (void){

	//Builder erstellen
	GtkBuilder *builder = NULL;
	builder = gtk_builder_new_from_file("gui.glade");
	struct GUI_ELEMENTE *gui = &gui_global;

	gui->mainwindow 		= GTK_WIDGET (gtk_builder_get_object(builder,"window1"));
	gui->treeview				= GTK_WIDGET (gtk_builder_get_object(builder,"treeview1"));
	gui->button_refresh	= GTK_WIDGET (gtk_builder_get_object(builder,"button_refresh"));
	gui->button_exit		= GTK_WIDGET (gtk_builder_get_object(builder,"button_exit"));
	gui->statusbar			= GTK_WIDGET (gtk_builder_get_object(builder,"statusbar1"));

	//Eigenschaften des Main-Window setzen
	gtk_window_set_title(GTK_WINDOW(gui->mainwindow), "odt2pft-gtkr");
  gtk_window_set_default_size(GTK_WINDOW(gui->mainwindow), 1500, 400);
  gtk_window_set_position(GTK_WINDOW(gui->mainwindow), GTK_WIN_POS_CENTER);


	//viewer "nullen"
	gtk_tree_view_set_model (gui_get_gtk_tree_viewer(),NULL);


	//Signale verbinden
	g_signal_connect(gui->mainwindow,"destroy",G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(gui->button_refresh,"clicked",G_CALLBACK(button_refresh_clicked), (gpointer*)gui);
	g_signal_connect(gui->button_exit,"clicked",G_CALLBACK(button_exit_clicked), NULL);
	//Buttons einfärben, wenn mit Maus darüber
	g_signal_connect(gui->button_refresh,"enter",G_CALLBACK(buttons_entered), NULL);
	g_signal_connect(gui->button_exit,"enter",G_CALLBACK(buttons_entered), NULL);

	//N_COLUMNS-1 Spalten erzeugen
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *spalte;

	gint counter;
	for (counter=0;counter<N_COLUMNS;counter++)
	{

			renderer = gtk_cell_renderer_text_new();
			//eigenschaften des cell_renderers festlegen -> Textumbruch
			g_object_set (renderer,"align-set",TRUE,"width-chars",11,"wrap-width",11,"wrap-mode",PANGO_WRAP_WORD,NULL);
			spalte	 = gtk_tree_view_column_new_with_attributes (string_viewer_header[counter],renderer,
																													"text", counter,NULL);

			gtk_tree_view_column_set_resizable (spalte,TRUE);
			gtk_tree_view_append_column (GTK_TREE_VIEW (gui->treeview), spalte);

	}
  /* Enter the main loop */
  gtk_widget_show (gui->mainwindow);
  gtk_main ();
  return;
  }

GtkListStore *gui_model_fill_data (void){
	//Variablen anlegen
	GtkTreeIter iter;
	gint counter=0;
	GtkListStore *store = NULL;
	//store erstellen um Daten für Tree-viewer zu speichern
	store	= gtk_list_store_new (N_COLUMNS,					//anzahl an Spalten
															G_TYPE_INT,					//id
															G_TYPE_STRING);			//pfad
	ordner_auslesen(store);


	return store;
}




//gibt den Refresh_button zurück
GtkWidget *gui_get_button_refresh (void){
	return gui_global.button_refresh;
}

GtkTreeView *gui_get_gtk_tree_viewer (void){
	return (GtkTreeView*)gui_global.treeview;
}


