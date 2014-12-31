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

#ifndef KEYFILE_H
#define KEYFILE_H

#include <stdlib.h>
#include <gtk/gtk.h>
#include "treeviewer.h"

#define keyfile_file "odt2pdf-gtk"

///läd die Keyfile *filename oder bie NULL die default keyfile_file
///@return true bei erfolg, bei fehlern false
gboolean keyfile_init (const gchar *filename);


///gibt einen String mit dem Verzeichnis zurück, dass eingelesen werden soll
///@warning muss mit g_free freigegeben werden
///@return gchar *dir
gchar *keyfile_get_searchdir (void);

///gibt einen String mit dem Verzeichnis zurück, in das das Ergebnis soll
///@warning muss mit g_free freigegeben werden
///@return gchar *dir
gchar *keyfile_get_outputdir (void);


#endif // KEYFILE_H
