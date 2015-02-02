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

///@file setup_window.c
///Funktionen für das Setupfenster
#include "setup_window.h"

///Callback für den Speichernbutton
void  setup_save (GtkWidget *widget, gpointer data){
	struct SETUP_WINDOW *setup = (struct SETUP_WINDOW*) data;
	static gint message_id = 0;
	gchar *text_buffer = NULL;

	text_buffer = gtk_file_chooser_get_uri(setup->filechooser_quelle);
	keyfile_set_search_dir(text_buffer+7);
	g_print("Quelle:\t%s\n",text_buffer+7);
	g_free(text_buffer);

	text_buffer = gtk_file_chooser_get_uri(setup->filechooser_ziel);
	keyfile_set_save_as_dir(text_buffer+7);
	g_print("Ziel:\t%s\n",text_buffer+7);
	g_free(text_buffer);

	text_buffer = g_strdup(gtk_entry_get_text(setup->entry_pdf_name));
	keyfile_set_pdf_name(text_buffer);
	g_print("Name:\t%s\n",text_buffer);
	g_free(text_buffer);

	if (message_id != 0){
		gtk_statusbar_remove (setup->statusbar_setup,0,message_id);
	}
	message_id = gtk_statusbar_push(setup->statusbar_setup,0,"Einstellungen wurden gespeichert");
}

///erstellt das Setupfenster und lässt es anzeigen
void setup_run (gpointer data){
	struct SETUP_WINDOW *setup = NULL;
	static GtkEntryBuffer *entry_buffer_pdf_name;
	setup = (struct SETUP_WINDOW*)data;
	gchar *txt_ptr = NULL;

	//Eigenschaften des Setup-Window setzen
	gtk_window_set_title(GTK_WINDOW(setup->setup_window),"odt2pft-gtk Setup");
  gtk_window_set_default_size(GTK_WINDOW(setup->setup_window), 500, 200);
  gtk_window_set_position(GTK_WINDOW(setup->setup_window), GTK_WIN_POS_CENTER);
	//verhindern, das das Fenster gelöscht wird, nur verstecken
	g_signal_connect(setup->setup_window,"delete-event",G_CALLBACK(gtk_widget_hide_on_delete),NULL);

	//Texteintry vorbereiten
	txt_ptr = keyfile_get_pdf_name();
	entry_buffer_pdf_name	= gtk_entry_buffer_new(txt_ptr,-1);
	g_free(txt_ptr);
	gtk_entry_set_buffer(setup->entry_pdf_name,entry_buffer_pdf_name);

	//filechooser_quelle vorbereiten
	txt_ptr = keyfile_get_searchdir();
	gtk_file_chooser_set_action(setup->filechooser_quelle,GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER);
	gtk_file_chooser_set_current_folder(setup->filechooser_quelle,txt_ptr);
	g_free(txt_ptr);
	//filechooser_ziel vorbereiten
	txt_ptr = keyfile_get_outputdir();
	gtk_file_chooser_set_action(setup->filechooser_ziel,GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER);
	gtk_file_chooser_set_current_folder(setup->filechooser_ziel,txt_ptr);
	g_free(txt_ptr);

	//Signale verbinden
	g_signal_connect(setup->setup_save_button,"clicked",G_CALLBACK(setup_save),(gpointer)setup);

	//statusbar resetten
	gtk_statusbar_remove_all (setup->statusbar_setup,0);

	//Fenster anzeigen
	gtk_widget_show (setup->setup_window);

}
