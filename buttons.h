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

///@file buttons.h
///Hier sind alle Funktionen deklariert die die Buttons betreffen,
///wie alle G_CALLBACK-Funktionen.
///Die G_Callback-Aufrufe sollten selbsterklären sein.

#ifndef BUTTONS_H
#define BUTTONS_H

#include <stdlib.h>
#include <gtk/gtk.h>
#include "treeviewer.h"
#include "dir_read.h"
#include "keyfile.h"
#include "temp_dir.h"
#include "helper.h"


///G_CALLBACK-Funktion die
void button_refresh_clicked(GtkWidget *widget, gpointer data);
void button_exit_clicked(GtkWidget *widget, gpointer data);
void button_work_clicked(GtkWidget *widget, gpointer data);
void button_open_clicked (GtkWidget *widget, gpointer data);
void button_save_as_clicked (GtkWidget *widget, gpointer data);
void button_about_clicked (GtkWidget *widget, gpointer data);

///verfärbt die Buttons, sobald die Maus darüber ist (nur optische spielerei)
void buttons_entered (GtkWidget *widget, gpointer data);

///unoconv_pid_watch wird aufgerufen sobald der unoconv-Prozess beendet ist,
///startet anschließend den pdftk-prozess, und graut die GUI aus.
void unoconv_child_watch_func (GPid unoconv_pid,		///<PID des Prozesses
																gint status,				///<Return-Status des Prozesses
																gpointer user_data	///<userdaten, die übergeben werden können
																);

///pdftk_pid_watch wird aufgerufen sobald der pdftk-Prozess beendet ist,
///und gibt die GUI wieder frei.
void pdftk_child_watch_func (GPid pdftk_pid,			///<PID des Prozesses
															gint status,				///<Return-Status des Prozesses
															gpointer user_data	///<userdaten, die übergeben werden können
															);
#endif // BUTTONS_H
