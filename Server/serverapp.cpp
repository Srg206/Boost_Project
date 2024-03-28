#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <boost/asio.hpp>

void handle_connection(boost::asio::ip::tcp::socket& socket) {
    try {
        boost::asio::streambuf buffer;
        boost::asio::read_until(socket, buffer, '\n'); // Чтение до символа новой строки
        std::string message(boost::asio::buffers_begin(buffer.data()), boost::asio::buffers_end(buffer.data()));
        std::cout << "Received message: " << message << std::endl;
        //mtx.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        boost::asio::write(socket, boost::asio::buffer(message));

    }
    catch (const std::exception& e) {
        std::cerr << "Error while sending message: " << e.what() << std::endl;
    }
   
}

int main() {
   
    boost::asio::io_context io_context;
    auto endpt = boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("10.54.65.132"), 8080);//"10.54.65.132"
    boost::asio::ip::tcp::acceptor acceptor(io_context, endpt);


    while (true) {
        boost::asio::ip::tcp::socket socket(io_context);
        acceptor.accept(socket);

     /*   std::string greating = "You`v succesfully connected to the server!\n";
        boost::asio::write(socket, boost::asio::buffer(greating)); */


        std::thread newconnection(handle_connection,std::ref(socket));
        newconnection.join();


    }




    return 0;
}
