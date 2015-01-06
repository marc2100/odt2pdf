#include "temp_dir.h"
gchar *global_temp_dir = NULL;

///speichert das aktuelle Temp_Verzeichnis
void temp_dir_save (gchar *dir){
	global_temp_dir = dir;
}

///gibt das aktuelle Temp_Verzeichnis aus, oder NULL
///muss nicht freigegeben werden
gchar* temo_dir_get (void){
	return global_temp_dir;
}

///löscht das gesetzte Temp_Verzeichnis, anschließend würde
///bei abfrage NULL geliefert
void temp_dir_clear (void){
	global_temp_dir = NULL;
}
