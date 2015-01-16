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

///@file keyfile.h
///Hier sind alle globalen Funktionen deklariert,
///um die Keyfile auslesen zu lassen, und die jeweilig
///interessante Information zu erhalten.
///Zudem wird hier der default-Standart für die Keyfile eingestellt

#ifndef KEYFILE_H
#define KEYFILE_H

#include <stdlib.h>
#include <gtk/gtk.h>
#include "treeviewer.h"

///@brief Keyfile die per default geladen wird
#define keyfile_file "odt2pdf-gtk.conf"


///läd die Keyfile filename oder bei NULL die default keyfile_file odt2pdf-gtk.conf
///@return true bei erfolg, bei fehlern false
///@param filename Pfadangabe zur Keyfile, oder NULL
gboolean keyfile_init (const gchar *filename);


///gibt einen String mit dem Verzeichnis zurück, dass eingelesen werden soll
///@warning muss mit g_free freigegeben werden
///@return gchar *dir
gchar *keyfile_get_searchdir (void);

///gibt einen String mit dem Verzeichnis zurück, in das das Ergebnis soll
///@warning muss mit g_free freigegeben werden
///@return gchar *dir
gchar *keyfile_get_outputdir (void);

///gibt einen String mit dem Dateinamen zurück, wie die PDF-Datei heißen soll
///@warning muss mit g_free freigegeben werden
///@return gchar *filename
gchar *keyfile_get_pdf_name (void);

///gibt einen String mit dem Dateinamen (voller Pfad) zurück, wie die PDF-Datei heißen soll,
///als absoluten Pfad aus.
///@warning muss mit g_free freigegeben werden
///@return gchar *filename_full_path
gchar *keyfile_get_pdf_full_path (void);

///speichert den übergebenen Pfad in der Keyfile
void keyfile_set_search_dir (const gchar *folderpath);

///speichert den übergebenen Pfad in der Keyfile,
///um zu bestimmen, wohin das fertige PDF gespeichert wird
void keyfile_set_save_as_dir (const gchar *path);
#endif // KEYFILE_H
