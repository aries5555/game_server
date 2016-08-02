/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */

#include "Public_Struct.h"
#include "Server_Config.h"
#include "Log.h"
#include "Mongo_Struct.h"
#include "Mysql_Struct.h"
#include "Log_Struct.h"
#include "Msg_Struct.h"

int load_struct(const char *path, Struct_Type struct_type, Struct_Id_Map &struct_id_map, Struct_Name_Map &struct_name_map){
	Xml xml;
	xml.load_xml(path);
	TiXmlNode *node = xml.get_root_node();
	XML_LOOP_BEGIN(node)
		Base_Struct *base_struct = nullptr;
		if (struct_type == MONGODB)	{
			base_struct = new Mongo_Struct(xml, node);
		} else if (struct_type == MYSQL) {
			base_struct = new Mysql_Struct(xml, node);
		} else if (struct_type == LOGDB) {
			base_struct = new Log_Struct(xml, node);
		} else if (struct_type == MSG) {
			base_struct = new Msg_Struct(xml, node);
		} else {
			LOG_FATAL("load_struct struct_type = %d error abort", struct_type);
		}

		if(base_struct->msg_id() != 0) {
			struct_id_map.insert(std::pair<int32_t, Base_Struct*>(base_struct->msg_id(), base_struct));
		}
		struct_name_map.insert(std::pair<std::string, Base_Struct*>(base_struct->struct_name(), base_struct));
	XML_LOOP_END(node)
	return 0;
}

void Server_Conf::init_server_conf(void) {
	const Json::Value &server_conf = SERVER_CONFIG->server_conf();
	Lib_Log::instance()->set_file_switcher(server_conf["lib_log_switcher"].asInt());
	Log::instance()->set_file_switcher(server_conf["server_log_switcher"].asInt());

	server_sleep_time = Time_Value(1, 0);					//1s
	receive_timeout = Time_Value(server_conf["receive_timeout"].asInt(), 0);	//900s
	server_send_interval = Time_Value(0, server_conf["server_send_interval"].asInt());				//100us
	connector_send_interval = Time_Value(0, server_conf["connector_send_interval"].asInt());	//100us

	server_ip = server_conf["server_ip"].asString();
	log_port = server_conf["log_server"]["port"].asInt();
	db_port = server_conf["db_server"]["port"].asInt();
	login_client_network_protocol = server_conf["login_server"]["client_network_protocol"].asInt();
	login_client_port = server_conf["login_server"]["client_port"].asInt();
	login_gate_port = server_conf["login_server"]["gate_port"].asInt();
	master_gate_port = server_conf["master_server"]["gate_port"].asInt();
	master_game_port = server_conf["master_server"]["game_port"].asInt();
	game_gate_port = server_conf["game_server"]["gate_port"].asInt();
	gate_client_network_protocol = server_conf["gate_server"]["client_network_protocol"].asInt();
	gate_client_port = server_conf["gate_server"]["client_port"].asInt();
}
