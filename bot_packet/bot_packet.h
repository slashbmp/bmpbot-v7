#pragma once
#include "../defines/defines.h"
#include <string>
#include <map>


class bot_value {
public:
	bot_value();
	bot_value(int, unsigned char);
	bot_value(int, unsigned long);
	bot_value(int, int);
	bot_value(int, int64_t);
	bot_value(int, char *);
	bot_value(int, std::string);

	operator std::string();
	operator unsigned char();
	operator unsigned long();
	operator int();
	operator int64_t();

	int iid;

	unsigned long size();
	const char *data();
private:
	std::string obj;
};


class bot_exchange_format
{
public:
	bot_exchange_format();
	bot_exchange_format(int pval);
	~bot_exchange_format();

	bool receive(u_char *data, unsigned long size);
	bool receive(std::string data);

	std::string data();
	int size();

	bot_value operator[](int iid);
	bot_exchange_format& operator<<(bot_value val);

	int pid;

protected:
	std::map <int, bot_value> _data;
};
