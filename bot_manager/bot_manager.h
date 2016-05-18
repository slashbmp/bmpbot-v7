//
//
#pragma once

#include "../defines/bot_api.h"

class bot_manager;
#include "bot_manager_item.h"
#include "../xml_interface/xml_interface.h"


struct case_insensitive_cmp 
{
	bool operator () (const std::string &left, const std::string &right) const;
};


class bot_manager
{
public:
	bot_manager(CFBOT_CALLBACKS *callbacks, void* context);
	~bot_manager();

	bool startup(std::string &init_data);

	bool add_bot(const char* bot_name, const char* work_dir);
	void remove_bot(const char* bot_name);

	void on_process_data(const char* bot_name, char* event, int size);
	void deliver_event(const char* bot_name, const char* event, int event_size);
	void cleanup();

#ifdef WIN32
	xml_core *nodes;
#endif


private:

	CFBOT_CALLBACKS _host;
	void * _context;

	std::map <std::string, bot_manager_item*, case_insensitive_cmp> _bot_items;
};
