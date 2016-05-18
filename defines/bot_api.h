//
//
#pragma once

typedef	unsigned long	u_long;


// bot version defines

// construct
#define CF_BOT_VERSION(a,b)		(((((short)(a))&0xFF)<<8)|(((short)(b))&0xFF))

// extract
#define CF_BOT_VERSION_MAJOR(a)	((u_char)((((short)(a))>>8)&0xFF))
#define CF_BOT_VERSION_MINOR(a)	(((u_char)(a))&0xFF)


// plug in module information
typedef struct
{	
	// plug in module version
	short pluginversion;

	// required bot version
	short desired_botversion;		

	// unique module ID. 8 chars + '\0'
	char uniqid[9];

	// module information
	char plugindescription[512];
	char authors[512];
	char email[512];
	char http[512];

} CFBOT_PLUGININFORMATION;


// externals from the bot
typedef struct 
{
	// subscribe to events
	void (*cfbot_subscribe)(void *context, char *plugin_id, u_long events_type);
	// unsubscribe from events
	void (*cfbot_unsubscribe)(void *context, char *plugin_id, u_long events_type);
	// push outgoing event
	void (*cfbot_pushevent)(void *context, char *plugin_id, char* bot_name, char *event_data, int event_size);

#ifdef WIN32
	// initializes settings window. Should be called on initialization
	void (*cfbot_init_settings)(void *context, char *plugin_id, char *event_data, int event_size);
#endif

} CFBOT_CALLBACKS;

// externals should be defined in plug in module
typedef struct
{
	// initialization on module loading
	int	(* cfbot_initialize)(CFBOT_CALLBACKS *callbacks, void *context);
	// finalization on module releasing or bot shutdown
	void(* cfbot_finalize)(void);

	// adding bot client which is going to use this module
	int	(* cfbot_add_client)(char* bot_name, char* work_dir);
	// removing bot client
	void(* cfbot_remove_client)(char* bot_name);

	// request module information
	void (* cfbot_plugininformation)(CFBOT_PLUGININFORMATION *info);
	// push event
	void (* cfbot_pushevent)(char* bot_name, char *event_data, int event_size);

} CFBOT_PLUGIN_CALLBACKS;

// subscribe event types

// chatroom connect/disconnect events
#define CF_BOT_CONNECTION_EVENTS		0x01

// central server IM events
#define CF_BOT_IM_EVENTS				0x02

// chatroom text/talk/users events
#define CF_BOT_ROOM_EVENTS				0x04

// bot to bot operations
#define CF_BOT_2_BOT_EVENTS				0x08

// room timer. every 100 ms only when connected to the room
#define CF_BOT_ROOM_TIMER				0x10

// *nix signal events
#define CF_BOT_SIGNAL					0x20

// service information ban/kick/punish etc. (available in chat room server 5.0 and higher)
#define CF_BOT_SERVICE_EVENTS			0x40

//
// bot -> plug in module event types
//

// connection events

#define BOT_EVENT_ROOM_CONNECTED		0x0101
// room connected
// empty message
//
// NOTE: this packet will be delivered if module will be turned off and then on

#define BOT_EVENT_ROOM_DENIED			0x0102
// denied from room
// deny reason			std::string	0x01

#define BOT_EVENT_ROOM_KILLED			0x0103
// kicked from room
// kick reason			std::string	0x01

#define BOT_EVENT_ROOM_DISCONNECTED		0x0104
// disconnected from room
// empty message

// room events

#define BOT_EVENT_ROOM_IN				0x0401
// user in
// nickname			std::string		0x01
// flags			u_long			0x02
// age				u_char			0x03
// count			int				0x04
// giftpoints		u_long			0x05	(on Camfrog Room Server 5.0 and higher)
//
// count value will be set in some "N" value in case of initial user list (bot room logon)
// in other cases this value will be "1"
// this event is repeating few times for every user in case of flags changing
// flags description:
// audio			1
// video			2
// female user		4
// OP or OPPLUS		8
// FRIEND			16
// OWNER			32
// punished user	64
// BOT				128
// privacy mode		256
// ignored user		512
// audio blocked	1024
// PRO user			2048
//
// NOTE: this packet will be delivered if module will be turned off and then on

#define BOT_EVENT_ROOM_OUT				0x0402
// user out
// nickname		std::string		0x01

#define BOT_EVENT_ROOM_TALK				0x0403
// room talk event
// state			u_char			0x01
// nickname			std::string		0x02
//
// state = "1" - start talk, "0" - stop talk
// nickname is available only if state is set to "1"

#define BOT_EVENT_ROOM_MOTD				0x0404
// room MOTD
// MOTD text		std::string		0x01

#define BOT_EVENT_ROOM_TOPIC			0x0405
// room topic
// state			u_char			0x01
// topic text		std::string		0x02
//
// state = "1" - topic on, "0" - topic off
// text is available only if state is set to "1"

#define BOT_EVENT_ROOM_TEXT				0x0406
// room text
// nickname			std::string		0x01
// text				std::string		0x02
// attributes		u_char			0x03
// size				u_char			0x04
// color			u_long			0x05
// effects			u_long			0x06
// charset			u_char			0x07
// pitch and family	u_char			0x08
// font name		std::string		0x09
//
// nickname can be empty. in this case this text is a server message
// attributes can be set to "0" - in this case all following parameters are absent
// effects can contain following flags CFE_BOLD | CFE_ITALIC | CFE_UNDERLINE

#define BOT_EVENT_ROOM_NAME				0x0407
// current room name (on room connection)
// room name		std::string			0x01
//
// NOTE: this packet will be delivered if module will be turned off and then on


// instant message events

#define BOT_EVENT_IM					0x0201
// IM message
// nickname_from	std::string		0x01
// feedback			int				0x02
// age				u_char			0x03
// text				std::string		0x04
// attributes		u_char			0x05
// size				u_char			0x06
// color			u_long			0x07
// effects			u_long			0x08
// charset			u_char			0x09
// pitch and family	u_char			0x0A
// font name		std::string		0x0B
//
// attributes can be set to "0" - in this case all following parameters are absent
// effects can contain following flags CFE_BOLD | CFE_ITALIC | CFE_UNDERLINE


// bot to bot events

#define BOT_EVENT_BOT_2_BOT				0x0801
// plugin to plugin message
// UNIQID			std::string		0x01
// ...
// ...
// SENDER UNIQID	std::string		0xFFFF

#define BOT_EVENT_MODULES				0x0802
// installed modules notifier
// COUNT			int				0x01
// UNIQID			std::string		0x02 + i

#define BOT_EVENT_ROOM_TIMER			0x1001
// room timer. raises every 100 msec only when bot connected to the room
// empty message

#define BOT_SIGNAL_UPDATE_CFG			0x2201
// signal to reload all configuration files
// empty message

#define BOT_SIGNAL_COPY_CFG				0x2202
// signal to copy configuration files
// destination path		std::string		0x01

// service channel events

#define BOT_EVENT_ROOM_KICK				0x4001
// user kicked
// nickname_to			std::string		0x00
// nickname_from		std::string		0x01
// reason				std::string		0x02

#define BOT_EVENT_ROOM_SETOPT			0x4002
// room options was set
// option				std::string		0x00
// nickname_from		std::string		0x01
// option value			std::string		0x02

#define BOT_EVENT_ROOM_PUNISH			0x4003
// user was punished
// nickname_to			std::string		0x00
// nickname_from		std::string		0x01
// reason				std::string		0x02
//
// reason message consist of two parts, e.g. "ExpireTime ReasonMessage"
// so, the expire time is separated from the reason message by space
// expire time			time_t

#define BOT_EVENT_ROOM_UNPUNISH			0x4004
// user was unpunished
// nickname_to			std::string		0x00
// nickname_from		std::string		0x01

#define BOT_EVENT_ROOM_BAN_ADD			0x4005
// user was banned
// ban list record		std::string		0x00
// nickname_from		std::string		0x01
//
// ban list record sample : deny|allow nick|ip|nick_ip [nickname] [ip mask] no-expire|expire interval

#define BOT_EVENT_ROOM_BAN_REMOVE		0x4006
// user was unbanned
// ban list record		std::string		0x00
// nickname_from		std::string		0x01
//
// ban list record sample : deny|allow nick|ip|nick_ip [nickname] [ip mask] no-expire|expire interval

#define BOT_EVENT_ROOM_BLOCKMIC			0x4007
// user's mic was blocked
// nickname_to			std::string		0x00
// nickname_from		std::string		0x01

#define BOT_EVENT_ROOM_UNBLOCKMIC		0x4008
// user's mic was unblocked
// nickname_to			std::string		0x00
// nickname_from		std::string		0x01

#define BOT_EVENT_ROOM_MAKEOP			0x4009
// user got new role  (operator, owner, friend)
// nickname_to			std::string		0x00
// nickname_from		std::string		0x01
// new role				std::string		0x02

#define BOT_EVENT_ROOM_DEOP				0x4010
// user was revoked operator status
// nickname_to			std::string		0x00
// nickname_from		std::string		0x01
// last role			std::string		0x02


// plug in module -> bot event types

#define PLUGIN_EVENT_DISCONNECT			0x0151
// disconnect from the active room and central server

#define PLUGIN_EVENT_RESUME				0x0152
// connect to last active room

#define PLUGIN_EVENT_PAUSE				0x0153
// disconnect from active room

#define PLUGIN_EVENT_IM					0x0251
// send IM message
// nickname_to		std::string		0x01
// text				std::string		0x02
// attributes		u_char			0x03
// size				u_char			0x04
// color			u_long			0x05
// effects			u_long			0x06
// charset			u_char			0x07
// pitch and family	u_char			0x08
// font name		std::string		0x09
//
// attributes can be set to "0" - in this case all following parameters are absent
// effects can contain following flags CFE_BOLD | CFE_ITALIC | CFE_UNDERLINE

#define PLUGIN_EVENT_ROOM_TEXT			0x0451
// send message to the room
// text				std::string		0x01
// attributes		u_char			0x02
// size				u_char			0x03
// color			u_long			0x04
// effects			u_long			0x05
// charset			u_char			0x06
// pitch and family	u_char			0x07
// font name		std::string		0x08
//
// attributes can be set to "0" - in this case all following parameters are absent - message will be default font/size
// effects can contain following flags CFE_BOLD | CFE_ITALIC | CFE_UNDERLINE

#define PLUGIN_BOT_2_BOT				0x0851
// send message to another plugin
// UNIQID			std::string		0x01
// data				std::string		0x02
// ...

// Note: As usual data can be processed as bot_exchange_format packet.
// See more in b2b_proto.h

#define PLUGIN_SETTINGS_GET_REPLY		0x8051
#define PLUGIN_SETTINGS_STATE_REPLY		0x8052

// settings events
#define BOT_EVENT_SETTINGS_GET			0x8001
#define BOT_EVENT_SETTINGS_PROCESS		0x8002
#define BOT_EVENT_SETTINGS_APPLY		0x8003
#define BOT_EVENT_SETTINGS_CANCEL		0x8004
