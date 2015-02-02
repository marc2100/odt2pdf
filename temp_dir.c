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
///@file temp_dir.c
///Hier sind alle Funktionen definiert, die das Temp-Verzeichnis betreffen @see temp_dir.h

#include "temp_dir.h"
static gchar *global_temp_dir = NULL;

///speichert das aktuelle Temp_Verzeichnis
void temp_dir_save (gchar *dir){
	//checken ob schon belegt
	if (global_temp_dir!=NULL){
		//falls ja freigeben
		temp_dir_clear();
	}
	//neues Verzeichnis speichern
	global_temp_dir = dir;

}

///gibt das aktuelle Temp_Verzeichnis aus, oder NULL
///muss nicht freigegeben werden
gchar* temp_dir_get (void){
	return global_temp_dir;
}

///gibt das aktuelle Temp_Verzeichnis aus, oder NULL
///muss mit g_free() freigegeben werden
gchar* temp_dir_get_strdub (void){
	return g_strdup(global_temp_dir);
}
///löscht das gesetzte Temp_Verzeichnis, anschließend würde
///bei abfrage NULL geliefert
void temp_dir_clear (void){
	g_free(global_temp_dir);
	global_temp_dir = NULL;
}

///löscht das Temp-Verzeichnis im Dateisystem,
///und setzt die gespeicherte Variable zurück
void temp_dir_delete (void){
	GError *error = NULL;

	//aufräumen des temp-verzeichnis
	GString *rm_cmd = g_string_new("rm -R ");
	rm_cmd = g_string_append(rm_cmd,temp_dir_get());
	g_print("%s\n",temp_dir_get());
	if(!g_spawn_command_line_sync(rm_cmd->str,NULL,NULL,NULL,&error) ){
		g_warning("%s",error->message);
		g_error_free(error);
		error = NULL;
	}
	g_string_free(rm_cmd,TRUE);

	//globale variable löschen
	if (global_temp_dir!=NULL){
		g_free(global_temp_dir);
	}
	global_temp_dir=NULL;

}
