//
//
#pragma once

// Bot2Bot internal protocol
// These packets are wrapped with 

#define BB_MODULE_INFO_REQUEST		0x01
// Module requests commands list
// Sender module ID		std::string		0x01

#define BB_MODULE_INFO_RESPONSE		0x02
// Module's commands list
// Module ID			std::string		0x01
// Module description	std::string		0x02
// Command 1			std::string		0x03
// Command 2			std::string		0x04
// ...

#define BB_PROCESS_COMMAND			0x03
// User executes command
// User nickname		std::string		0x01
// Command				std::string		0x02
// Text					std::string		0x03

// Note: If your module receives this packet it means :
//		1. that the user, who executes the command is in bot owners list;
//		2. that your represented this command in BB_MODULE_INFO_RESPONSE packet.

#define BB_SHOW_HELP				0x04
// Module should post help topic
