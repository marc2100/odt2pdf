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


///@file buttons.c
///Hier sind alle Funktionen definiert die die Buttons betreffen,
///wie alle G_CALLBACK-Funktionen.

#include "buttons.h"

void button_refresh_clicked(GtkWidget *widget, gpointer data)
{
	GtkListStore *store = gui_model_fill_data ();
	gtk_tree_view_set_model (gui_get_gtk_tree_viewer(),(GtkTreeModel*)store);
	//viewer "sortierbar" machen
	gtk_tree_view_set_reorderable (gui_get_gtk_tree_viewer(),TRUE);

}

void button_exit_clicked(GtkWidget *widget, gpointer data)
{
	gtk_main_quit();
}

void button_work_clicked(GtkWidget *widget, gpointer data){
	GError *error=NULL;
	GPtrArray *unoconv_argv=NULL;
	GPid unoconv_pid=0;

	//prüfen ob überhaupt Daten im Model verfügbar sind
	if (!gtk_tree_model_get_n_columns(gtk_tree_view_get_model(gui_get_gtk_tree_viewer()))){
		return;
	}
	//tmp-Ordner anlegen
	gchar *tmp = g_dir_make_tmp("odt2pdf-gtk_XXXXXX",&error);
	if (error!=NULL){
		g_warning("%s",error->message);
		g_error_free(error);
		error = NULL;
	}
	//temp_verzeichnis verfügbar machen
	temp_dir_save(g_strdup(tmp));
	//DEBUG
	g_print("%s\n",temp_dir_get());

	//Pfad für unoconv erstellen...
	unoconv_argv = g_ptr_array_new ();
	//function zum aufräumen übergeben
	g_ptr_array_set_free_func(unoconv_argv,ptr_array_clean);

	//ptr_array füllen
	fill_g_ptr_array(unoconv_argv,"unoconv");
	fill_g_ptr_array(unoconv_argv,"-f");
	fill_g_ptr_array(unoconv_argv,"pdf");
	fill_g_ptr_array(unoconv_argv,"-o");
	g_ptr_array_add(unoconv_argv,(gpointer)temp_dir_get_strdub());

	//wird für jedes Elemt in der Liststore ausgeführt
	gtk_tree_model_foreach(gtk_tree_view_get_model(gui_get_gtk_tree_viewer()),treemodel_ausgabe_unoconv,(gpointer)unoconv_argv);
	//abschließende NULL anhängen
	g_ptr_array_add(unoconv_argv,(gpointer)NULL);
	//startet den Converter und wartet bis er fertig ist
	//g_print("%s\n",unoconv_cmd->str);

	g_spawn_async_with_pipes 	(NULL,
														(gchar**)unoconv_argv->pdata,
														NULL,
														G_SPAWN_SEARCH_PATH|G_SPAWN_DO_NOT_REAP_CHILD,
														NULL,
														NULL,
														&unoconv_pid,
														NULL,
														NULL,
														NULL,
														&error);
	if (error!=NULL){
		g_warning("%s",error->message);
		g_error_free(error);
		error = NULL;
	}

	//g_child_watch einrichten, um über Programmende informiert zu werden
	g_child_watch_add(unoconv_pid,unoconv_child_watch_func,NULL);

	//strings freigeben
	g_ptr_array_free (unoconv_argv,TRUE);
	g_free(tmp);

	//Buttons ausgrauen
	interface_ausgrauen(FALSE);
}

//Ändert die Hintergrundfarbe der Buttons, wenn mit Maus darüber
void buttons_entered (GtkWidget *widget, gpointer data){
  GdkRGBA color;
  color.red = 27000;
  color.green = 30325;
  color.blue = 30000;
  color.alpha = 15000;
  gtk_widget_override_background_color(widget, GTK_STATE_PRELIGHT, &color);

}

//unoconv_pid_watch wird aufgerufen sobald der unoconv-Prozess beendet ist
void unoconv_child_watch_func (GPid unoconv_pid,gint status,gpointer user_data){
	GError *error = NULL;

	//Status abfragen
	g_spawn_check_exit_status (status,&error);
	if (error!=NULL){
		g_warning("%s",error->message);
		g_error_free(error);
		error = NULL;
	}
	//Pid schließen (ist unter UNIX nicht nötig)
	g_spawn_close_pid(unoconv_pid);

	//pdftk aufruf bauen
	GPtrArray *pdftk_argv= g_ptr_array_new ();
	//Funktion zum aufräumen setzten
	g_ptr_array_set_free_func(pdftk_argv,ptr_array_clean);

	GPid pdftk_pid=0;
	fill_g_ptr_array (pdftk_argv,"pdftk");

	//den ListStore durchlaufen lassen, und pfad bauen
	gtk_tree_model_foreach(gtk_tree_view_get_model(gui_get_gtk_tree_viewer()),treemodel_ausgabe_pdftk,(gpointer)pdftk_argv);
	fill_g_ptr_array (pdftk_argv,"output");
	//speichert den Pfad
	g_ptr_array_add(pdftk_argv,(gpointer)keyfile_get_pdf_full_path());
	g_ptr_array_add(pdftk_argv,(gpointer)NULL);

	//PDF zusammenfügen
	g_spawn_async_with_pipes (NULL,
														(gchar**)pdftk_argv->pdata,
														NULL,
														G_SPAWN_SEARCH_PATH|G_SPAWN_DO_NOT_REAP_CHILD,
														NULL,
														NULL,
														&pdftk_pid,
														NULL,
														NULL,
														NULL,
														&error);
	if (error!=NULL){
		g_warning("%s",error->message);
		g_error_free(error);
		error = NULL;
	}

	//g_child_watch einrichten, um über Programmende(pdftk) informiert zu werden
	g_child_watch_add(pdftk_pid,pdftk_child_watch_func,NULL);
	//aufräumen
	g_ptr_array_free(pdftk_argv,TRUE);
	//g_print("das PDF \"%s\" wurde unter \"%s\" erstellt\n",keyfile_get_pdf_name(),keyfile_get_outputdir());
	//Buttons wieder aktivieren
		interface_ausgrauen(TRUE);
}

//pdftk_pid_watch wird aufgerufen sobald der pdftk-Prozess beendet ist
void pdftk_child_watch_func (GPid pdftk_pid,gint status,gpointer user_data){
	GError *error = NULL;

	//Status abfragen
	g_spawn_check_exit_status (status,&error);
	if (error!=NULL){
		g_warning("%s",error->message);
		g_error_free(error);
		error = NULL;
	}
	//Pid schließen (ist unter UNIX nicht nötig)
	g_spawn_close_pid(pdftk_pid);
	interface_ausgrauen(TRUE);
	temp_dir_delete();
}

void button_open_clicked (GtkWidget *widget, gpointer data){
	//Dialog für Ordner auswahl erstellen
	GtkWidget *dialog;
	dialog = gtk_file_chooser_dialog_new ("Quellordner auswählen",
																				NULL,
																				GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER|GTK_FILE_CHOOSER_ACTION_OPEN,
																				"_Cancel", GTK_RESPONSE_CANCEL,
				      													"_Open", GTK_RESPONSE_ACCEPT,NULL);
	//Dialog starten
	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT){
    gchar *folder;
    folder = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
		keyfile_set_search_dir (folder);
		g_print("Ordner :\n");
		g_print("%s",folder);
		g_print("\ngespeichert...\n");
    g_free (folder);

  }
  else {
  	g_print("Setting abgebrochen\n");
  }

gtk_widget_destroy (dialog);
}

void button_save_as_clicked (GtkWidget *widget, gpointer data){
	//Dialog für Ordner auswahl erstellen
	GtkWidget *dialog;
	dialog = gtk_file_chooser_dialog_new ("Zielverzeichnis wählen",
																				NULL,
																				GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER|GTK_FILE_CHOOSER_ACTION_OPEN,
																				"_Cancel", GTK_RESPONSE_CANCEL,
				      													"_Open", GTK_RESPONSE_ACCEPT,NULL);
	//Dialog starten
	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT){
    gchar *folder;
    folder = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
		keyfile_set_save_as_dir (folder);
		g_print("Ordner :\n");
		g_print("%s",folder);
		g_print("\ngespeichert...\n");
    g_free (folder);

  }
  else {
  	g_print("Setting abgebrochen\n");
  }

gtk_widget_destroy (dialog);
}

void button_about_clicked (GtkWidget *widget, gpointer data){
	GdkPixbuf *logo = NULL;
	GError *error = NULL;
	GtkAboutDialog *about = NULL;
	gchar *authors[] = {"Marcus Pries <email@marcus-pries.de>",NULL};
	gchar *documenters[] = {"Marcus Pries <email@marcus-pries.de>",NULL};
	gchar *about_icon = keyfile_get_about_icon ();

	//dialog erzeugen
	about = gtk_about_dialog_new();
	//pixbuf laden
	logo = gdk_pixbuf_new_from_file (about_icon,&error);
	if (error!=NULL){
		g_error("%s",error->message);
		g_error_free(error);
		error = NULL;
	}
	//pfad freigeben
	g_free(about_icon);
	//Dialog zusammensetzten
	gtk_about_dialog_set_logo(about,logo);
	gtk_about_dialog_set_version (about,odt2pdf_gtk_version);
	gtk_about_dialog_set_copyright(about,"Marcus Pries <email@marcus-pries.de>");
	gtk_about_dialog_set_comments (about,"Mit diesem Programm kann man odt-Dateien sortieren,\nund daraus ein PDF-Dokument erstellen lassen.");
	gtk_about_dialog_set_license (about,"GPL 3.0 or later");
	gtk_about_dialog_set_license_type (about,GTK_LICENSE_GPL_3_0);
	gtk_about_dialog_set_authors (about,&authors);
	gtk_about_dialog_set_documenters(about,&documenters);
	//dialog ausführen
	gtk_dialog_run (GTK_DIALOG (about));
	//Speicher freigeben
	g_object_unref (logo);
  gtk_widget_destroy ((GtkWidget*)about);
}

void button_help_clicked (GtkWidget *widget, gpointer data){
	GError *error=NULL;
	GString *folderpath=NULL;
	folderpath = g_string_new ("file://");
	folderpath = g_string_append (folderpath,g_get_current_dir());
	folderpath = g_string_append (folderpath,"/odt2pdf-anleitung.txt");
	g_print("%s\n",folderpath->str);
	gtk_show_uri (NULL,folderpath->str,GDK_CURRENT_TIME,&error);
	if (error!=NULL){
		g_error("%s",error->message);
		g_error_free(error);
		error = NULL;
	}
	g_string_free(folderpath,TRUE);
}

void button_setup_clicked (GtkWidget *widget, gpointer data){
	//startet das Setupfenster
	setup_run (data);
}
