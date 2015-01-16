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
///@file main.c
///Startpunkt für das Programm, hier werden
///auch die übergeben Konsolenbefehle an
///GTK weitergeleitet, und die Gui initiiert.

#include <stdlib.h>
#include <gtk/gtk.h>
#include "treeviewer.h"


///@brief Startpunkt des Programms
int main (int argc, char *argv[])
{


  /* Initialize GTK+ */
  g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, (GLogFunc) gtk_false, NULL);
  gtk_init (&argc, &argv);
  g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, g_log_default_handler, NULL);

  //Keyfile laden
  keyfile_init (NULL);

	//
	gui_init ();
  return 0;
}



