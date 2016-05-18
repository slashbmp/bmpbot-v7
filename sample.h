//
//
#pragma once

#include "defines/bot_api.h"

#ifdef WIN32

int		__cdecl cfbot_initialize(CFBOT_CALLBACKS *callbacks, void *context);
void	__cdecl cfbot_finalize();

int		__cdecl cfbot_add_client(char* bot_name, char* work_dir);
void	__cdecl cfbot_remove_client(char* bot_name);

void	__cdecl cfbot_plugininformation(CFBOT_PLUGININFORMATION *info);
void	__cdecl cfbot_pushevent(char* bot_name, char *event_data, int event_size);

#else

extern "C"
{
	int	 cfbot_initialize(CFBOT_CALLBACKS *callbacks, void *context);
	void cfbot_finalize();

	int	 cfbot_add_client(char* bot_name, char* work_dir);
	void cfbot_remove_client(char* bot_name);

	void cfbot_plugininformation(CFBOT_PLUGININFORMATION *info);
	void cfbot_pushevent(char* bot_name, char *event_data, int event_size);
};

#endif
