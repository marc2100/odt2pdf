/*
Copyright (c) 2014 "Marcus Pries"

This file is part of mysqlviewer.

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
gpointer *thread_refresh_button (gpointer data);
gboolean thread_catcher_refresh (gpointer data);


void button_refresh_clicked(GtkWidget *widget, gpointer data)
{
	//Variablen erzeugen
	struct REFRESH *thread_data = g_malloc(sizeof(struct REFRESH));
	struct CATCHER *catcher_data = g_malloc(sizeof(struct CATCHER));
	GAsyncQueue *warteschlange = NULL;
	gchar *texteingabe = NULL;
	GtkListStore *store = NULL;

	//übergebene Daten in Thread struct verpacken
	thread_data->gui = (struct GUI_ELEMENTE*) data;
	//Asyncrone Queue in die strcut packen
	warteschlange = g_async_queue_new ();
	thread_data->queue = warteschlange;
	catcher_data->queue= warteschlange;

	//Textbuffer auslesen, in Int wandeln und in Thread_daten verpacken
	texteingabe = gtk_entry_get_text(GTK_ENTRY(thread_data->gui->texteingabe));
	thread_data->eingabe = g_ascii_strtod(texteingabe,NULL);
	//2. Textbuffer auslesen
	texteingabe = gtk_entry_get_text(GTK_ENTRY(thread_data->gui->texteingabe_spalten_anzahl));
	thread_data->anzahl_zeilen = g_ascii_strtod (texteingabe,NULL);

	//Wenn anzahl_zeilen = 0, oder eingabe = 0 auf eins setzten, damit einen Zeile ausgegeben wird
	if (thread_data->anzahl_zeilen<=0) thread_data->anzahl_zeilen=1;
	if (thread_data->eingabe<=0) thread_data->eingabe=1;

	//den Button ausgrauen
	g_object_set(widget,"sensitive",FALSE,NULL);

	//Spinner sichbar machen und aktivieren
	g_object_set(gui_get_spinner(),"visible",TRUE,NULL);
	gtk_spinner_start (gui_get_spinner());

	//prüfen ob schon ein Model verbunden ist
	store = (GtkListStore*)gtk_tree_view_get_model (gui_get_gtk_tree_viewer());
	if(store!=NULL)
	{
		//falls schon ein Model verbunden ist,
		//müssen wir die Ref() holen, sonst wird die Liste freigegeben, nach der trennung ;)
		//danach vom viewer trennen
		//und den Inhalt des Stores löschen
		g_object_ref(store);
		gtk_tree_view_set_model (gui_get_gtk_tree_viewer(),NULL);
		gtk_list_store_clear (store);
	}
	else
	{
		// ansonsten einen neuen store erzeugen
		store = gui_get_list_store();
	}
	//store in Thread-variable übergeben
	thread_data->store = store;
	//thread starten
	catcher_data->thread_id=g_thread_new("refresh_thread",(GThreadFunc)thread_refresh_button,(gpointer)thread_data);
	//Die struct des Threads mitgeben, um sie freizugeben
	catcher_data->refresh = thread_data;
	//Timer starten um Thread wieder zu "fangen"
	g_timeout_add (100,thread_catcher_refresh,(gpointer)catcher_data);


	return;
}

void button_exit_clicked(GtkWidget *widget, gpointer data)
{
  gtk_main_quit();
}

gpointer *thread_refresh_button (gpointer data){
	//Variablen anlegen
	struct REFRESH *thread = (struct REFRESH*) data;
	GtkListStore *thread_store = thread->store;
	while(thread->anzahl_zeilen!=0)
	{
		//store mit Daten füllen
		gui_modell_fill_row (thread_store,thread->eingabe);
		thread->eingabe++;
		thread->anzahl_zeilen--;
	}

	g_async_queue_push(thread->queue,GINT_TO_POINTER(1));
	g_thread_exit (thread_store);
	//wird nicht erreicht
	return (gpointer)thread_store;
}

gboolean thread_catcher_refresh (gpointer data){
	GtkListStore *store=NULL;
	struct CATCHER *catcher_data = ((struct CATCHER *)data);
	gint status = 0;
	//check ob thread fertig
	//nötig, da die GUI sonst einfriert
	//g_async_queue_try_pop(catcher_data->queue);
	status = GPOINTER_TO_INT(g_async_queue_try_pop(catcher_data->queue));
	if (!status)
	{
		return TRUE;
	}
	//auf Thread rückgabe einsammeln
	store = (GtkListStore*)g_thread_join (catcher_data->thread_id);

	//den Button wieder freischalten
	g_object_set(gui_get_button_refresh(),"sensitive",TRUE,NULL);
	//Das Model mir dem Viewer verbinden
	gtk_tree_view_set_model (gui_get_gtk_tree_viewer(),(GtkTreeModel*)store);
	//Spinner unsichbar machen und deaktivieren
	g_object_set(gui_get_spinner(),"visible",FALSE,NULL);
	gtk_spinner_stop (gui_get_spinner());
	//store freigeben, er bleibt bestehen, da er mit dem Viewer verknüpft ist
	g_object_unref(store);
	//Speicher freigeben
	//erst den Speicher vom Thread freigeben
	g_free(catcher_data->refresh);
	g_free(catcher_data);
	return FALSE;
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
