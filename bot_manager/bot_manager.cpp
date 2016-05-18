//
//
#include "bot_manager.h"
#include "../defines/defines.h"
#include "../bot_packet/bot_packet.h"
#include "../xml_interface/xml_interface.h"


std::string to_upper_copy(const std::string& s)
{
	std::string tmp(s);
	std::string::iterator i = tmp.begin();
	std::string::iterator end = tmp.end();

	while (i != end) {
		*i = toupper((unsigned char)*i);
		++i;
	}

	return tmp;
}


bool case_insensitive_cmp::operator () (const std::string &left, const std::string &right) const
{

	if (strcmp(to_upper_copy(left).c_str(), to_upper_copy(right).c_str()) < 0)
		return true;
	else
		return false;
	
}

bool bot_manager::startup(std::string &init_data)
{
#ifdef WIN32
	nodes = new xml_core(this);
	return nodes->startup(init_data);
#endif

	return true;
}


bot_manager::bot_manager(CFBOT_CALLBACKS *callbacks, void* context)
{
	memcpy(&_host, callbacks, sizeof(CFBOT_CALLBACKS));
	_context = context;
}

bot_manager::~bot_manager()
{

}

bool bot_manager::add_bot(const char* bot_name, const char* work_dir)
{
	std::map <std::string, bot_manager_item*, case_insensitive_cmp> ::iterator it = _bot_items.find(std::string(bot_name));
	if (it != _bot_items.end())
		return false;

	bot_manager_item *item = new bot_manager_item(this, bot_name, work_dir);
	if (!item->startup())
	{
		delete item;
		return false;
	}

	try
	{
		_bot_items.insert(std::map <std::string, bot_manager_item*, case_insensitive_cmp> ::value_type(std::string(bot_name), item));
	} catch (...) {
		item->cleanup();
		
		delete item; item = NULL;
		return false;
	}

	return true;
}

void bot_manager::remove_bot(const char* bot_name)
{
	std::map <std::string, bot_manager_item*, case_insensitive_cmp> ::iterator it = _bot_items.find(std::string(bot_name));
	if (it != _bot_items.end())
	{
		it->second->cleanup();
		delete it->second;

		_bot_items.erase(it);
	}
}

void bot_manager::on_process_data(const char* bot_name, char* event, int size)
{
	std::map <std::string, bot_manager_item*, case_insensitive_cmp> ::iterator it = _bot_items.find(std::string(bot_name));
	if (it != _bot_items.end())
	{
		bot_exchange_format p;
		if (p.receive((u_char*)event, (u_long)size))
		{
			if (!it->second->disable)
				it->second->on_push_event(p);
		}
	}
}

void bot_manager::deliver_event(const char* bot_name, const char* event, int event_size)
{
	_host.cfbot_pushevent(_context, PLUGIN_UNIQID, (char*)bot_name, (char*)event, event_size);
}

void bot_manager::cleanup()
{

}
