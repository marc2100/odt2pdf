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
gchar * const string_viewer_header[] = {"ID","Datei"};

//lokale funtionen


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
  gui->button_work		= GTK_WIDGET (gtk_builder_get_object(builder,"button_work"));

	//Eigenschaften des Main-Window setzen
	gtk_window_set_title(GTK_WINDOW(gui->mainwindow), "odt2pft-gtk");
  gtk_window_set_default_size(GTK_WINDOW(gui->mainwindow), 1500, 400);
  gtk_window_set_position(GTK_WINDOW(gui->mainwindow), GTK_WIN_POS_CENTER);


	//viewer "nullen"
	gtk_tree_view_set_model (gui_get_gtk_tree_viewer(),NULL);
	//viewer "sortierbar" machen
	gtk_tree_view_set_reorderable (gui_get_gtk_tree_viewer(),TRUE);
	gtk_tree_view_set_headers_clickable (gui_get_gtk_tree_viewer(),TRUE);

	//Signale verbinden
	g_signal_connect(gui->mainwindow,"destroy",G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(gui->button_refresh,"clicked",G_CALLBACK(button_refresh_clicked), NULL);
	g_signal_connect(gui->button_exit,"clicked",G_CALLBACK(button_exit_clicked), NULL);
	g_signal_connect(gui->button_work,"clicked",G_CALLBACK(button_work_clicked), NULL);
	//Menüsignale verknüpfen
	g_signal_connect(gtk_builder_get_object(builder,"imagemenuitem5"),"activate",G_CALLBACK(button_exit_clicked), NULL);

	//Buttons einfärben, wenn mit Maus darüber
	g_signal_connect(gui->button_refresh,"enter",G_CALLBACK(buttons_entered), NULL);
	g_signal_connect(gui->button_exit,"enter",G_CALLBACK(buttons_entered), NULL);
	g_signal_connect(gui->button_work,"enter",G_CALLBACK(buttons_entered), NULL);

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
	//TreeViewer in Drag-and-Drop aufnehmen
	gtk_tree_view_enable_model_drag_source(gui_get_gtk_tree_viewer(),
																					0,NULL,0,0);

  /* Enter the main loop */
  gtk_widget_show (gui->mainwindow);
  gtk_main ();
  return;
  }

GtkListStore *gui_model_fill_data (void){
	//Variablen anlegen
	GtkListStore *store = NULL;
	//store erstellen um Daten für Tree-viewer zu speichern
	store	= gtk_list_store_new (N_COLUMNS,					//anzahl an Spalten
															G_TYPE_INT,			//Datei
															G_TYPE_STRING);			//Pfad
	ordner_auslesen(store);


	return store;
}




//gibt den Refresh_button zurück
GtkWidget *gui_get_button_refresh (void){
	return gui_global.button_refresh;
}

GtkWidget *gui_get_button_exit (void){
	return gui_global.button_exit;
}

GtkWidget *gui_get_button_work (void){
	return gui_global.button_work;
}

GtkTreeView *gui_get_gtk_tree_viewer (void){
	return (GtkTreeView*)gui_global.treeview;
}

//wird bei der ausgabe der sortierten Anzeige, für jedes Element aufgerufen
gboolean treemodel_ausgabe_unoconv (GtkTreeModel *model,GtkTreePath *path,GtkTreeIter *iter,gpointer data){
	GPtrArray *ptr_array = (GPtrArray *)data;
	gchar *text_data,*ptr;
	GString *pfad = g_string_new(NULL);
	//Dateiname aus Liststore auslesen
	gtk_tree_model_get(model,iter,COLUMN_Pfad,&text_data,-1);

	//Datei mit absolutem Pfad an ptr_arry hängen
	ptr = keyfile_get_searchdir();
	pfad = (gpointer)g_string_append (pfad,ptr);
	g_free(ptr);
	pfad = (gpointer)g_string_append (pfad,"/");
	pfad = (gpointer)g_string_append (pfad,text_data);

	//Pfad als String ins array schreiben
	g_ptr_array_add(ptr_array,(gpointer)g_strdup(pfad->str));

	//string freigeben
	g_string_free(pfad,TRUE);
	return FALSE;
}

//wird bei der ausgabe der sortierten Anzeige, für jedes Element aufgerufen
gboolean treemodel_ausgabe_pdftk (GtkTreeModel *model,GtkTreePath *path,GtkTreeIter *iter,gpointer data){
	gchar *text_data;
	GPtrArray *pdftk_ptr = (GPtrArray *) data;
	GString *pdftk = g_string_new(NULL);
	//Dateiname aus Liststore auslesen
	gtk_tree_model_get(model,iter,COLUMN_Pfad,&text_data,-1);

	//Datei mit absolutem Pfad an den command anhängen
	pdftk = g_string_append (pdftk,temp_dir_get());
	pdftk = g_string_append (pdftk,"/");
	pdftk = g_string_append (pdftk,text_data);
	//das Datenende anpassen, es sind ja mittlerweile pdf daten
	g_string_overwrite(pdftk,(pdftk->len-4),".pdf");
	//datei in argv speichern, und gstring auflösen
	g_ptr_array_add(pdftk_ptr,(gpointer)g_string_free(pdftk,FALSE));

	return FALSE;
}
