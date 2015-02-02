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
///@file dir_read.c
///Hier sind alle Funktionen definiert um den Quellordner,
///mit den odt-Dateien auszulesen.

#include "dir_read.h"

//Lokale Funktionen
///sortiert alle Einträge aus, bis auf *.odf-dateien
///@return dateipfad
gchar* dateifilter (gchar const *pfad);

///gibt die Einträge aus
void ausgabe (gint *id,gchar const *pfad,GtkListStore *list_store);


///ließt die Einträge aus dem in der Keyfile
///angegebenen Ordner aus
void ordner_auslesen (GtkListStore* store){
	GDir 		*ordner = NULL;
	GError 	*error = NULL;
	gchar 	*ordner_eintrag = NULL;
	gchar 	*ordner_pfad = keyfile_get_searchdir();
	gint id_counter = 0;

	//Ordner öffnen
	ordner = g_dir_open(ordner_pfad,0,&error);
	g_free(ordner_pfad);
	//auf Fehler prüfen
	if (error!=NULL){
		g_error("%s",error->message);
		g_error_free(error);
		error = NULL;
	}
	//ersten Eintrag lesen
	ordner_eintrag = (gchar*)g_dir_read_name(ordner);
	while (ordner_eintrag){

	//Einträge filtern
	ordner_eintrag = dateifilter (ordner_eintrag);
	//Einträge ausgeben
	ausgabe(&id_counter,ordner_eintrag,store);
	//Schleife solange wiederholen, bis keine Einträge mehr vorhanden (=NULL)
	ordner_eintrag = (gchar*)g_dir_read_name(ordner);
	}
	//Ordner wieder schließen
	g_dir_close(ordner);
	return;
}


///sortiert alle Einträge aus, bis auf *.odf-dateien
gchar* dateifilter (gchar const *pfad)
{
	if (g_str_has_suffix(pfad,".odt"))
		return (gchar*)pfad;
	else
		return NULL;
}

///gibt die Einträge aus
void ausgabe (gint *id,gchar const *pfad,GtkListStore *list_store){
	GtkTreeIter iter;
	//Wenn pfad = NULL dann verwerfen
	if (!pfad){
		return;
	//ansonsten Daten speichern
	}else{
		*id+=1;
		gtk_list_store_append (list_store, &iter);
		gtk_list_store_set (list_store, &iter,
												COLUMN_ID, *id,
												COLUMN_Pfad, pfad,
												-1);
	}

}
