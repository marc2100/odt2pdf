/*
Copyright (c) 2014 "Marcus Pries"

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

///@file helper.h
///Sammlung von Hilfsfunktionen, die sonst nicht zugeordnet
///werden konnten.

#ifndef HELPER_H
#define HELPER_H

#include <stdlib.h>
#include <gtk/gtk.h>
#include "treeviewer.h"
#include "dir_read.h"
#include "keyfile.h"

///gibt den Speicher im Array frei..
void ptr_array_clean (gpointer data);

///füllt das Array mit dem String, dieser kann danach einfach per g_free(),
///freigegeben werden, wenn das Array zustört wird.
void fill_g_ptr_array (GPtrArray *ptr_array,gchar* data);


///graut die Buttons im Interface aus
///@param status True=aktive False=ausgegraut
void buttons_ausgrauen(gboolean status);

///graut das ganze Interface aus (mit den Buttons)
///@param status True=aktive False=ausgegraut
void interface_ausgrauen (gboolean status);

#endif // HELPER_H
