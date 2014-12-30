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
#ifndef DIR_READ_H
#define DIR_READ_H

#include <stdlib.h>
#include <gtk/gtk.h>
#include "treeviewer.h"
#include "buttons.h"

///ließt die Einträge aus dem in der Keyfile
///angegebenen Ordner aus
void ordner_auslesen (GtkListStore* store);

#endif // DIR_READ_H
