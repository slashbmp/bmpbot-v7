//
//

#pragma once

#ifndef WIN32
#include <inttypes.h>
#endif

typedef unsigned char		u_char;
typedef unsigned long		u_long;
typedef unsigned int		u_int;

#ifdef WIN32
typedef __int64				int64_t;
#endif

#ifndef COMPILE_WEB_BOT
#define PLUGIN_VERSION_MAJ	5
#define PLUGIN_VERSION_MIN	1
#else
#define PLUGIN_VERSION_MAJ	2
#define PLUGIN_VERSION_MIN	0
#endif

#define PLUGIN_UNIQID		"JSBMPBOT"
#define INFO_AUTHORS		"BMP BOT PLUGIN"
#define INFO_EMAIL			"mailto:bmp@live.jp"
#define INFO_HTTP			"http://www.bmpscript.com"
#define INFO_DESCRIPTION	"\"DEMO\" plugin"


#define CONFIG_FILENAME		"bmp.conf"
#define MAX_MSG_LENGTH		30

#define CMD_VERSION			"/VER"

#define MSG_HEAD				"\t\tVersion plugin commands\n"
#define MSG_VERSION_HELP		"/ver - plugin version"
