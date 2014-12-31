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

#ifndef BUTTONS_H
#define BUTTONS_H

#include <stdlib.h>
#include <gtk/gtk.h>
#include "treeviewer.h"
#include "dir_read.h"
#include "keyfile.h"

struct PDFTK_DATA{
	GString *pdftk_cmd;
	gchar *tmp;
};

//G_CALLBACK-Funktionen
void button_refresh_clicked(GtkWidget *widget, gpointer data);
void buttons_entered (GtkWidget *widget, gpointer data);
void button_exit_clicked(GtkWidget *widget, gpointer data);
void button_work_clicked(GtkWidget *widget, gpointer data);



#endif // BUTTONS_H
