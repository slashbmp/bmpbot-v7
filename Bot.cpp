#include <Windows.h>
#include "Bot.h"
#include "bot_packet/bot_packet.h"

Bot::Bot()
{
}

Bot::Bot(bot_manager* mgr, const char* name, const char* workDir) {
	_mgr = mgr;
	_name = name;
	_workDir = workDir;

	_charset.color = RGB(0xff, 0x99, 0x00);
}

Bot::~Bot()
{
}

void Bot::say(std::string str) {
	MessageBox(NULL, str.c_str(), "Yeah", 0);
}

void Bot::say(const char* str) {
	MessageBox(NULL, str, "Yeah", 0);
}

void Bot::say(const wchar_t* str) {
	TString strOut = str;
	bot_exchange_format p(PLUGIN_EVENT_ROOM_TEXT);
	p << bot_value(1, strOut.GetAsChar());
	p << bot_value(2, _charset.attributes);
	p << bot_value(3, _charset.size);
	p << bot_value(4, _charset.color);
	p << bot_value(5, _charset.effect);
	p << bot_value(6, _charset.charset);
	p << bot_value(7, _charset.pitch);
	p << bot_value(8, _charset.font);

	std::string d = p.data();

	_mgr->deliver_event(_name.GetAsChar(), d.c_str(), (int)d.size());
}