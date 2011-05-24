/*
 * dhcpcd - DHCP client daemon -
 * Copyright (C) 2002 Red Hat, Inc.,
 * Copyright (C) 2002 Christopher Blizzard <blizzard@redhat.com>
 * dhcpcd is an RFC2131 and RFC1541 compliant DHCP client daemon.
 *
 * This is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

int  readDhcpCache(void);
void writeDhcpCache(void);
void deleteDhcpCache(void);
void releaseDhcpOptionsCached(void);
void useDhcpOptionsCached(void);

void touch(const char *name);
int kill_pidfile(char *file, int signal);
