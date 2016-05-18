#pragma once
#include <stdlib.h>
#include <string>

#include "defines/bot_api.h"
#include "BMP/TString.h"

class bot_manager;
#include "bot_manager/bot_manager.h"

typedef struct MYCHARSET {
	BYTE attributes;
	BYTE size;
	COLORREF color;
	DWORD effect;
	DWORD charset;
	BYTE pitch;
	char font[50];
	MYCHARSET() {
		attributes = 1;
		size = 14;
		color = 0;
		effect = 0;
		charset = 0;
		pitch = 0;
		strcpy_s(font, 49, "Tahoma");
	}
} MYCHARSET;

class Bot
{
public:
	Bot();
	Bot(bot_manager*, const char*, const char*);
	~Bot();
	void say(std::string);
	void say(const char*);
	void say(const wchar_t*);
private:
	bot_manager* _mgr;
	TString _name;
	TString _workDir;
	MYCHARSET _charset;
};

