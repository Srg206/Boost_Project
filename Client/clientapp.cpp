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
		boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve("127.0.0.1", "8080");
		boost::asio::connect(socket, endpoints);
		
		
		boost::asio::streambuf buffer;
		boost::asio::read_until(socket, buffer, '\0'); // Чтение до символа новой строки
		std::string message(boost::asio::buffers_begin(buffer.data()), boost::asio::buffers_end(buffer.data()));
		std::cout << "Received message: " << message << std::endl;
		while (true) {
			boost::asio::read_until(socket, buffer, '\0'); // Чтение до символа новой строки
			std::string message(boost::asio::buffers_begin(buffer.data()), boost::asio::buffers_end(buffer.data()));
			std::cout << "Received message: " << message << std::endl;
			boost::asio::write(socket, boost::asio::buffer("123"));
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			//system("cls");
			buffer.consume(buffer.size());

		}
		


		//handle_connection(socket);

	}
	catch (std::exception& ex) {
		std::cout << ex.what() << std::endl;
	}

	system("pause");
	return 0;
}
