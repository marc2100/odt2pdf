#ifndef TEMP_DIR_H
#define TEMP_DIR_H


#include <stdlib.h>
#include <gtk/gtk.h>
#include "treeviewer.h"
#include "dir_read.h"
#include "keyfile.h"

///speichert das aktuelle Temp_Verzeichnis
void temp_dir_save (gchar *dir);

///gibt das aktuelle Temp_Verzeichnis aus, oder NULL
///muss nicht freigegeben werden
gchar* temo_dir_get (void);

///löscht das gesetzte Temp_Verzeichnis, anschließend würde
///bei abfrage NULL geliefert
void temp_dir_clear (void);

#endif // TEMP_DIR_H
