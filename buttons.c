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
	struct PDFTK_DATA pdftk;
	struct UNOCONV_DATA *unoconv = g_malloc(sizeof(struct UNOCONV_DATA));

	gint unoconv_output=0, unoconv_error=0;
	GError *error=NULL;
	GPtrArray *unoconv_argv=NULL;

	//tmp-Ordner anlegen
	gchar *tmp = g_dir_make_tmp("odt2pdf-gtk_XXXXXX",&error);
	if (error!=NULL){
		g_warning("%s",error->message);
		g_error_free(error);
		error = NULL;
	}
	printf("Das erstellte Verzeichniss ist: %s\n",tmp);
	//Pfad für unoconv erstellen...
	unoconv_argv = g_ptr_array_new ();
	g_ptr_array_add(unoconv_argv,(gpointer)"unoconv");
	g_ptr_array_add(unoconv_argv,(gpointer)"-f");
	g_ptr_array_add(unoconv_argv,(gpointer)"pdf");
	g_ptr_array_add(unoconv_argv,(gpointer)"-o");
	g_ptr_array_add(unoconv_argv,(gpointer)g_strdup(tmp));

	//wird für jedes Elemt in der Liststore ausgeführt
	gtk_tree_model_foreach(gtk_tree_view_get_model(gui_get_gtk_tree_viewer()),treemodel_ausgabe_unoconv,(gpointer)unoconv_argv);
	//abschließende NULL anhängen
	g_ptr_array_add(unoconv_argv,(gpointer)NULL);
	//startet den Converter und wartet bis er fertig ist
	//g_print("%s\n",unoconv_cmd->str);
	gboolean status = FALSE;
	status =	g_spawn_async_with_pipes (NULL,
																			(gchar**)unoconv_argv->pdata,
																			NULL,
																			G_SPAWN_SEARCH_PATH|G_SPAWN_DO_NOT_REAP_CHILD,
																			NULL,
																			NULL,
																			&unoconv->unoconv_pid,
																			NULL,
																			&unoconv_output,
																			&unoconv_error,
																			&error);
	if (error!=NULL){
		g_warning("%s",error->message);
		g_error_free(error);
		error = NULL;
	}else{
		g_print("Unoconv gestartet mit PID:%d\n",unoconv->unoconv_pid);
	}
	//den Output/Error von Unoconv abfragen
	unoconv->unoconv_output = g_io_channel_unix_new(unoconv_output);
	unoconv->unoconv_error 	= g_io_channel_unix_new(unoconv_error);

	//g_child_watch einrichten, um über Programmende informiert zu werden
	g_child_watch_add(unoconv->unoconv_pid,
										unoconv_child_watch_func,
										(gpointer)unoconv);


	//pdftk aufruf bauen
	pdftk.pdftk_cmd = g_string_new("pdftk");


	g_string_append(pdftk.pdftk_cmd," ");
	//Tmp-Verzeichnis in struct kopieren, wird für den cmd-bau gebraucht
	pdftk.tmp = g_strdup(tmp);
	//den ListStore durchlaufen lassen, und pfad bauen
	gtk_tree_model_foreach(gtk_tree_view_get_model(gui_get_gtk_tree_viewer()),treemodel_ausgabe_pdftk,(gpointer)&pdftk);
	g_string_append(pdftk.pdftk_cmd," output ");
	g_string_append(pdftk.pdftk_cmd,keyfile_get_outputdir());
	g_string_append(pdftk.pdftk_cmd,"/");
	g_string_append(pdftk.pdftk_cmd,keyfile_get_pdf_name());
	/*
	if(!g_spawn_command_line_sync(pdftk.pdftk_cmd->str,&standard_output,&standard_error,NULL,&error) ){
		g_warning("%s",error->message);
		g_error_free(error);
		error = NULL;
	}else{
		g_print("------------------------------------------ \n");
		g_print("Output: %s\n",standard_output);
		g_print("Error : %s\n",standard_error);
		g_print("------------------------------------------ \n");
	}
	*/

	//Verzeichnis löschen
	//g_rmdir (tmp);
	//strings freigeben
	g_ptr_array_free (unoconv_argv,TRUE);
	g_free(tmp);
	g_free(pdftk.tmp);
	g_string_free(pdftk.pdftk_cmd,TRUE);
	//g_string_free(unoconv_cmd,TRUE);

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

//unoconv_pid_watch
void unoconv_child_watch_func (GPid unoconv_pid,gint status,gpointer user_data){
	struct UNOCONV_DATA *unoconv = (struct UNOCONV_DATA*) user_data;
	GError *error = NULL;

		GString *out = g_string_new(NULL);
	GString *err = g_string_new(NULL);

	g_io_channel_read_line_string (unoconv->unoconv_output,out,NULL,&error);
	if (error!=NULL){
		g_warning("%s",error->message);
		g_error_free(error);
		error = NULL;
	}
	g_io_channel_read_line_string (unoconv->unoconv_error,err,NULL,&error);
	if (error!=NULL){
		g_warning("%s",error->message);
		g_error_free(error);
		error = NULL;
	}



	//Status abfragen
	g_spawn_check_exit_status (status,&error);
	if (error!=NULL){
		g_warning("%s",error->message);
		g_error_free(error);
		error = NULL;
		g_print("ERROR:\t%s\n",err->str);
	}else{
		g_print("Unoconv mit der Pid: %d ist beendet, mit dem Status: %d\n",unoconv->unoconv_pid,status);
		g_print("OUTPUT:\t%s\n",out->str);
	}
	//Pid schließen (ist unter UNIX nicht nötig)
	g_spawn_close_pid(unoconv->unoconv_pid);

}
