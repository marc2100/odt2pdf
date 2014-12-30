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
gchar * const string_viewer_header[] = {"ID","Index","Name","Rasse","Farbe","Geschlecht","Vater","Mutter","Stockmaß","Nachkommen",
																"Verwendung","verkäuflich","Herkunftsland","Marke","Bild","Geburtstag"};
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
	gui->texteingabe		= GTK_WIDGET (gtk_builder_get_object(builder,"entry1"));
	gui->button_refresh	= GTK_WIDGET (gtk_builder_get_object(builder,"button_refresh"));
	gui->button_exit		= GTK_WIDGET (gtk_builder_get_object(builder,"button_exit"));
	gui->statusbar		= GTK_WIDGET (gtk_builder_get_object(builder,"statusbar1"));
	gui->texteingabe_spalten_anzahl= GTK_WIDGET (gtk_builder_get_object(builder,"entry2"));
	gui->spinner				= GTK_WIDGET (gtk_builder_get_object(builder,"spinner1"));

	//Eigenschaften des Main-Window setzen
	gtk_window_set_title(GTK_WINDOW(gui->mainwindow), "MySQL-Viewer");
  gtk_window_set_default_size(GTK_WINDOW(gui->mainwindow), 1500, 400);
  gtk_window_set_position(GTK_WINDOW(gui->mainwindow), GTK_WIN_POS_CENTER);


	//GtkSpinner unsichtbar machen
	g_object_set(gui->spinner,"visible",FALSE,NULL);

	//viewer "nullen"
	gtk_tree_view_set_model (gui_get_gtk_tree_viewer(),NULL);

	//Textfeld einen Buffer zuweisen
	GtkEntryBuffer *buffer = NULL;
	buffer = gtk_entry_buffer_new("1",-1);
	gtk_entry_set_buffer (GTK_ENTRY(gui->texteingabe),buffer);


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

		//Wenn Bild erreicht, dann mit dem pixbuf_renderer laden
		if (counter==COLUMN_Bild)
		{
			renderer = gtk_cell_renderer_pixbuf_new();
			spalte	 = gtk_tree_view_column_new_with_attributes (string_viewer_header[counter],renderer,
																													"pixbuf", counter,NULL);
			gtk_tree_view_column_set_resizable (spalte,TRUE);
			gtk_tree_view_append_column (GTK_TREE_VIEW (gui->treeview), spalte);
		}
		//Wenn verkäuflich erreicht, dann mit checkbox laden
		else if (counter==COLUMN_verkaeuflich)
		{
			renderer = gtk_cell_renderer_toggle_new ();
			spalte	 = gtk_tree_view_column_new_with_attributes (string_viewer_header[counter],renderer,
																													"active", counter,NULL);
			gtk_tree_view_column_set_resizable (spalte,TRUE);
			gtk_tree_view_append_column (GTK_TREE_VIEW (gui->treeview), spalte);
		}

		//alle anderen Daten als "text" laden
		else
		{
			renderer = gtk_cell_renderer_text_new();
			//eigenschaften des cell_renderers festlegen -> Textumbruch
			g_object_set (renderer,"align-set",TRUE,"width-chars",11,"wrap-width",11,"wrap-mode",PANGO_WRAP_WORD,NULL);
			spalte	 = gtk_tree_view_column_new_with_attributes (string_viewer_header[counter],renderer,
																													"text", counter,NULL);

			gtk_tree_view_column_set_resizable (spalte,TRUE);
			gtk_tree_view_append_column (GTK_TREE_VIEW (gui->treeview), spalte);
		}
	}
  /* Enter the main loop */
  gtk_widget_show (gui->mainwindow);
  gtk_main ();
  return;
  }

GtkListStore *gui_model_fill_data (gint anzahl_zeilen){
	//Variablen anlegen
	GtkTreeIter iter;
	gint counter=0;
	GtkListStore *store = NULL;
	//store erstellen um Daten für Tree-viewer zu speichern
	store	= gtk_list_store_new (N_COLUMNS,					//anzahl an Spalten
															G_TYPE_STRING,			//id
															G_TYPE_STRING,			//Index
															G_TYPE_STRING,			//Name
															G_TYPE_STRING,			//Rasse
															G_TYPE_STRING,			//Farbe
															G_TYPE_STRING,			//Geschlecht
															G_TYPE_STRING,			//Vater
															G_TYPE_STRING,			//Mutter
															G_TYPE_STRING,			//Stockmass
															G_TYPE_STRING,			//Nachkommen
															G_TYPE_STRING,			//Verwendung
															G_TYPE_BOOLEAN,			//verkäuflich
															G_TYPE_STRING,			//Herkunftsland
															G_TYPE_STRING,			//Marke
															GDK_TYPE_PIXBUF,		//Bild
															G_TYPE_STRING);			//Geburtstag


	struct DB_STRUCT *db_data = NULL;
	gint schleife=1;
	/*
			do{

		//DB auslesen und Daten eintragen
		db_data = read_row_mysql(schleife);
		schleife++;
		/* Append an empty row to the list store. Iter will point to the new row
		gtk_list_store_append (store, &iter);

		 füllt die hinzugefügte Reihe mit Daten
		for (counter=0;counter<=(db_data->num_fields-1);counter++)
		{

			//Bild als Pixbuf speichern
			if (counter==COLUMN_Bild)
			{
				gtk_list_store_set (store,&iter,counter,(GdkPixbuf*)g_ptr_array_index(db_data->ptr_array,counter),-1);
			}
			else if (counter==COLUMN_verkaeuflich)
			{
				//string in integer umwandeln
				guint g=0;
				g = (guint)g_ascii_strtod ((gchar*)g_ptr_array_index(db_data->ptr_array,counter),NULL);
				//und in store speichern
				gtk_list_store_set (store,&iter,counter,g,-1);
				//abschließend den Speicher freigeben
				g_free(g_ptr_array_index(db_data->ptr_array,counter));
			}
			//rest als "text" speichern
			else
			{
				gtk_list_store_set (store,&iter,counter,(gchar*)g_ptr_array_index(db_data->ptr_array,counter),-1);
				//string freigeben
				g_free(g_ptr_array_index(db_data->ptr_array,counter));
			}


		}

		//ptr-arry freigeben
		g_ptr_array_free (db_data->ptr_array,TRUE);
		//db_daten freigeben
		g_free(db_data);

		//runterzählen, bei 0 hört die Schleife auf
		anzahl_zeilen--;
	}while (anzahl_zeilen);
	return store
	*/
	return NULL;
}



gboolean gui_modell_fill_row (GtkListStore *store,gint zeile){
	/*
	if (store==NULL||zeile==0) return FALSE;
	//Variablen anlegen
	struct DB_STRUCT *db_data = NULL;

	//DB auslesen und Daten eintragen
	db_data = read_row_mysql(zeile);
	//prüfen on Eintrag vorhanden
	//Falls ja Modell mit Daten füllen,
	//und strcut db_daten freigeben
	if (db_data!=NULL)
	{
		fill_modell (store,db_data);
		g_ptr_array_free (db_data->ptr_array,TRUE);
		g_free (db_data);
		return TRUE;
	}
	*/
	//ansonsten direkt zurückkehren
	return FALSE;

}


void fill_modell (GtkListStore *store,struct DB_STRUCT *db_data){

	return TRUE;
}

//erzeugt den store
GtkListStore * gui_get_list_store(void){
	GtkListStore *store = NULL;
	//store erstellen um Daten für Tree-viewer zu speichern
	store	= gtk_list_store_new (N_COLUMNS,					//anzahl an Spalten
															G_TYPE_STRING,			//id
															G_TYPE_STRING,			//Index
															G_TYPE_STRING,			//Name
															G_TYPE_STRING,			//Rasse
															G_TYPE_STRING,			//Farbe
															G_TYPE_STRING,			//Geschlecht
															G_TYPE_STRING,			//Vater
															G_TYPE_STRING,			//Mutter
															G_TYPE_STRING,			//Stockmass
															G_TYPE_STRING,			//Nachkommen
															G_TYPE_STRING,			//Verwendung
															G_TYPE_BOOLEAN,			//verkäuflich
															G_TYPE_STRING,			//Herkunftsland
															G_TYPE_STRING,			//Marke
															GDK_TYPE_PIXBUF,		//Bild
															G_TYPE_STRING);			//Geburtstag
	return store;
}

//gibt den Refresh_button zurück
GtkWidget *gui_get_button_refresh (void){
	return gui_global.button_refresh;
}

GtkTreeView *gui_get_gtk_tree_viewer (void){
	return (GtkTreeView*)gui_global.treeview;
}

GtkSpinner *gui_get_spinner (void){
	return (GtkSpinner*)gui_global.spinner;
}
