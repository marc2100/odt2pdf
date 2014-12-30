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
#include "dir_read.h"

//Lokale Funktionen
///sortiert alle Einträge aus, bis auf *.odf-dateien
gchar* dateifilter (gchar const *pfad);
///gibt die Einträge aus
void ausgabe (gchar const *pfad);

///ließt die Einträge aus dem in der Keyfile
///angegebenen Ordner aus
void ordner_auslesen (void){
	GDir 		*ordner = NULL;
	GError 	*error = NULL;
	gchar 	*ordner_eintrag = NULL;
	gchar *dir_path = keyfile_get_searchdir();

	//Ordner öffnen
	ordner = g_dir_open(dir_path,0,error);
	g_free(dir_path);
	//auf Fehler prüfen
	if (error!=NULL){
		g_error("%s",error->message);
		g_error_free(error);
		error = NULL;
	}
	//ersten Eintrag lesen
	ordner_eintrag = g_dir_read_name(ordner);
	do{
	//Einträge filtern
	ordner_eintrag = dateifilter (ordner_eintrag);
	//Einträge ausgeben
	ausgabe(ordner_eintrag);
	//Schleife solange wiederholen, bis keine Einträge mehr vorhanden (=NULL)
	}while (ordner_eintrag = g_dir_read_name(ordner));
	//Ordner wieder schließen
	g_dir_close(ordner);
	return;
}


///sortiert alle Einträge aus, bis auf *.odf-dateien
gchar* dateifilter (gchar const *pfad)
{
	if (g_str_has_suffix(pfad,".odt"))
		return pfad;
	else
		return NULL;
}

///gibt die Einträge aus
void ausgabe (gchar const *pfad){
	if (!pfad)
		return;
	else
		g_print("%s/%s\n",keyfile_get_searchdir(),pfad);
}
