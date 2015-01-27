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

///@file helper.c
///Sammlung von Hilfsfunktionen, die sonst nicht zugeordnet
///werden konnten. @see helper.h

#include "helper.h"

///gibt den Speicher im Array frei..
void ptr_array_clean (gpointer data){
	static gint i=0;
	g_print("i = %d\tText = %s\n",i,(gchar*)data);
	g_free(data);
	i++;
}

///füllt das Array mit dem String, dieser kann danach einfach per g_free(),
///freigegeben werden, wenn das Array zustört wird.
void fill_g_ptr_array (GPtrArray *ptr_array,gchar* data){
	gchar *ptr_temp=NULL;
	ptr_temp = (gchar*)g_malloc(sizeof(data));
	ptr_temp = g_strdup (data);
	g_ptr_array_add(ptr_array,(gpointer)ptr_temp);
	return;
}

///graut die Buttons im Interface aus
///@param status True=aktive False=ausgegraut
void 	buttons_ausgrauen(gboolean status){
	gtk_widget_set_sensitive (gui_get_button_refresh(),status);
	gtk_widget_set_sensitive (gui_get_button_exit(),status);
	gtk_widget_set_sensitive (gui_get_button_work(),status);
}

///graut das ganze Interface aus (mit den Buttons)
///@param status True=aktive False=ausgegraut
void interface_ausgrauen (gboolean status){
	buttons_ausgrauen(status);
	gtk_widget_set_sensitive((GtkWidget*)gui_get_gtk_tree_viewer(),status);
}

void  entry_save (GtkWidget *widget, gpointer data){
	g_print("Hallo\n");
}
