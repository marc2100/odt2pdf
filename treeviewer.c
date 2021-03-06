/*
Copyright (c) 2014-2015 "Marcus Pries"

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
///@file treeviewer.c
///Hier sind alle Funktionen definiert die das eigentliche Fenster
///betreffen, so wird auch die eigentliche GUI hier
///initialisiert, sowie der Treeviewer bearbeitet.

#include "treeviewer.h"

///@struct GUI_ELEMENTE
///beinhaltet alle wichtigen GUI Elemente
struct GUI_ELEMENTE{
	GtkWidget *mainwindow;			///<das Main-Window
	GtkWidget *treeview;				///<den Treeviewer
	GtkWidget *button_refresh;	///<den Refresh-Button
	GtkWidget *button_exit;			///<den Exit-Button
	GtkWidget *button_work;			///<den Ausführen-Button
	GtkWidget *statusbar;				///<die Statusbar
	GtkWidget *setup_window;		///<Setup-Window
};

//globale Variablen

///@brief lokale Variable um GUI-Elemente zu speichern
struct GUI_ELEMENTE gui_global;

///@brief globaler Eintrag für Spaltennamen
gchar * const string_viewer_header[] = {"ID","Datei"};


///initiiert die GUI, und startet damit das eigentliche Programm
void gui_init (void){

	//Builder erstellen
	GtkBuilder *builder = NULL;
	builder = gtk_builder_new_from_file("odt2pdf-gtk.glade");
	struct GUI_ELEMENTE *gui = &gui_global;
	struct SETUP_WINDOW *setup = (struct SETUP_WINDOW*) g_malloc(sizeof(struct SETUP_WINDOW));

	gui->mainwindow 		= GTK_WIDGET (gtk_builder_get_object(builder,"window1"));
	gui->treeview				= GTK_WIDGET (gtk_builder_get_object(builder,"treeview1"));
	gui->button_refresh	= GTK_WIDGET (gtk_builder_get_object(builder,"button_refresh"));
	gui->button_exit		= GTK_WIDGET (gtk_builder_get_object(builder,"button_exit"));
	gui->statusbar			= GTK_WIDGET (gtk_builder_get_object(builder,"statusbar1"));
  gui->button_work		= GTK_WIDGET (gtk_builder_get_object(builder,"button_work"));

	//Elemente des Setupwindow
	setup->setup_window				= GTK_WIDGET (gtk_builder_get_object(builder,"window2"));
	setup->statusbar_setup		= GTK_WIDGET (gtk_builder_get_object(builder,"statusbar_setup"));
	setup->entry_pdf_name			= GTK_WIDGET (gtk_builder_get_object(builder,"entry_pdf_name"));
	setup->filechooser_quelle	= GTK_WIDGET (gtk_builder_get_object(builder,"filechooserbutton_quelle"));
	setup->filechooser_ziel		= GTK_WIDGET (gtk_builder_get_object(builder,"filechooserbutton_ziel"));
	setup->setup_save_button 	= GTK_WIDGET (gtk_builder_get_object(builder,"setup-save-button"));

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
	g_signal_connect(gtk_builder_get_object(builder,"gtk-exit-menu"),"activate",G_CALLBACK(button_exit_clicked), NULL);
	g_signal_connect(gtk_builder_get_object(builder,"gtk-open-menu"),"activate",G_CALLBACK(button_open_clicked), NULL);
	g_signal_connect(gtk_builder_get_object(builder,"gtk-save-as-menu"),"activate",G_CALLBACK(button_save_as_clicked), NULL);
	g_signal_connect(gtk_builder_get_object(builder,"gtk-save-menu"),"activate",G_CALLBACK(button_work_clicked), NULL);
	g_signal_connect(gtk_builder_get_object(builder,"gtk-about-menu"),"activate",G_CALLBACK(button_about_clicked), NULL);
	g_signal_connect(gtk_builder_get_object(builder,"gtk-help-menu"),"activate",G_CALLBACK(button_help_clicked), NULL);
	g_signal_connect(gtk_builder_get_object(builder,"gtk-setup-menu"),"activate",G_CALLBACK(button_setup_clicked), (gpointer)setup);

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

///erzeugt einen GtkListStore und füllt ihn mit Daten
///@return GtkListStore *
GtkListStore *gui_model_fill_data (void){
	//Variablen anlegen
	GtkListStore *store = NULL;
	//store erstellen um Daten für Tree-viewer zu speichern
	store	= gtk_list_store_new (N_COLUMNS,			//anzahl an Spalten
															G_TYPE_INT,			//Datei
															G_TYPE_STRING);	//Pfad
	ordner_auslesen(store);

	return store;
}




///Gibt einen pointer auf den button_refresh zurück
///@return GtkWidget *button_refresh
GtkWidget *gui_get_button_refresh (void){
	return gui_global.button_refresh;
}

///Gibt einen pointer auf den button_exit zurück
///@return GtkWidget *button_exit
GtkWidget *gui_get_button_exit (void){
	return gui_global.button_exit;
}

///Gibt einen pointer auf den button_work zurück
///@return GtkWidget *button_work
GtkWidget *gui_get_button_work (void){
	return gui_global.button_work;
}

///Gibt einen pointer auf den Viewer zurück
///@return GtkWidget *treeviewer
GtkTreeView *gui_get_gtk_tree_viewer (void){
	return (GtkTreeView*)gui_global.treeview;
}

///wird bei der ausgabe der sortierten Anzeige, für jedes Element aufgerufen, um den cmd für unoconv zu bauen
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

///wird bei der ausgabe der sortierten Anzeige, für jedes Element aufgerufen, um den cmd für pdftk zu bauen
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
