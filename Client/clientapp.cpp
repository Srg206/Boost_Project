#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>
#include <chrono>
#include <stdlib.h>

//using namespace boost::asio;
//using namespace boost::asio::ip;
void send_message(boost::asio::ip::tcp::socket& socket) {
	std::string msg;
	std::getline(std::cin,msg,'\n');
	boost::asio::write(socket, boost::asio::buffer(msg));
	//std::this_thread::sleep_for(std::chrono::milliseconds(10));

}

void recieve_messge(boost::asio::ip::tcp::socket& socket, std::mutex &mtx) {
	while (true) {
		//mtx.lock();
		boost::asio::streambuf buffer;
		boost::asio::read_until(socket, buffer, '\n'); // Чтение до символа новой строки
		std::string message(boost::asio::buffers_begin(buffer.data()), boost::asio::buffers_end(buffer.data()));
		std::cout << "Received message: " << message << std::endl;
		//mtx.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

void handle_connection(boost::asio::ip::tcp::socket& socket) {
	std::mutex mtx;
	while (true) {
		std::thread getter(recieve_messge, std::ref(socket),std::ref(mtx));
		getter.join();
		//std::string newmsg="";det
		send_message(socket);
		//std::cout<< recieve_messge(socket)<<std::endl;

	}
}


int main() {

	boost::asio::io_context io_context;
	boost::asio::ip::tcp::socket socket(io_context);
	try {
		boost::asio::ip::tcp::resolver resolver(io_context);
		boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve("10.54.65.132", "8080");
		boost::asio::connect(socket, endpoints);
		
		std::string ping = "TESTING PING MESSAGE\n";

		std::mutex mtx;
		//mtx.lock();
		boost::asio::write(socket, boost::asio::buffer(ping));
		std::cout << "wrote to socket" << std::endl;
		//mtx.unlock();
		


		boost::asio::streambuf buffer;
		boost::asio::read_until(socket, buffer, '\n');
		
		std::string message(boost::asio::buffers_begin(buffer.data()), boost::asio::buffers_end(buffer.data()));
		std::cout << "Received message: " << message << (message==ping? " is message we`v sended   ------- OK" : " is not message we`v sended   ------- WRONG ") << std::endl;

	}
	catch (std::exception& ex) {
		std::cout << ex.what() << std::endl;
	}

	system("pause");
	return 0;
}
