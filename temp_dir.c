#include "temp_dir.h"
gchar *global_temp_dir = NULL;

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
