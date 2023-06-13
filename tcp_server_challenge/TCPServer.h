#pragma once
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/asio.hpp>
#include <string>
#include "FileStorage.h"

struct ConfigData 
{
	unsigned short port;
	std::size_t max_file_size_bytes;
	std::string directory;
	std::string file_prefix;
	std::size_t timer_seconds;
	//	Any other config data desired may be added on
};


using boost::asio::ip::tcp;

class TCPServer
{
public:
	//	Constructor
	TCPServer();
	
	//	Destructor
	~TCPServer();

	//	Read configuration data
	void import_config_data(const std::string& configFileName);
	
	ConfigData get_config_data(void);

	void set_config_data(ConfigData data);

	// checks if the config data was imported
	bool check_config_data_is_set(void);
	
	void start_server(boost::asio::io_context& ioContext);

	void handleConnection(boost::asio::ip::tcp::socket socket);

	void readData(tcp::socket& socket);

	void closeConnection(boost::asio::ip::tcp::socket& socket);

private:
	ConfigData config_data;
	bool is_server_started = false;
	std::atomic<bool> closeServer = false;
};