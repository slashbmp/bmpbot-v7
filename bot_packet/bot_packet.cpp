//
//

#include "bot_packet.h"


bot_value::bot_value()
{
	iid = -1;
}

bot_value::bot_value(int ival, u_char val)
{
	u_char d = val;
	obj.clear();
	obj.append((char *)&d, sizeof(u_char));
	iid = ival;
}

bot_value::bot_value(int ival, unsigned long val)
{
	unsigned long d = val;
	obj.clear();
	obj.append((char *)&d, sizeof(unsigned long));
	iid = ival;
}

bot_value::bot_value(int ival, int val)
{
	int d = val;
	obj.clear();
	obj.append((char *)&d, sizeof(int));
	iid = ival;
}

bot_value::bot_value(int ival, int64_t val)
{
	int64_t d = val;
	obj.clear();
	obj.append((char *)&d, sizeof(int64_t));
	iid = ival;
}

bot_value::bot_value(int ival, char *val)
{
	iid = ival;
	obj.clear();
	obj.append((char *)val, strlen(val));
}

bot_value::bot_value(int ival, std::string val)
{
	iid = ival;
	obj = val;
}

bot_value::operator std::string()
{
	return obj;
}

bot_value::operator unsigned char()
{
	if (obj.size() == sizeof(u_char))
	{
		u_char r;
		memcpy(&r, obj.data(), sizeof(u_char));
		return r;
	} else {
		return 0;
	}
}

bot_value::operator unsigned long()
{
	if (obj.size() == sizeof(unsigned long))
	{
		unsigned long r;
		memcpy(&r, obj.data(), sizeof(unsigned long));
		return r;
	} else {
		return 0;
	}
}

bot_value::operator int()
{
	if (obj.size() == sizeof(int))
	{
		unsigned long r;
		memcpy(&r, obj.data(), sizeof(int));
		return r;
	} else {
		return 0;
	}
}

bot_value::operator int64_t()
{
	if (obj.size() == sizeof(int64_t))
	{
		int64_t r;
		memcpy(&r, obj.data(), sizeof(int64_t));
		return r;
	} else {
		return 0;
	}
}

unsigned long bot_value::size()
{
	return (unsigned long)obj.size();
}

const char *bot_value::data()
{
	return obj.data();
}





bot_exchange_format::bot_exchange_format()
{
	pid = -1;
}

bot_exchange_format::bot_exchange_format(int pval)
{
	pid = pval;
}

bot_exchange_format::~bot_exchange_format()
{

}

bool bot_exchange_format::receive(u_char *data, unsigned long size)
{
	_data.clear();

	u_char *ptr = data;
	unsigned long left = size;

	if (left < sizeof(int)) return false;

	memcpy(&pid, ptr, sizeof(int));
	ptr += sizeof(int); left -= sizeof(int);

	while (left > 0)
	{
		if (left < sizeof(int)) break;
		int idx;

		memcpy(&idx, ptr, sizeof(int));
		ptr += sizeof(int); left -= sizeof(int);

		if (left < sizeof(unsigned long)) break;
		unsigned long l;

		memcpy(&l, ptr, sizeof(unsigned long));
		ptr += sizeof(unsigned long); left -= sizeof(unsigned long);

		if (!l) break;

		if (left < l) break;
		std::string data_t;
		data_t.append((char *)ptr, l);
		ptr += l; left -= l;		

		_data.insert(std::map <int, bot_value>::value_type(idx, bot_value(idx, data_t)));
	}

	return true;
}

bool bot_exchange_format::receive(std::string data)
{
	return receive((u_char *)data.data(), (unsigned long)data.size());
}

std::string bot_exchange_format::data()
{
	std::string reply;
	reply.append((char *)&pid, sizeof(int));

	for (std::map <int, bot_value>::iterator it = _data.begin(); it != _data.end(); ++it)
	{
		int iid = it->first;
		bot_value iis = it->second;
		unsigned long isz = iis.size();

		if (isz)
		{
			reply.append((char *)&iid, sizeof(int));
			reply.append((char *)&isz, sizeof(unsigned long));
			reply.append(iis.data(), isz);
		}
	}

	return reply;
}

int bot_exchange_format::size()
{
	return (int)_data.size();
}

bot_value bot_exchange_format::operator[](int iid)
{
	std::map <int, bot_value>::iterator it = _data.find(iid);
	if (it != _data.end())
		return it->second;
	else
		return bot_value();
}

bot_exchange_format& bot_exchange_format::operator<<(bot_value val)
{
	_data.insert(std::map <int, bot_value>::value_type(val.iid, val));
	return *this;
}
