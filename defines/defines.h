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

#define PLUGIN_UNIQID		"CSLLCSPL"
#define INFO_AUTHORS		"Sample plugin"
#define INFO_EMAIL			"mailto:sampleplugin@sampledomain.com"
#define INFO_HTTP			"http://www.camfrog.com"
#define INFO_DESCRIPTION	"\"Sample version\" plugin"


#define CONFIG_FILENAME		"sample.conf"
#define MAX_MSG_LENGTH		30

#define CMD_VERSION			"/VER"

#define MSG_HEAD				"\t\tVersion plugin commands\n"
#define MSG_VERSION_HELP		"/ver - plugin version"
