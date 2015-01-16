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
///@file keyfile.c
///Hier sind alle globalen Funktionen definiert @see keyfile.h

#include "keyfile.h"

///@brief lokale Variable, die nur intern in keyfile.c benutzt wird
GKeyFile *global_keyfile = NULL;


///läd die Keyfile *filename oder bie NULL die default keyfile_file
///@return true bei erfolg, bei fehlern false
gboolean keyfile_init (const gchar *filename){
	GError *error = NULL;
	GKeyFile *keyfile;
	keyfile = g_key_file_new ();

	//Falls Dateiname übergeben wurde, versuchen die Datei zu laden
	if (filename!=NULL){
		if (!g_key_file_load_from_file(keyfile,
																filename,
																G_KEY_FILE_KEEP_COMMENTS,
																&error))
		{
			g_warning("%s",error->message);
			g_error_free(error);
			error = NULL;
			return FALSE;
		}
	}
	//ansonsten die "default" Datei laden, die definiert wurde mit keyfile_file
	else
	{
		if (!g_key_file_load_from_file(keyfile,
																keyfile_file,
																G_KEY_FILE_KEEP_COMMENTS,
																&error))
		{
			g_warning("%s",error->message);
			g_error_free(error);
			error = NULL;
			return FALSE;
		}
	}
	//Falls schon eine Keyfile bereit gestellt wurde, alte freigeben
	if (global_keyfile!=NULL)
	{
		g_key_file_free (global_keyfile);
	}
	//keyfile global verfügbar machen
	global_keyfile = keyfile;
	return TRUE;
}

///gibt einen String mit dem Verzeichnis zurück, dass eingelesen werden soll
///@warning muss mit g_free freigegeben werden
///@return gchar *dir
gchar *keyfile_get_searchdir (void)
{
GError *error = NULL;
gchar *dir = NULL;

//checken ob Keyfile eingelesen wurde
if (global_keyfile==NULL)
{
	g_warning ("Noch kein \"keyfile_init()\" aufgerufen\n");
	if (!keyfile_init (NULL))
	{
		g_error ("Konnte Keyfile nicht laden, bitte erst keyfile_init() aufrufen!");
	}
}


//String aus Keyfile laden
dir = g_key_file_get_string(	global_keyfile,
															"Ordner",
															"Verzeichnis",
															&error);
//auf fehler prüfen
if (error!=NULL||dir==NULL)
{
	g_error("%s",error->message);
	g_error_free(error);
	error = NULL;
}

return dir;
}


///gibt einen String mit dem Verzeichnis zurück, in das das Ergebnis soll
///@warning muss mit g_free freigegeben werden
///@return gchar *dir
gchar *keyfile_get_outputdir (void){
GError *error = NULL;
gchar *dir = NULL;

//checken ob Keyfile eingelesen wurde
if (global_keyfile==NULL)
{
	g_warning ("Noch kein \"keyfile_init()\" aufgerufen\n");
	if (!keyfile_init (NULL))
	{
		g_error ("Konnte Keyfile nicht laden, bitte erst keyfile_init() aufrufen!");
	}
}


//String aus Keyfile laden
dir = g_key_file_get_string(global_keyfile,
														"Ordner",
														"Output",
														&error);
//auf fehler prüfen
if (error!=NULL||dir==NULL)
{
	g_error("%s",error->message);
	g_error_free(error);
	error = NULL;
}

return dir;
}

///gibt einen String mit dem Dateinamen zurück, wie die PDF-Datei heißen soll
///@warning muss mit g_free freigegeben werden
///@return gchar *filename
gchar *keyfile_get_pdf_name (void)
{
	GError *error = NULL;
	gchar *pdfname = NULL;

	//checken ob Keyfile eingelesen wurde
	if (global_keyfile==NULL)
	{
		g_warning ("Noch kein \"keyfile_init()\" aufgerufen\n");
		if (!keyfile_init (NULL))
		{
			g_error ("Konnte Keyfile nicht laden, bitte erst keyfile_init() aufrufen!");
		}
	}


	//String aus Keyfile laden
	pdfname = g_key_file_get_string(global_keyfile,
															"Datei",
															"pdfname",
															&error);
	//auf fehler prüfen
	if (error!=NULL||pdfname==NULL)
	{
		g_error("%s",error->message);
		g_error_free(error);
		error = NULL;
	}

	return pdfname;
}

///gibt einen String mit dem Dateinamen (voller Pfad) zurück, wie die PDF-Datei heißen soll,
///als absoluten Pfad aus.
///@warning muss mit g_free freigegeben werden
///@return gchar *filename_full_path
gchar *keyfile_get_pdf_full_path (void){
	GString *pfad = g_string_new(NULL);
	gchar *tmp = NULL;

	tmp = keyfile_get_outputdir();
	g_string_append(pfad,tmp);
	g_free(tmp);

	g_string_append(pfad,"/");

	tmp = keyfile_get_pdf_name();
	g_string_append(pfad,tmp);
	g_free(tmp);

	return g_string_free(pfad,FALSE);
}

///speichert den übergebenen Pfad in der Keyfile
void keyfile_set_search_dir (const gchar *folderpath){
	GError *error = NULL;

	//neuen Pfad speichern
	g_key_file_set_string(global_keyfile,"Ordner","Verzeichnis",folderpath);
	//keyfile speichern auf HDD, damit die einstellungen erhalten bleiben
	g_key_file_save_to_file(global_keyfile,keyfile_file,&error);
	if (error!=NULL){
		g_error("%s",error->message);
		g_error_free(error);
		error = NULL;
	}
}

///speichert den übergebenen Pfad in der Keyfile,
///um zu bestimmen, wohin das fertige PDF gespeichert wird
void keyfile_set_save_as_dir (const gchar *path){
	GError *error = NULL;

	//neuen Pfad speichern
	g_key_file_set_string(global_keyfile,"Ordner","Output",path);
	//keyfile speichern auf HDD, damit die einstellungen erhalten bleiben
	g_key_file_save_to_file(global_keyfile,keyfile_file,&error);
	if (error!=NULL){
		g_error("%s",error->message);
		g_error_free(error);
		error = NULL;
	}
}
