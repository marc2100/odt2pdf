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
	temp_dir_save(g_strdup(tmp));

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
	GString *pdftk_cmd = NULL;
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
	//GPtrArray *pdftk_argv=NULL;
	pdftk_cmd = g_string_new("pdftk");
	g_string_append(pdftk_cmd," ");


	//den ListStore durchlaufen lassen, und pfad bauen
	gtk_tree_model_foreach(gtk_tree_view_get_model(gui_get_gtk_tree_viewer()),treemodel_ausgabe_pdftk,(gpointer)pdftk_cmd);
	g_string_append(pdftk_cmd," output ");
	g_string_append(pdftk_cmd,keyfile_get_outputdir());
	g_string_append(pdftk_cmd,"/");
	g_string_append(pdftk_cmd,keyfile_get_pdf_name());

	if(!g_spawn_command_line_sync(pdftk_cmd->str,NULL,NULL,NULL,&error) ){
		g_warning("%s",error->message);
		g_error_free(error);
		error = NULL;
	}

	//aufräumen des temp-verzeichnis
	GString *rm_cmd = g_string_new("rm -R ");
	g_string_append(rm_cmd,temp_dir_get());
	if(!g_spawn_command_line_sync(rm_cmd->str,NULL,NULL,NULL,&error) ){
		g_warning("%s",error->message);
		g_error_free(error);
		error = NULL;
	}


	g_string_free(rm_cmd,TRUE);
	//ist der gleiche der auch in struct unconv steckte
	temp_dir_clear();


	g_print("das PDF \"%s\" wurde unter \"%s\" erstellt\n",keyfile_get_pdf_name(),keyfile_get_outputdir());
	//Buttons wieder aktivieren
		interface_ausgrauen(TRUE);
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
