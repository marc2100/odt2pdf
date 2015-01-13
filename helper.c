#include "helper.h"

//gibt den Speicher im Array frei..
void ptr_array_clean (gpointer data){
	static gint i=0;
	g_print("i = %d\tText = %s\n",i,data);
	g_free(data);
	i++;
}


void fill_g_ptr_array (GPtrArray *ptr_array,gchar* data){
	gchar *ptr_temp=NULL;
	ptr_temp = (gchar*)g_malloc(sizeof(data));
	ptr_temp = g_strdup (data);
	g_ptr_array_add(ptr_array,(gpointer)ptr_temp);
	return;
}

void 	buttons_ausgrauen(gboolean status){
	gtk_widget_set_sensitive (gui_get_button_refresh(),status);
	gtk_widget_set_sensitive (gui_get_button_exit(),status);
	gtk_widget_set_sensitive (gui_get_button_work(),status);
}

void interface_ausgrauen (gboolean status){
	buttons_ausgrauen(status);
	gtk_widget_set_sensitive(gui_get_gtk_tree_viewer(),status);
}
