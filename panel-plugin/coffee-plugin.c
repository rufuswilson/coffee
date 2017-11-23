/*  $Id$
 *
 *  Copyright (C) 2017 ModExpEng ltd <modexpeng@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include <gtk/gtk.h>
#include <libxfce4util/libxfce4util.h>
#include <libxfce4panel/xfce-panel-plugin.h>

#include "coffee-plugin.h"
#include "coffee-dialogs.h"

/* default settings */
#define DEFAULT_STATE FALSE
#define DEFAULT_SETTING2 1
#define DEFAULT_SETTING3 FALSE



/* prototypes */
static void
coffee_construct (XfcePanelPlugin *plugin);


/* register the plugin */
XFCE_PANEL_PLUGIN_REGISTER (coffee_construct);



void
coffee_save (XfcePanelPlugin *plugin,
             CoffeePlugin    *coffee)
{
  XfceRc *rc;
  gchar  *file;

  /* get the config file location */
  file = xfce_panel_plugin_save_location (plugin, TRUE);

  if (G_UNLIKELY (file == NULL))
    {
       DBG ("Failed to open config file");
       return;
    }

  /* open the config file, read/write */
  rc = xfce_rc_simple_open (file, FALSE);
  g_free (file);

  if (G_LIKELY (rc != NULL))
    {
      /* save the settings */
      DBG(".");
      xfce_rc_write_bool_entry (rc, "state", coffee->state);

      /* close the rc file */
      xfce_rc_close (rc);
    }
}



static void
coffee_read (CoffeePlugin *coffee)
{
  XfceRc      *rc;
  gchar       *file;
  const gchar *value;

  /* get the plugin config file location */
  file = xfce_panel_plugin_save_location (coffee->plugin, TRUE);

  if (G_LIKELY (file != NULL))
    {
      /* open the config file, readonly */
      rc = xfce_rc_simple_open (file, TRUE);

      /* cleanup */
      g_free (file);

      if (G_LIKELY (rc != NULL))
        {
          /* read the settings */
          coffee->state = xfce_rc_read_bool_entry (rc, "state", DEFAULT_STATE);

          /* cleanup */
          xfce_rc_close (rc);

          /* leave the function, everything went well */
          return;
        }
    }

  /* something went wrong, apply default values */
  DBG ("Applying default settings");

  coffee->state = DEFAULT_STATE;
}



static CoffeePlugin *
coffee_new (XfcePanelPlugin *plugin)
{
  CoffeePlugin   *coffee;
  GtkOrientation  orientation;
  GtkWidget      *label;

  /* allocate memory for the plugin structure */
  coffee = panel_slice_new0 (CoffeePlugin);

  /* pointer to plugin */
  coffee->plugin = plugin;

  /* read the user settings */
  coffee_read (coffee);

  /* get the current orientation */
  orientation = xfce_panel_plugin_get_orientation (plugin);

  /* create some panel widgets */
  coffee->ebox = gtk_event_box_new ();
  gtk_widget_show (coffee->ebox);

  coffee->hvbox = gtk_box_new (orientation, 2);
  gtk_widget_show (coffee->hvbox);
  gtk_container_add (GTK_CONTAINER (coffee->ebox), coffee->hvbox);

  /* some coffee widgets */
  coffee->label = gtk_label_new (_("Coffee"));
  gtk_widget_show (coffee->label);
  gtk_box_pack_start (GTK_BOX (coffee->hvbox), coffee->label, FALSE, FALSE, 0);

  return coffee;
}



static void
coffee_free (XfcePanelPlugin *plugin,
             CoffeePlugin    *coffee)
{
  GtkWidget *dialog;

  /* check if the dialog is still open. if so, destroy it */
  dialog = g_object_get_data (G_OBJECT (plugin), "dialog");
  if (G_UNLIKELY (dialog != NULL))
    gtk_widget_destroy (dialog);

  /* destroy the panel widgets */
  gtk_widget_destroy (coffee->hvbox);

  /* free the plugin structure */
  panel_slice_free (CoffeePlugin, coffee);
}



static void
coffee_orientation_changed (XfcePanelPlugin *plugin,
                            GtkOrientation   orientation,
                            CoffeePlugin    *coffee)
{
  /* change the orienation of the box */
  gtk_orientable_set_orientation(GTK_ORIENTABLE(coffee->hvbox), orientation);
}



static gboolean
coffee_size_changed (XfcePanelPlugin *plugin,
                     gint             size,
                     CoffeePlugin    *coffee)
{
  GtkOrientation orientation;

  /* get the orientation of the plugin */
  orientation = xfce_panel_plugin_get_orientation (plugin);

  /* set the widget size */
  if (orientation == GTK_ORIENTATION_HORIZONTAL)
    gtk_widget_set_size_request (GTK_WIDGET (plugin), -1, size);
  else
    gtk_widget_set_size_request (GTK_WIDGET (plugin), size, -1);

  /* we handled the orientation */
  return TRUE;
}



static gboolean
coffee_buttonpress (XfcePanelPlugin *plugin,
               gint             size,
               CoffeePlugin    *coffee)
{
  GtkOrientation orientation;

  /* get the orientation of the plugin */
  orientation = xfce_panel_plugin_get_orientation (plugin);

  /* set the widget size */
  if (orientation == GTK_ORIENTATION_HORIZONTAL)
    gtk_widget_set_size_request (GTK_WIDGET (plugin), -1, size);
  else
    gtk_widget_set_size_request (GTK_WIDGET (plugin), size, -1);

  /* we handled the orientation */
  return TRUE;
}



static gboolean
coffee_toggle (XfcePanelPlugin *plugin,
               gint             size,
               CoffeePlugin    *coffee)
{
  GtkOrientation orientation;

  /* get the orientation of the plugin */
  orientation = xfce_panel_plugin_get_orientation (plugin);

  /* set the widget size */
  if (orientation == GTK_ORIENTATION_HORIZONTAL)
    gtk_widget_set_size_request (GTK_WIDGET (plugin), -1, size);
  else
    gtk_widget_set_size_request (GTK_WIDGET (plugin), size, -1);

  /* we handled the orientation */
  return TRUE;
}



static void
coffee_construct (XfcePanelPlugin *plugin)
{
  CoffeePlugin *coffee;

  /* setup transation domain */
  xfce_textdomain(GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR, "UTF-8");

  /* create the plugin */
  coffee = coffee_new (plugin);

  /* add the ebox to the panel */
  gtk_container_add (GTK_CONTAINER (plugin), coffee->ebox);

  /* show the panel's right-click menu on this ebox */
  xfce_panel_plugin_add_action_widget (plugin, coffee->ebox);

  /* connect plugin signals */
  g_signal_connect (G_OBJECT (plugin), "free-data",
                    G_CALLBACK (coffee_free), coffee);

  g_signal_connect (G_OBJECT (plugin), "save",
                    G_CALLBACK (coffee_save), coffee);

  g_signal_connect (G_OBJECT (plugin), "size-changed",
                    G_CALLBACK (coffee_size_changed), coffee);

  g_signal_connect (G_OBJECT (plugin), "orientation-changed",
                    G_CALLBACK (coffee_orientation_changed), coffee);
                    
  g_signal_connect (G_OBJECT (coffee->label), "button-press-event",
                    G_CALLBACK (coffee_buttonpress), coffee);

  /* show the configure menu item and connect signal */
  xfce_panel_plugin_menu_show_configure (plugin);
  g_signal_connect (G_OBJECT (plugin), "configure-plugin",
                    G_CALLBACK (coffee_configure), coffee);

  /* show the about menu item and connect signal */
  xfce_panel_plugin_menu_show_about (plugin);
  g_signal_connect (G_OBJECT (plugin), "about",
                    G_CALLBACK (coffee_about), NULL);
}
