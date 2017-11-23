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

#ifndef __COFFEE_DIALOGS_H__
#define __COFFEE_DIALOGS_H__

G_BEGIN_DECLS

void
coffee_configure    (XfcePanelPlugin *plugin,
                     CoffeePlugin    *coffee);

void
coffee_about        (XfcePanelPlugin *plugin);

G_END_DECLS

#endif
