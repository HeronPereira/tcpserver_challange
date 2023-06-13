#include <iostream>
#include "FileStorage.h"
#include "TCPServer.h"

int main()
{
	TCPServer server;
	boost::asio::io_context ioContext;

	server.import_config_data("configData.ini");


	server.start_server(ioContext);

	ioContext.run();

	return(0);
}