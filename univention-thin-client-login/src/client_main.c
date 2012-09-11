/*
 * Univention Client Login
 *	this file is part of the Univention thin client tools
 *
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006 Univention GmbH
 *
 * http://www.univention.de/
 *
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Binary versions of this file provided by Univention to you as
 * well as other copyrighted, protected or trademarked materials like
 * Logos, graphics, fonts, specific documentations and configurations,
 * cryptographic keys etc. are subject to a license agreement between
 * you and Univention.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include "debug.h"
#include "signals.h"
#include "connection.h"
#include "protocol.h"
#include "script.h"
#include "process.h"
#include "security.h"
#include "command.h"
#include "alive.h"
#include "select_loop.h"
#include "command_socket.h"

/* command line args */
int debug_level = 0;
char * session_script = NULL;
char * prog_name = NULL;
char * server_host = NULL;
char * server_prog = NULL;
char * script_name = NULL;
char * socket_name = NULL;

extern long GLOBAL_TIMEOUT;

static void usage(const char *exeName)
{
  fprintf(stderr, "\nusage: %s [options]\n\n", exeName);
  fprintf(stderr, "options:\n");
  fprintf(stderr, "   -s host  set the name of the server\n");
  fprintf(stderr, "   -r prog  set the name of the remote server program\n");
  fprintf(stderr, "   -p prog  set the name of the local session setup script\n");
  fprintf(stderr, "   -f file  set the name of the local input socket\n");
  fprintf(stderr, "   -d<n>    set debug level to <n>\n");
  fprintf(stderr, "   -t<sec>  set timeout\n");
  fprintf(stderr, "   -h       display this help message\n");
  fprintf(stderr, "\n");
}

static void parse_args( int argc, char *argv[] )
{
	int i;
	int j=0;
	char *timeout;

	prog_name = strdup ( argv[0] );

	for (i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-')
		{
			switch(argv[i][1])
			{
				case 's':
					if ( argv[i+1] == NULL ) {
						usage ( prog_name );
						exit(1);
					}
					server_host = strdup ( argv[i+1] );
					i++;
					break;
				case 'p':
					if ( argv[i+1] == NULL ) {
						usage ( prog_name );
						exit(1);
					}
					script_name = strdup ( argv[i+1] );
					i++;
					break;
				case 'r':
					if ( argv[i+1] == NULL ) {
						usage ( prog_name );
						exit(1);
					}
					server_prog = strdup ( argv[i+1] );
					i++;
					break;
				case 'f':
					if ( argv[i+1] == NULL ) {
						usage ( prog_name );
						exit(1);
					}
					socket_name = strdup ( argv[i+1] );
					i++;
					break;
				case 'd':
					if (isdigit(argv[i][2])) debug_level = atoi( argv[i] + 2 );
					else debug_level++;
					break;
				case 't':
					/* getopt would be nice ... */
					timeout=malloc( (strlen(argv[i]) - 1 ) * sizeof(char));
					memset(timeout, 0, strlen(argv[i]) -1 );
					for(j=2;j<strlen(argv[i]); j++) {
						timeout[j-2]=argv[i][j];
					}
					GLOBAL_TIMEOUT = atol( timeout );
					free(timeout);
					break;
				case 'h':
					usage( prog_name );
					exit(0);
					break;
				default:
					fprintf( stderr, "Unknown option '%s'\n", argv[i] );
					usage( prog_name );
					exit(1);
			}
		}
		else
		{
			usage( prog_name );
			exit(1);
		}
	}
}

int main( int argc, char *argv[] )
{
	GLOBAL_TIMEOUT=600;

    parse_args( argc, argv );
    init_security ();
    signal_init();
    atexit(kill_childs);
    connect_to_server();
    if (socket_name)
      command_socket_server_init(socket_name);
    start_script( "start" );
    atexit(run_stop_script);
    client_keep_alive_init();

    select_loop();

    return 0;

}
