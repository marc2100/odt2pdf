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
gchar* temp_dir_get (void);

///gibt das aktuelle Temp_Verzeichnis aus, oder NULL
///muss mit g_free() freigegeben werden
gchar* temp_dir_get_strdub (void);

///löscht das gesetzte Temp_Verzeichnis, anschließend würde
///bei abfrage NULL geliefert
void temp_dir_clear (void);

///löscht das Temp-Verzeichnis im Dateisystem,
///und setzt die gespeicherte Variable zurück
void temp_dir_delete (void);

#endif // TEMP_DIR_H
