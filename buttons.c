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
//lokale struct
struct REFRESH{
struct GUI_ELEMENTE *gui;
gint eingabe;
gint anzahl_zeilen;
GtkListStore *store;
GAsyncQueue *queue;

};

struct CATCHER{
GThread *thread_id;
GAsyncQueue *queue;
struct REFRESH *refresh;
};

//lokale Variablen

//lokale funktionen

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
	GString *unoconv_cmd = g_string_new("unoconv -f pdf -o /home/marc2100/temp/odt2pdf/test_dir ");
	gchar *standard_output=NULL;
	gchar *standard_error=NULL;
	GError *error=NULL;
	//tmp-Ordner anlegen
	gchar *tmp = g_dir_make_tmp("odt2pdf-gtk_XXXXXX",&error);
	if (error!=NULL){
		g_warning("%s",error->message);
		g_error_free(error);
		error = NULL;
	}
	printf("Das erstellte Verzeichniss ist: %s\n",tmp);
	//wird für jedes Elemt in der Liststore ausgeführt
	gtk_tree_model_foreach(gtk_tree_view_get_model(gui_get_gtk_tree_viewer()),treemodel_ausgabe,(gpointer)unoconv_cmd);
	//startet den Converter und wartet bis er fertig ist
	g_print("%s\n",unoconv_cmd->str);
	if (!g_spawn_command_line_sync(unoconv_cmd->str,&standard_output,&standard_error,NULL,&error) ){
		g_warning("%s",error->message);
		g_error_free(error);
		error = NULL;
	}else{
		g_print("------------------------------------------ \n");
		g_print("Output: %s\n",standard_output);
		g_print("Error : %s\n",standard_error);
		g_print("------------------------------------------ \n");
	}
	//string freigeben
	g_string_free(unoconv_cmd,TRUE);
	//tmp-Verzeichnis wieder löschen
	g_rmdir (tmp);
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
