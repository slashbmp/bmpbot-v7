//
//
#include <string.h>
#include "bot_manager/bot_manager.h"
#include "defines/defines.h"
#include "xml_interface/xml_interface.h"

bot_manager *bot_mngr;

extern "C"
{
	int cfbot_initialize(CFBOT_CALLBACKS *callbacks, void *context)
	{
		try
		{
			bot_mngr = new bot_manager(callbacks, context);
		} catch (...) {
			return 0;
		}

		std::string init_data;

		if (!bot_mngr->startup(init_data))
		{
			delete bot_mngr;
			return 0;
		}

		callbacks->cfbot_subscribe(context, PLUGIN_UNIQID, CF_BOT_ROOM_EVENTS | CF_BOT_ROOM_TIMER | CF_BOT_SERVICE_EVENTS | CF_BOT_IM_EVENTS | CF_BOT_2_BOT_EVENTS);

#ifdef WIN32
		callbacks->cfbot_init_settings(context, PLUGIN_UNIQID, (char*)init_data.c_str(), (int)init_data.size());
#endif

		return 1;
	}

	void cfbot_finalize()
	{
		if (bot_mngr)
			delete bot_mngr;

		bot_mngr = NULL;
	}

	int cfbot_add_client(char* bot_name, char* work_dir)
	{
		if (bot_mngr)
		{
			return bot_mngr->add_bot(bot_name, work_dir);
		}

		return 0;
	}

	void cfbot_remove_client(char* bot_name)
	{
		if (bot_mngr)
		{
			bot_mngr->remove_bot(bot_name);
		}
	}

	void cfbot_plugininformation(CFBOT_PLUGININFORMATION *info)
	{
#ifdef WIN32
		strcpy_s(info->uniqid, 9, PLUGIN_UNIQID);
		info->pluginversion = CF_BOT_VERSION(PLUGIN_VERSION_MAJ, PLUGIN_VERSION_MIN);
		info->desired_botversion = CF_BOT_VERSION(PLUGIN_VERSION_MAJ, PLUGIN_VERSION_MIN);

		strcpy_s(info->plugindescription, 512, INFO_DESCRIPTION);
		strcpy_s(info->authors, 512, INFO_AUTHORS);
		strcpy_s(info->email, 512, INFO_EMAIL);
		strcpy_s(info->http, 512, INFO_HTTP);
#else
		strcpy(info->uniqid, PLUGIN_UNIQID);
		info->pluginversion = CF_BOT_VERSION(PLUGIN_VERSION_MAJ, PLUGIN_VERSION_MIN);
		info->desired_botversion = CF_BOT_VERSION(5, 0);

		strcpy(info->plugindescription, INFO_DESCRIPTION);
		strcpy(info->authors, INFO_AUTHORS);
		strcpy(info->email, INFO_EMAIL);
		strcpy(info->http, INFO_HTTP);
#endif
	}

	void cfbot_pushevent(char* bot_name, char *event_data, int event_size)
	{
		if (bot_mngr)
		{
			bot_mngr->on_process_data(bot_name, event_data, event_size);
		}
	}
};
