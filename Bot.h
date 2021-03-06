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
		size = 16;
		color = 0;
		effect = 0;
		charset = 0;
		pitch = 0;
		strcpy_s(font, 49, "Tahoma");
	}
} MYCHARSET;

typedef struct TIMER {
	void* pToFunc;
	long delay;
	long repeat;
	long count;
	long id;
	long timeBegin;
	bool isStop;
	TIMER() {
		delay = 0;
		repeat = 1;
		count = 1;
		id = 0;
		isStop = false;
		timeBegin = GetTickCount();
	}
} TIMER;

class Bot
{
public:
	Bot(bot_manager*, const char*, const char*);
	~Bot();

	void setActive();

	void say(std::string);
	void say(const char*);
	void say(const wchar_t*);
	void say(TString& text);
	void say(TString& text, TString& font, unsigned char fontSize, TString& col);
	void setFontStyle(TString& fontName, int fontSize, COLORREF col);

	struct v7* getV7();
	v7_val_t* getV7Obj();
	const wchar_t* getWorkDir();

	void onIm(TString& nick, TString& text);
	//void onText(TString& nick, TString& text);
	void onText(bot_exchange_format& f);
	void onTimer();
	void onJoin(bot_exchange_format& f);
	void onLeft(TString& nick);
	void onTalk(TString& nick, unsigned char flag);
	void onMotd(TString& motd);

	TPtrArray _timers;
	TPtrArray _onTexts;
	TPtrArray _onJoin;
	TPtrArray _onLeft;
	TPtrArray _onTalk;
	TPtrArray _onMotd;
	TPtrArray _onIm;
private:
	bot_manager* _mgr;
	TString _name;
	TString _workDir;
	TString _scriptFile;
	MYCHARSET _charset;
	struct v7* v7;
	v7_val_t* v7Obj;
	v7_val_t* botObj;
};
