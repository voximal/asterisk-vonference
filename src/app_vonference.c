/*
 * app_vonference
 *
 * A channel independent conference application for Asterisk
 *
 * Copyright (C) 2002, 2003 Junghanns.NET GmbH
 * Copyright (C) 2003, 2004 HorizonLive.com, Inc.
 * Copyright (C) 2005, 2006 HorizonWimba, Inc.
 * Copyright (C) 2007 Wimba, Inc.
 *
 * Klaus-Peter Junghanns <kapejod@ns1.jnetdns.de>
 *
 * Video Conferencing support added by
 * Neil Stratford <neils@vipadia.com>
 * Copyright (C) 2005, 2005 Vipadia Limited
 *
 * VAD driven video conferencing, text message support
 * and miscellaneous enhancements added by
 * Mihai Balea <mihai at hates dot ms>
 *
 * This program may be modified and distributed under the
 * terms of the GNU General Public License. You should have received
 * a copy of the GNU General Public License along with this
 * program; if not, write to the Free Software Foundation, Inc.
 * 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

// SVN revision number, provided by make
#ifndef REVISION
#define REVISION "unknown"
#endif

static char *revision = REVISION;

#include "app_vonference.h"
#include "conference.h"
#include "cli.h"


ASTERISK_FILE_VERSION(__FILE__, REVISION)

/*
 *
 * a conference has N threads, where N is the number of members
 *
 * each member thread reads frames from its channel adding them 
 * to its frame queue which is read by the conference thread
 *
 * the conference thread reads frames from each speaking members
 * queue, mixes them, and then queues them to the member threads
 *
 */

static char *app = "Vonference";
static char *synopsis = "Channel Independent Video Conference";
static char *descrip = "Channel Independent Video Conference Application";

static char *app2 = "VonferenceCount";
static char *synopsis2 = "Channel Independent Video Conference Count";
static char *descrip2 = "Channel Independent Video Conference Count Application";

static int app_vonference_main(struct ast_channel* chan, const char* data)
{
	int res ;
	struct ast_module_user *u ;

	u = ast_module_user_add(chan);

	// call member thread function
	res = member_exec( chan, data ) ;

	ast_module_user_remove(u);

	return res ;
}

static int app_vonferencecount_main(struct ast_channel* chan, const char* data)
{
	int res ;
	struct ast_module_user *u ;

	u = ast_module_user_add(chan);

	// call count thread function
	res = count_exec( chan, data ) ;

	ast_module_user_remove(u);

	return res ;
}

static int unload_module( void )
{
	int res = 0;

	ast_log( LOG_NOTICE, "Unloading app_vonference module\n" ) ;

	ast_module_user_hangup_all();

	unregister_conference_cli() ;

	res |= ast_unregister_application( app ) ;
	res |= ast_unregister_application( app2 ) ;

	dealloc_conference() ;
	
	return res ;
}

static int load_module( void )
{
	int res = 0;

	ast_log( LOG_NOTICE, "Loading app_vonference module, revision=%s\n", revision) ;

	init_conference() ;

	res |= ast_register_application( app, app_vonference_main, synopsis, descrip ) ;
	res |= ast_register_application( app2, app_vonferencecount_main, synopsis2, descrip2 ) ;

	register_conference_cli(ast_module_info) ;

	return res ;
}

#undef AST_BUILDOPT_SUM
#define AST_BUILDOPT_SUM ""

#define AST_MODULE "Vonference"
AST_MODULE_INFO_STANDARD(ASTERISK_GPL_KEY,
		"Channel Independent Video Conference Application");
#undef AST_MODULE
