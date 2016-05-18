//
//
#ifdef WIN32

#pragma once

class xml_core;
#include "../bot_manager/bot_manager.h"

class xml_core
{
public:
	xml_core(bot_manager *mngr);
	~xml_core();

	bool startup(std::string &init_data);
	void cleanup();

	void process_settings_wnd(std::string &data, std::string &message);
	void process_state(std::string &out_data, std::string in_data, std::string &message);

private:
	bot_manager *_mngr;
};

#endif
