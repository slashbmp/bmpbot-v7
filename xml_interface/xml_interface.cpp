//
//
#ifdef WIN32

#include <boost/property_tree/detail/rapidxml.hpp>

#include "xml_interface.h"
#include "../defines/defines.h"

using namespace boost::property_tree::detail;

const char* str_xml =
	"<Form Id = 'sample'>"
	"<Label Id = 'label1' X = '20' Y = '30' Width = '200' Height = '20'>Plugin UNIQUID: JSBMPBOT</Label>"
	"<Label Id = 'label2' X = '20' Y = '60' Width = '200' Height = '20'>JS plugin by BMP</Label>"
	"<Label Id = 'label3' X = '20' Y = '90' Width = '200' Height = '20'>mailto:bmp@live.jp</Label>"
	"<Label Id = 'label4' X = '20' Y = '120' Width = '200' Height = '20'>http://www.bmpscript.com</Label>"
	"<Label Id = 'label5' X = '20' Y = '160' Width = '390' Height = '20'>JS plugin by BMP</Label>"
	
	"<Edit Id = 'edit_new_text' X = '20' Y = '182' Width = '280' Height = '22' IsEnabled = 'true'></Edit>"
	"<Button Id = 'button_set' X = '320' Y = '180' Width = '70' Height = '25' IsEnabled = 'true'>Set</Button>"

	"</Form>";

const char* state_xml = 
	"<FormState Id = 'sample'>"

	"<Edit Id = 'edit_new_text' X = '20' Y = '182' Width = '280' Height = '22' IsEnabled = 'true'>%s</Edit>"
	"<Button Id = 'button_set' X = '320' Y = '180' Width = '70' Height = '25' IsEnabled = 'true'>Set</Button>"

	"</FormState>";

xml_core::xml_core(bot_manager *mngr)
{
	_mngr = mngr;
}

xml_core::~xml_core()
{

}

bool xml_core::startup(std::string &init_data)
{
	init_data = str_xml;

	std::string form_xml = str_xml;
	
	rapidxml::xml_document<> doc_v;

	try
	{
		doc_v.parse<0>(const_cast<char*>(form_xml.c_str()));
	}
	catch (...)
	{
		return false;
	}

	return true;
}

void xml_core::process_settings_wnd(std::string &data, std::string &message)
{
	char buff[2600];
	memset(buff,0,2600);
	sprintf_s(buff, 2600, state_xml,message.c_str());
	data = std::string(buff);
}

void xml_core::process_state(std::string &out_data, std::string in_data, std::string &message)
{
	rapidxml::xml_document<> doc_v;

	std::string edit_state;

	try
	{
		doc_v.parse<0>(const_cast<char*>(in_data.c_str()));
	}
	catch (...)
	{
		return;
	}

	rapidxml::xml_node<> *root = doc_v.first_node();

	if (root && !strcmp(root->name(), "FormState"))
	{
		const char* form_id = root->first_attribute()->value();
		if (form_id && !strcmp(form_id, "sample"))
		{
			rapidxml::xml_node<> *node;
			for (node = root->first_node(); node; node = node->next_sibling())
			{
				const char* control = node->name();

				if (!strcmp(control, "Edit"))
				{
					const char * id = node->first_attribute()->value();
					if (!strcmp(id, "edit_new_text"))
					{
						const char * state = node->value();
						if (state)
							message = std::string(state);
						else 
							message = std::string("");
						edit_state.clear();
						edit_state.append(message);

					}
				}

				if (!strcmp(control, "Button"))
				{
					const char * id = node->first_attribute()->value();
					if (!strcmp(id, "button_set"))
					{
						const char *state = node->value();
						if (!strcmp(state, "true"))
						{
							message = edit_state;
						}
					}
				}
			}
		}
	}

	process_settings_wnd(out_data, message);

}
#endif
