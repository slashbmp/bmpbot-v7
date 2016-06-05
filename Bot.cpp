#include <Windows.h>
#include "Bot.h"
#include "bot_packet/bot_packet.h"
#include <Shlobj.h>
#include <fstream>

#ifndef WIN32

#include <ctype.h>

void strupr(char *s)
{
	while (*s) {
		*s = toupper((unsigned char)*s);
		s++;
	}
}

#endif

std::string ExePath() {
	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	return std::string(buffer).substr(0, pos);
}

inline bool FileExists(const std::string& name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

inline bool EnsureDirExists(const wchar_t* fullDirPath)
{
	HWND hwnd = NULL;
	const SECURITY_ATTRIBUTES *psa = NULL;
	TString dir = fullDirPath;
	int retval = SHCreateDirectoryEx(hwnd, dir.GetAsChar(), psa);
	if (retval == ERROR_SUCCESS || retval == ERROR_FILE_EXISTS || retval == ERROR_ALREADY_EXISTS) return true;

	return false;
}

bool fexists(TString& fileName) {
	std::ifstream ifile(fileName.GetAsChar());
	return (bool)ifile;
}

int h2a1(const wchar_t chr) {
	if (chr == L'1') return 1;
	else if (chr == L'2') return 2;
	else if (chr == L'3') return 3;
	else if (chr == L'4') return 4;
	else if (chr == L'5') return 5;
	else if (chr == L'6') return 6;
	else if (chr == L'7') return 7;
	else if (chr == L'8') return 8;
	else if (chr == L'9') return 9;
	else if (chr == L'a' || chr == L'A') return 10;
	else if (chr == L'b' || chr == L'B') return 11;
	else if (chr == L'c' || chr == L'C') return 12;
	else if (chr == L'd' || chr == L'D') return 13;
	else if (chr == L'e' || chr == L'E') return 14;
	else if (chr == L'f' || chr == L'F') return 15;
	return 0;
}

int h2a2(const wchar_t chr[2]) {
	int nOut = h2a1(chr[0]) * 16;
	nOut += h2a1(chr[1]);
	return nOut;
}

DWORD GetCol(const wchar_t* str) {
	int i = 0;
	TString strChk = str;
	if (strChk.GetLength() < 6) return 0;
	if (strChk.GetAt(i) == L'#') i++;
	wchar_t r[2], g[2], b[2];
	r[0] = strChk.GetAt(i++);
	r[1] = strChk.GetAt(i++);
	g[0] = strChk.GetAt(i++);
	g[1] = strChk.GetAt(i++);
	b[0] = strChk.GetAt(i++);
	b[1] = strChk.GetAt(i);
	int rr = h2a2(r);
	int gg = h2a2(g);
	int bb = h2a2(b);
	return RGB(rr, gg, bb);
}

Bot* _activeBot;

static enum v7_err jsBotLoadFile(struct v7* v7, v7_val_t* res) {
	v7_val_t file = v7_arg(v7, 0);
	if (!v7_is_string(file)) {
		return V7_OK;
	}
	TString fullPath = _activeBot->getWorkDir();
	TString fileName = v7_to_cstring(v7, &file);
	fullPath += fileName.GetAsWChar();
	if (!fexists(fullPath)) {
		return V7_OK;
	}
	v7_val_t result;
	enum v7_err ress = V7_OK;
	ress = v7_exec_file(v7, fullPath.GetAsChar(), &result);

	//v7_val_t* v7Obj = _activeBot->getV7Obj();
	//*v7Obj = v7_mk_object(v7);

	if (ress != V7_OK) {
		if (result == V7_SYNTAX_ERROR) MessageBox(NULL, "script fail syntax error", "Nooo", 0);
		else if (result == V7_EXEC_EXCEPTION) MessageBox(NULL, "script fail, exception", "Nooo", 0);
		else if (result == V7_EXEC_EXCEPTION) MessageBox(NULL, "script fail, exception", "Nooo", 0);
		else if (result == V7_AST_TOO_LARGE) MessageBox(NULL, "script fail, ast too large", "Nooo", 0);
	}

	return V7_OK;
}

static enum v7_err jsBotFontStyle(struct v7* v7, v7_val_t* res) {
	v7_val_t fontName = v7_arg(v7, 0);
	v7_val_t fontSize = v7_arg(v7, 1);
	v7_val_t fontColor = v7_arg(v7, 2);
	if (!v7_is_string(fontName) || !v7_is_number(fontSize) || !v7_is_string(fontColor)) {
		return V7_OK;
	}
	TString fontColor2 = v7_to_cstring(v7, &fontColor);
	TString strFontName = (v7_is_null(fontName)) ? "" : (const char*)v7_to_cstring(v7, &fontName);
	int nFontSize = (!v7_is_number(fontSize)) ? NULL : (int)v7_to_number(fontSize);
	COLORREF nFontColor = (!v7_is_string(fontColor)) ? NULL : GetCol(fontColor2.GetAsWChar());
	_activeBot->setFontStyle(strFontName, nFontSize, nFontColor);
	return V7_OK;
}

static enum v7_err jsBotEvent(struct v7* v7, v7_val_t* res) {
	v7_val_t val = v7_arg(v7, 0);
	TString strEvent = (const char*)v7_to_cstring(v7, &val);
	v7_val_t* func = new v7_val_t;
	v7_own(v7, func);
	*func = v7_arg(v7, 1);
	
	//if not a function type in 2nd argument
	if (!v7_is_callable(v7, *func)) {
		//TODO: throw error
		return V7_OK;
	}
	else {

	}

	if (strEvent.IsEQNC(L"text")) {
		_activeBot->_onTexts.Add(func);
	}
	else if (strEvent.IsEQNC(L"join")) {
		_activeBot->_onJoin.Add(func);
	}
	else if (strEvent.IsEQNC(L"left")) {
		_activeBot->_onLeft.Add(func);
	}
	else if (strEvent.IsEQNC(L"talk")) {
		_activeBot->_onTalk.Add(func);
	}
	else if (strEvent.IsEQNC(L"motd")) {
		_activeBot->_onMotd.Add(func);
	}
	else if (strEvent.IsEQNC(L"im")) {
		_activeBot->_onIm.Add(func);
	}
	return V7_OK;
}

static enum v7_err jsBotSay(struct v7 *v7, v7_val_t *res) {
	long argc = v7_argc(v7);
	if (argc == 0) {
		return V7_OK;
	}
	v7_val_t v = v7_arg(v7, 0);
	if (!v7_is_string(v)) {
		return V7_OK;
	}
	TString str = (const char*)v7_to_cstring(v7, &v);
	TString font;
	if (argc >= 2) {
		v7_val_t fontt = v7_arg(v7, 1);
		if (!v7_is_null(fontt) && !v7_is_undefined(fontt)) font = (const char*)v7_to_cstring(v7, &fontt);
	}
	int size = 0;
	if (argc >= 3) {
		v7_val_t sizee = v7_arg(v7, 2);
		if (v7_is_number(sizee)) size = floor(v7_to_number(sizee));
		if (size < 10) size = 10;
	}
	TString col;
	if (argc >= 4) {
		v7_val_t coll = v7_arg(v7, 3);
		if (v7_is_string(coll)) col = (const char*)v7_to_cstring(v7, &coll);
		if (col.GetAt(0) != L'#' || col.GetLength() != 7) col = L"";
	}
	_activeBot->say(str, font, size, col);
	return V7_OK;
}

static enum v7_err jsSetInterval(struct v7* v7, v7_val_t* res) {
	v7_val_t* func = new v7_val_t;
	v7_own(v7, func);
	*func = v7_arg(v7, 0);
	v7_val_t inv = v7_arg(v7, 1);

	if (!v7_is_callable(v7, *func)) {
		//TODO: catch error if 1st argument not callable code
		return V7_OK;
	}

	if (!v7_is_number(inv)) {
		//TODO: catch error if 2nd argument not a number
		return V7_OK;
	}

	double ms = v7_to_number(inv);
	long lms = floor(ms);

	//v7_apply(v7, func, _activeBot->getV7Obj(), v7_mk_undefined(), NULL);

	TIMER* newTimer = new TIMER;
	newTimer->pToFunc = func;
	newTimer->delay = lms;
	newTimer->timeBegin = GetTickCount();
	newTimer->repeat = 0;
	newTimer->id = (long)newTimer;

	_activeBot->_timers.Add(newTimer);

	double vOut = (double)newTimer->id;

	*res = v7_mk_number(vOut);

	return V7_OK;
}

static enum v7_err jsSetTimeout(struct v7* v7, v7_val_t* res) {
	v7_val_t* func = new v7_val_t;
	v7_own(v7, func);
	*func = v7_arg(v7, 0);
	v7_val_t inv = v7_arg(v7, 1);

	if (!v7_is_callable(v7, *func)) {
		//TODO: catch error if 1st argument not callable code
		return V7_OK;
	}

	if (!v7_is_number(inv)) {
		//TODO: catch error if 2nd argument not a number
		return V7_OK;
	}

	double ms = v7_to_number(inv);
	long lms = floor(ms);

	//v7_apply(v7, func, _activeBot->getV7Obj(), v7_mk_undefined(), NULL);

	TIMER* newTimer = new TIMER;
	newTimer->pToFunc = func;
	newTimer->delay = lms;
	newTimer->timeBegin = GetTickCount();
	newTimer->id = (long)newTimer;

	_activeBot->_timers.Add(newTimer);

	double vOut = (double)newTimer->id;

	*res = v7_mk_number(vOut);

	return V7_OK;
}

static enum v7_err jsClearTimeout(struct v7* v7, v7_val_t* res) {
	v7_val_t ind = v7_arg(v7, 0);
	if (!v7_is_number(ind)) {
		//TODO: throw error
		_activeBot->say("Invalid timer key, (not a number)");
		return V7_OK;
	}

	long n = (long)floor(v7_to_number(ind));

	TIMER* p = (TIMER*)n;

	v7_disown(v7, (v7_val_t*)p->pToFunc);
	delete (v7_val_t*)p->pToFunc;
	_activeBot->_timers.Del(p);
	delete p;

	return V7_OK;
}

static enum v7_err jsClearInterval(struct v7* v7, v7_val_t* res) {
	v7_val_t ind = v7_arg(v7, 0);
	if (!v7_is_number(ind)) {
		//TODO: throw error
		_activeBot->say("Invalid timer key, (not a number)");
		return V7_OK;
	}

	long n = (long)floor(v7_to_number(ind));

	TIMER* p = (TIMER*)n;

	v7_disown(v7, (v7_val_t*)p->pToFunc);
	delete (v7_val_t*)p->pToFunc;
	_activeBot->_timers.Del(p);
	delete p;

	return V7_OK;
}

Bot::Bot(bot_manager* mgr, const char* name, const char* workDir) {

	_mgr = mgr;
	_name = name;
	/*_workDir = std::string(ExePath()).c_str();
	_workDir += L"\\plugins\\BMP\\";
	TString botNamee = name;
	botNamee.ToUpper();
	_workDir += botNamee.GetAsWChar();

	MessageBox(NULL, _workDir.GetAsChar(), "Work Dir", 0);*/

	setActive();

	_workDir = workDir;
	_workDir += L"bmp\\";

	bool isScriptOk = true;
	if (!EnsureDirExists(_workDir.GetAsWChar())) {
		isScriptOk = false;
	}

	_charset.color = RGB(0xff, 0x99, 0x00);
	_activeBot = this;

	_scriptFile = _workDir.GetAsWChar();
	_scriptFile += L"main.js";

	if (isScriptOk && !FileExists(std::string(_scriptFile.GetAsChar()))) {
		TString strSay = L"BMP Plugin will not work, please put file 'main.js' into work dir. (";
		strSay += _scriptFile.GetAsWChar();
		strSay += L")";
		//say(strSay);
		MessageBox(NULL, strSay.GetAsChar(), "BMP Bot Alert", 0);
		isScriptOk = false;
	}

	v7 = v7_create();
	enum v7_err rcode = V7_OK;
	v7_val_t result;

	//events function define
	v7_set_method(v7, v7_get_global(v7), "on", (v7_cfunction_t*)&jsBotEvent);

	//utilities functions
	v7_set_method(v7, v7_get_global(v7), "say", (v7_cfunction_t*)&jsBotSay);
	v7_set_method(v7, v7_get_global(v7), "setTimeout", (v7_cfunction_t*)&jsSetTimeout);
	v7_set_method(v7, v7_get_global(v7), "setInterval", (v7_cfunction_t*)&jsSetInterval);
	v7_set_method(v7, v7_get_global(v7), "clearTimeout", (v7_cfunction_t*)&jsClearTimeout);
	v7_set_method(v7, v7_get_global(v7), "clearInterval", (v7_cfunction_t*)&jsClearInterval);

	botObj = new v7_val_t;
	v7_own(v7, botObj);
	*botObj = v7_mk_object(v7);
	v7_val_t botName = v7_mk_string(v7, name, ~0, 1);
	v7_set(v7, *botObj, "name", ~0, botName);
	v7_set(v7, v7_get_global(v7), "BOT", ~0, *botObj);
	v7_set_method(v7, *botObj, "setFontStyle", (v7_cfunction_t*)&jsBotFontStyle);
	v7_set_method(v7, *botObj, "load", (v7_cfunction_t*)&jsBotLoadFile);

	//rcode = v7_exec(v7, "var a = \"aaa\"; say(a); print(\"Yeah\");", &result);
	if (isScriptOk) {
		rcode = v7_exec_file(v7, _scriptFile.GetAsChar(), &result);

		if (rcode != V7_OK) {
			if (result == V7_SYNTAX_ERROR) MessageBox(NULL, "script fail syntax error", "Nooo", 0);
			else if (result == V7_EXEC_EXCEPTION) MessageBox(NULL, "script fail, exception", "Nooo", 0);
			else if (result == V7_EXEC_EXCEPTION) MessageBox(NULL, "script fail, exception", "Nooo", 0);
			else if (result == V7_AST_TOO_LARGE) MessageBox(NULL, "script fail, ast too large", "Nooo", 0);
		}
		else {
			v7_val_t vObj;
			v7_own(v7, &vObj);
			vObj = v7_mk_object(v7);
			v7Obj = &vObj;
		}
	}

}

Bot::~Bot()
{
	long i, l;
	TIMER* p;
	l = _timers.GetLength();
	for (i = 0; i < l; i++) {
		p = (TIMER*)_timers.GetAt(i);
		v7_disown(v7, (v7_val_t*)p->pToFunc);
		delete (v7_val_t*)p->pToFunc;
		delete p;
	}

	l = _onTexts.GetLength();
	for (i = 0; i < l; i++) {
		v7_disown(v7, (v7_val_t*)_onTexts.GetAt(i));
		delete (v7_val_t*)_onTexts.GetAt(i);
	}

	l = _onJoin.GetLength();
	for (i = 0; i < l; i++) {
		v7_disown(v7, (v7_val_t*)_onJoin.GetAt(i));
		delete (v7_val_t*)_onJoin.GetAt(i);
	}

	l = _onLeft.GetLength();
	for (i = 0; i < l; i++) {
		v7_disown(v7, (v7_val_t*)_onLeft.GetAt(i));
		delete (v7_val_t*)_onLeft.GetAt(i);
	}

	l = _onTalk.GetLength();
	for (i = 0; i < l; i++) {
		v7_disown(v7, (v7_val_t*)_onTalk.GetAt(i));
		delete (v7_val_t*)_onTalk.GetAt(i);
	}

	l = _onMotd.GetLength();
	for (i = 0; i < l; i++) {
		v7_disown(v7, (v7_val_t*)_onMotd.GetAt(i));
		delete (v7_val_t*)_onMotd.GetAt(i);
	}
	
	l = _onIm.GetLength();
	for (i = 0; i < l; i++) {
		v7_disown(v7, (v7_val_t*)_onIm.GetAt(i));
		delete (v7_val_t*)_onIm.GetAt(i);
	}

	v7_disown(v7, botObj);
	v7_disown(v7, v7Obj);

	v7_destroy(v7);
}

void Bot::setActive() {
	_activeBot = this;
}

void Bot::say(std::string str) {
	TString tstr = str.c_str();
	TString font;
	int size = 0;
	TString col;
	say(tstr, font, size, col);
}

void Bot::say(const char* str) {
	TString tstr = str;
	TString font;
	int size = 0;
	TString col;
	say(tstr, font, size, col);
}

void Bot::say(const wchar_t* str) {
	TString strOut = str;
	TString font;
	int size = 0;
	TString col;
	say(strOut, font, size, col);
}

void Bot::say(TString& text) {
	TString font;
	int size = 0;
	TString col;
	say(text, font, size, col);
}

void Bot::say(TString& text, TString& font, unsigned char fontSize, TString& color) {

	if (font.IsEQ(L"")) font = _charset.font;
	if (fontSize < 10) fontSize = _charset.size;
	COLORREF coll = _charset.color;
	if (!color.IsEQ(L"")) coll = GetCol(color.GetAsWChar());

	bot_exchange_format p(PLUGIN_EVENT_ROOM_TEXT);
	p << bot_value(1, text.GetAsChar());
	p << bot_value(2, _charset.attributes);
	p << bot_value(3, fontSize);
	p << bot_value(4, coll);
	p << bot_value(5, _charset.effect);
	p << bot_value(6, _charset.charset);
	p << bot_value(7, _charset.pitch);
	p << bot_value(8, font.GetAsChar());

	std::string d = p.data();

	_mgr->deliver_event(_name.GetAsChar(), d.c_str(), (int)d.size());
}

struct v7* Bot::getV7() {
	return v7;
}

v7_val_t* Bot::getV7Obj() {
	return v7Obj;
}

const wchar_t* Bot::getWorkDir() {
	return _workDir.GetAsWChar();
}

void Bot::setFontStyle(TString& fontName, int fontSize, COLORREF fontColor) {
	if (!fontName.IsEQ(L"")) memcpy(_charset.font, fontName.GetAsChar(), fontName.GetLength() * sizeof(char));
	if (fontSize != NULL) _charset.size = fontSize;
	if (fontColor != NULL) _charset.color = fontColor;
}

void Bot::onIm(TString& nick, TString& text) {
	v7_val_t theNick = v7_mk_string(v7, nick.GetAsChar(), ~0, 1);
	v7_val_t theText = v7_mk_string(v7, text.GetAsChar(), ~0, 1);
	v7_val_t* func;
	v7_val_t args;

	long i;
	long l = _onMotd.GetLength();

	for (i = 0; i < l; i++) {
		func = (v7_val_t*)_onIm.GetAt(i);
		args = v7_mk_array(v7);
		v7_array_push(v7, args, theNick);
		v7_array_push(v7, args, theText);

		v7_apply(v7, *func, *v7Obj, args, NULL);
	}
}

/*void Bot::onText(TString& nick, TString& text) {
	long i, l;
	l = _onTexts.GetLength();
	v7_val_t nickName = v7_mk_string(v7, nick.GetAsChar(), ~0, 0);
	v7_val_t fullText = v7_mk_string(v7, text.GetAsChar(), ~0, 0);
	for (i = 0; i < l; i++) {
		v7_val_t* func = (v7_val_t*)_onTexts.GetAt(i);
		v7_val_t args;
		args = v7_mk_array(v7);
		v7_array_push(v7, args, nickName);
		v7_array_push(v7, args, fullText);
		v7_apply(v7, *func, *func, args, NULL);

	}
}*/

void Bot::onText(bot_exchange_format& f) {
	long i, l;
	l = _onTexts.GetLength();
	std::string nick = std::string(f[1]);
	std::string text = std::string(f[2]);

	v7_val_t attr;

	if ((u_char)f[3] == 0) {
		attr = v7_mk_undefined();
	}
	else {
		attr = v7_mk_object(v7);

		v7_val_t size = v7_mk_number((u_char)f[4]);
		v7_set(v7, attr, "size", ~0, size);

		COLORREF col = (u_long)f[5];
		int r = GetRValue(col);
		int g = GetGValue(col);
		int b = GetBValue(col);
		char strCol[8];
		sprintf(strCol, "#%02X%02X%02X", r, g, b);
		v7_val_t color = v7_mk_string(v7, strCol, ~0, 1);
		v7_set(v7, attr, "color", ~0, color);

		v7_val_t effects = v7_mk_number((u_long)f[6]);
		v7_set(v7, attr, "effects", ~0, effects);

		v7_val_t charset = v7_mk_number((u_char)f[7]);
		v7_set(v7, attr, "charset", ~0, charset);

		v7_val_t pitch = v7_mk_number((u_char)f[8]);
		v7_set(v7, attr, "pitch", ~0, pitch);

		std::string font = std::string(f[9]);
		v7_val_t fontName = v7_mk_string(v7, font.c_str(), ~0, 1);
		v7_set(v7, attr, "font", ~0, fontName);
	}

	v7_val_t nickName = v7_mk_string(v7, nick.c_str(), ~0, 0);
	v7_val_t fullText = v7_mk_string(v7, text.c_str(), ~0, 0);
	for (i = 0; i < l; i++) {
		v7_val_t* func = (v7_val_t*)_onTexts.GetAt(i);
		v7_val_t args;
		args = v7_mk_array(v7);
		v7_array_push(v7, args, nickName);
		v7_array_push(v7, args, fullText);
		v7_array_push(v7, args, attr);
		v7_apply(v7, *func, *v7Obj, args, NULL);

	}
}

void Bot::onTimer() {
	long i, p, chk, chk2, timeNow;
	TIMER* tmr;
	for (i = 0; i < _timers.GetLength(); i++) {
		tmr = (TIMER*)_timers.GetAt(i);
		timeNow = GetTickCount();
		chk = timeNow - tmr->timeBegin;
		if (chk >= tmr->delay) {
			v7_apply(v7, *((v7_val_t*)tmr->pToFunc), *v7Obj, v7_mk_undefined(), NULL);
			if (tmr->repeat != 0) {
				_timers.DelAt(i--);
				v7_disown(v7, (v7_val_t*)tmr->pToFunc);
				delete (v7_val_t*)tmr->pToFunc;
				delete tmr;
			}
			else {
				tmr->timeBegin = timeNow - (chk - tmr->delay);
			}
		}
	}
}

void Bot::onJoin(bot_exchange_format& f) {
	
	long l = _onJoin.GetLength();
	if (l == 0) return;

	v7_val_t user = v7_mk_object(v7);

	v7_val_t userName = v7_mk_string(v7, std::string(f[1]).c_str(), ~0, 1);
	v7_set(v7, user, "name", ~0, userName);

	v7_val_t flags = v7_mk_number((u_long)f[2]);
	v7_set(v7, user, "flags", ~0, flags);

	v7_val_t age = v7_mk_number((u_char)f[3]);
	v7_set(v7, user, "age", ~0, age);

	v7_val_t count = v7_mk_number((int)f[4]);
	v7_set(v7, user, "count", ~0, count);
	
	v7_val_t gifts = v7_mk_number((u_long)f[5]);
	v7_set(v7, user, "gifts", ~0, gifts);

	long i;
	v7_val_t* func;
	v7_val_t args;
	for (i = 0; i < l; i++) {
		func = (v7_val_t*)_onJoin.GetAt(i);
		args = v7_mk_array(v7);
		v7_array_push(v7, args, user);

		v7_apply(v7, *func, *v7Obj, args, NULL);
	}
}

void Bot::onLeft(TString& nick) {
	long l = _onLeft.GetLength();
	if (l == 0) return;

	long i;
	v7_val_t* func;
	v7_val_t args;
	v7_val_t userName = v7_mk_string(v7, nick.GetAsChar(), ~0, 1);
	for (i = 0; i < l; i++) {
		func = (v7_val_t*)_onLeft.GetAt(i);
		args = v7_mk_array(v7);
		v7_array_push(v7, args, userName);

		v7_apply(v7, *func, *v7Obj, args, NULL);
	}
}

void Bot::onTalk(TString& nick, unsigned char flag) {
	long l = _onTalk.GetLength();
	if (l == 0) return;

	long i;
	v7_val_t* func;
	v7_val_t args;
	v7_val_t userName = v7_mk_string(v7, nick.GetAsChar(), ~0, 1);
	v7_val_t theFlag = v7_mk_number(flag);

	for (i = 0; i < l; i++) {
		func = (v7_val_t*)_onTalk.GetAt(i);
		args = v7_mk_array(v7);
		v7_array_push(v7, args, userName);
		v7_array_push(v7, args, theFlag);

		v7_apply(v7, *func, *v7Obj, args, NULL);
	}
}

void Bot::onMotd(TString& motd) {
	v7_val_t theMotd = v7_mk_string(v7, motd.GetAsChar(), ~0, 1);
	v7_val_t* func;
	v7_val_t args;

	long i;
	long l = _onMotd.GetLength();

	for (i = 0; i < l; i++) {
		func = (v7_val_t*)_onMotd.GetAt(i);
		args = v7_mk_array(v7);
		v7_array_push(v7, args, theMotd);

		v7_apply(v7, *func, *v7Obj, args, NULL);
	}
}