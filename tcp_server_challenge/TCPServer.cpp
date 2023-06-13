#include "TCPServer.h"

TCPServer::TCPServer()
{
	this->config_data.port = 0;
	this->config_data.max_file_size_bytes = 0;
	this->config_data.directory = "";
	this->config_data.file_prefix = "";
	this->config_data.timer_seconds = 0;

}

TCPServer::~TCPServer()
{

}

void TCPServer::import_config_data(const std::string& configFileName)
{
	boost::property_tree::ptree pt;
	boost::property_tree::ini_parser::read_ini(configFileName, pt);

	this->config_data.port = pt.get<unsigned short>("port");
	this->config_data.max_file_size_bytes = pt.get<std::size_t>("max_file_size_bytes");
	this->config_data.directory = pt.get<std::string>("directory");
	this->config_data.file_prefix = pt.get<std::string>("file_prefix");
	this->config_data.timer_seconds = pt.get<std::size_t>("timer_seconds");


}


void TCPServer::set_config_data(ConfigData d)

{
	this->config_data = d;
}

ConfigData TCPServer::get_config_data(void)
{
	return this->config_data;
}

bool TCPServer::check_config_data_is_set(void)
{
	if ((this->config_data.port != 0) &&
		(this->config_data.max_file_size_bytes != 0) &&
		(this->config_data.directory != "") &&
		(this->config_data.file_prefix != "") &&
		(this->config_data.timer_seconds != 0))
	{
		return true;
		
	}

	return false;

}


void TCPServer::start_server(boost::asio::io_context& ioContext)
{
	std::cout << "Served started" << std::endl;
	if (this->check_config_data_is_set())
	{
		
		boost::asio::ip::tcp::acceptor acceptor(ioContext, 
			boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), this->config_data.port));


		boost::asio::ip::tcp::socket socket(ioContext);


		std::cout << "Waiting for client connection..." << std::endl;
		acceptor.accept(socket);
	
		

		this->handleConnection(std::move(socket));
	}
	else
	{
		std::cout << "Error! Make sure all config variables are set before conect" << std::endl;
	}
}

void TCPServer::handleConnection(boost::asio::ip::tcp::socket socket) {
	// Read data from the client

	std::cout << "handling connection" << std::endl;
	this->readData(socket);
}

void TCPServer::readData(tcp::socket& socket)
{

	std::cout << "Reading Data" << std::endl;
	try {

		boost::asio::streambuf buffer;
		auto startTime = std::chrono::steady_clock::now();
	
		boost::system::error_code error;
		
		auto currentTime = std::chrono::steady_clock::now();


		size_t bytesTransferred = boost::asio::read_until(socket, buffer, '\n', error);
		
		if (error) 
		{
			std::cerr << "Error receiving data: " << error.message() << std::endl;
		}


		std::string data(boost::asio::buffers_begin(buffer.data()), boost::asio::buffers_end(buffer.data()));
		buffer.consume(bytesTransferred);

		if (data.size() > 0)
		{
				startTime = std::chrono::steady_clock::now();
				saveFile(this->config_data.directory, this->config_data.file_prefix, data, this->config_data.max_file_size_bytes);
		}
		else
		{
			auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
				
			if (elapsedTime >= this->config_data.timer_seconds)
			{
				std::cout << "Time ran out" << std::endl;
				// Close the connection
				this->closeConnection(socket);
			}
		}

	}
	catch (const std::exception& ex)
	{
		std::cerr << "Exception occurred: " << ex.what() << std::endl;
		// Close the connection
	}

	this->closeConnection(socket);
}


void TCPServer::closeConnection(boost::asio::ip::tcp::socket& socket) {
	boost::system::error_code error;

	std::cout << "Close connection" << std::endl;
	// Close the connection
	socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, error);
	socket.close();
}