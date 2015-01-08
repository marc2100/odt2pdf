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

#include "buttons.h"

//lokale Funktionen

//True = buttons nicht ausgegraut, False = Buttons ausgegraut
void buttons_ausgrauen(gboolean status);
void interface_ausgrauen (gboolean status);


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
	g_ptr_array_add(unoconv_argv,(gpointer)"unoconv");
	g_ptr_array_add(unoconv_argv,(gpointer)"-f");
	g_ptr_array_add(unoconv_argv,(gpointer)"pdf");
	g_ptr_array_add(unoconv_argv,(gpointer)"-o");
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
	GPid pdftk_pid=0;
	g_ptr_array_add(pdftk_argv,(gpointer)"pdftk");

	//den ListStore durchlaufen lassen, und pfad bauen
	gtk_tree_model_foreach(gtk_tree_view_get_model(gui_get_gtk_tree_viewer()),treemodel_ausgabe_pdftk,(gpointer)pdftk_argv);
	g_ptr_array_add(pdftk_argv,"output");
	//speichert den Pfad
	g_ptr_array_add(pdftk_argv,(gpointer)keyfile_get_pdf_full_path());
	g_ptr_array_add(pdftk_argv,(gpointer)NULL);

	int i=0;
	while (g_ptr_array_index(pdftk_argv,i)){
		printf("Pointer (%d) = %s\n",i,g_ptr_array_index(pdftk_argv,i));
		i++;
	}
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

void 	buttons_ausgrauen(gboolean status){
	gtk_widget_set_sensitive (gui_get_button_refresh(),status);
	gtk_widget_set_sensitive (gui_get_button_exit(),status);
	gtk_widget_set_sensitive (gui_get_button_work(),status);
}

void interface_ausgrauen (gboolean status){
	buttons_ausgrauen(status);
	gtk_widget_set_sensitive(gui_get_gtk_tree_viewer(),status);
}

