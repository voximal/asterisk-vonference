
// $Id: conference.h,v 1.1 2016/05/26 08:45:43 borja.sixto Exp $

/*
 * app_conference
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

#ifndef _VONFERENCE_CONFERENCE_H
#define _VONFERENCE_CONFERENCE_H

//
// includes
//

#include "app_vonference.h"
#include "conf_frame.h"
#include "member.h"

//
// defines
//

#ifndef ASTERISK_VERSION_NUM

 #ifdef AST_MAX_UNIQUEID
  //#warning WARNING: DETECT AST_MAX_UNIQUEID
  #ifdef AST_FORMAT_ATTR_SIZE
   //#warning WARNING: DETECT AST_FORMAT_ATTR_SIZE
   #define ASTERISK_VERSION_NUM 120000
  #else
   #define ASTERISK_VERSION_NUM 130000
  #endif
 #endif

 #ifdef DEFAULT_PARKINGLOT
  //#warning WARNING: INCLUDING ast_version.h
  #include <asterisk/ast_version.h>
 #else
  #ifdef AST_JITTERBUFFER_FD
    //#warning WARNING: INCLUDING ast_version.h
    #include <asterisk/ast_version.h>
  #else
    //#warning WARNING: INCLUDING version.h
    #include <asterisk/version.h>
  #endif
 #endif

#endif

#define AST_4       10400
#define AST_6       10600
#define AST_6_1     10601
#define AST_8       10800
#define AST_10		  100000
#define AST_11		  110000
#define AST_12			120000
#define AST_13			130000
#define AST_99			990000

/* Forcing NUM Version ? */
#if ASTERISK_VERSION_NUM_FORCE
  #define ASTERISK_VERSION_NUM ASTERISK_VERSION_NUM_FORCE
  #warning "Compilation Force ASTERISK_VERSION_NUM"
#else
     /* Only videocaps are SVN normaly */
    #if ASTERISK_VERSION_NUM > AST_99
     #undef ASTERISK_VERSION_NUM
     #ifdef AST_FORMAT_SLINEAR16
      #define ASTERISK_VERSION_NUM 10602
     #else
      #define ASTERISK_VERSION_NUM 10499
     #endif
     #undef ASTERISK_VERSION
     #define ASTERISK_VERSION "VideoCaps"
     #warning
     #warning "Compilation for VideoCaps !"
     #warning
    #endif
#endif

#ifndef ASTERISK_VERSION_NUM
#define ASTERISK_VERSION_NUM AST_11
#endif

/* For Asterisk 1.4.x */
#if ASTERISK_VERSION_NUM < AST_6
 #warning
 #warning "Compilation for Asterisk 1.4 !"
 #warning

#elif ASTERISK_VERSION_NUM >= AST_6 && ASTERISK_VERSION_NUM < AST_8
/* For Asterisk 1.6.x */
 #warning
 #warning "Compilation for Asterisk 1.6 !"
 #warning

#elif ASTERISK_VERSION_NUM >= AST_8 && ASTERISK_VERSION_NUM < AST_10
/* For Asterisk 1.8.x */
 #warning
 #warning "Compilation for Asterisk 1.8 !"
 #warning

#elif ASTERISK_VERSION_NUM >= AST_10 && ASTERISK_VERSION_NUM < AST_11
/* For Asterisk 10 */
 #warning
 #warning "Compilation for Asterisk 10 !"
 #warning

#elif ASTERISK_VERSION_NUM >= AST_11 && ASTERISK_VERSION_NUM < AST_12
/* For Asterisk 11 */
 #warning
 #warning "Compilation for Asterisk 11 !"
 #warning

#elif ASTERISK_VERSION_NUM >= AST_12  && ASTERISK_VERSION_NUM < AST_13
/* For Asterisk 12 */
 #warning
 #warning "Compilation for Asterisk 12 !"
 #warning

#else
/* For Asterisk 13 */
 #warning
 #warning "Compilation for Asterisk 13 !"
 #warning
#endif

#include <version_macros.h>


#define CONF_NAME_LEN 80

//
// struct declarations
//

typedef struct ast_conference_stats
{
	// conference name ( copied for ease of use )
	char name[CONF_NAME_LEN + 1] ;

	// type of connection
	unsigned short phone ;
	unsigned short iaxclient ;
	unsigned short sip ;

	// type of users
	unsigned short moderators ;
	unsigned short conferees ;

	// accounting data
	unsigned long frames_in ;
	unsigned long frames_out ;
	unsigned long frames_mixed ;

	struct timeval time_entered ;

} ast_conference_stats ;

struct ast_conference
{
	// conference name
	char name[CONF_NAME_LEN + 1] ;

	// conference volume
	int volume;

	// single-linked list of members in conference
	struct ast_conf_member* memberlist ;
#ifndef	VIDEO
	// pointer to last member in list
	struct ast_conf_member* memberlast ;
#endif
	int membercount ;
        int id_count;
#ifdef	VIDEO
	// id of the default video source
	// If nobody is talking and video is unlocked, we use this source
	int default_video_source_id;

	// id of the current video source
	// this changes according to VAD rules and lock requests
	int current_video_source_id;

	// timestamp of when the current source has started talking
	// TODO: do we really need this?
	//struct timeval current_video_source_timestamp;

	// Video source locked flag, 1 -> locked, 0 -> unlocked
	short video_locked;
#endif
	// conference thread id
	pthread_t conference_thread ;

	// conference data lock
	ast_rwlock_t lock ;

	// pointers to conference in doubly-linked list
	struct ast_conference* next ;
	struct ast_conference* prev ;

	// pointer to conference's bucket list head
	struct conference_bucket *bucket;
	// list entry for conference's bucket list
	AST_LIST_ENTRY(ast_conference) hash_entry ;

	// pointer to translation paths
	struct ast_trans_pvt* from_slinear_paths[ AC_SUPPORTED_FORMATS ] ;

	// conference stats
	ast_conference_stats stats ;

	// keep track of current delivery time
	struct timeval delivery_time ;
#ifdef	VIDEO
	// the conference does chat mode: special treatment for situations with 1 and 2 members
	short does_chat_mode;

	// chat mode is on;
	short chat_mode_on;
#endif	
#ifdef	APP_VONFERENCE_DEBUG
	// 1 => on, 0 => off
	short debug_flag ;
#endif
	// flag indicating we should remove this member
	char kick_flag ;
} ;

//
// function declarations
//

int hash( const char *channel_name ) ;

int count_exec( struct ast_channel* chan, const char* data ) ;

struct ast_conference* join_conference( struct ast_conf_member* member, char* conf_name, char* max_users_flag ) ;
#ifdef ulex
int check_moderators( const char* confname) ;
int remove_speakers( struct ast_conference *conf, int hangup );
#endif

int end_conference( const char *name, int hangup ) ;

// Find a particular member, locked if found.
struct ast_conf_member *find_member( const char *chan ) ;
struct ast_conference* join_conference( struct ast_conf_member* member, char* conf_name, char* max_users_flag ) ;
#ifdef ulex
struct ast_conference *find_conf( const char* name ) ;
struct ast_conference *find_member_conf( struct ast_conf_member *member ) ;
#endif


int queue_frame_for_listener( struct ast_conference* conf, struct ast_conf_member* member, conf_frame* frame ) ;
int queue_frame_for_speaker( struct ast_conference* conf, struct ast_conf_member* member, conf_frame* frame ) ;
int queue_silent_frame( struct ast_conference* conf, struct ast_conf_member* member ) ;

void remove_member( struct ast_conf_member* member, struct ast_conference* conf, char* conf_name ) ;

#ifdef	TEXT
int send_text_message_to_member(struct ast_conf_member *member, const char *text);
#endif

// called by app_confernce.c:load_module()
void init_conference( void ) ;
void dealloc_conference( void ) ;

#ifdef	CACHE_CONTROL_BLOCKS
void freeconfblocks( void ) ;
#endif

int get_conference_count( void ) ;

int show_conference_list ( int fd, const char* name );

#ifdef	MANAGER_COMMANDS
int manager_conference_list( struct mansession *s, const struct message *m);
int manager_conference_end(struct mansession *s, const struct message *m);
#endif

int show_conference_stats ( int fd );
int kick_member ( const char* confname, int user_id);
int kick_channel ( const char *confname, const char *channel);
int kick_all ( void );
int mute_member ( const char* confname, int user_id);
int unmute_member ( const char* confname, int user_id);
int mute_conference ( const char* confname);
int unmute_conference ( const char* confname);

#ifdef	VIDEO
int viewstream_switch ( const char* confname, int user_id, int stream_id);
int viewchannel_switch ( const char* confname, const char* user_chan, const char* stream_chan);
#endif

int get_conference_stats( ast_conference_stats* stats, int requested ) ;
int get_conference_stats_by_name( ast_conference_stats* stats, const char* name ) ;

#ifdef	VIDEO
int lock_conference(const char *conference, int member_id);
int lock_conference_channel(const char *conference, const char *channel);
int unlock_conference(const char *conference);

int set_default_id(const char *conference, int member_id);
int set_default_channel(const char *conference, const char *channel);

int video_mute_member(const char *conference, int member_id);
int video_unmute_member(const char *conference, int member_id);
int video_mute_channel(const char *conference, const char *channel);
int video_unmute_channel(const char *conference, const char *channel);
#endif

#ifdef ulex
int change_allow_member(const char *conference, int member_id);
int change_disallow_member(const char *conference, int member_id);
#endif

#ifdef	TEXT
int send_text(const char *conference, int member, const char *text);
int send_text_channel(const char *conference, const char *channel, const char *text);
int send_text_broadcast(const char *conference, const char *text);
#endif

#ifdef	VIDEO
int drive(const char *conference, int src_member_id, int dst_member_id);
int drive_channel(const char *conference, const char *src_channel, const char *dst_channel);
#endif

int play_sound_channel(int fd, const char *channel, char **file, int mute, int tone, int n);
#ifdef ulex
int play_sound_conference(int fd, const char *name, const char *file, int mute);
#endif
int stop_sound_channel(int fd, const char *channel);
#ifdef ulex
int stop_sound_conference(int fd, const char *name);
#endif

int start_moh_channel(int fd, const char *channel);
int stop_moh_channel(int fd, const char *channel);

int talk_volume_channel(int fd, const char *channel, int up);
int listen_volume_channel(int fd, const char *channel, int up);
int volume(int fd, const char *conference, int up);

#ifdef	APP_VONFERENCE_DEBUG
int set_conference_debugging( const char* name, int state ) ;
#endif

#endif
