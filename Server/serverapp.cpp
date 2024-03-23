#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <boost/asio.hpp>

void handle_connection(boost::asio::ip::tcp::socket& socket) {
    try {
        std::string greating = "You`v connected to the server!\n";
        std::mutex mtx;
        mtx.lock();
        boost::asio::write(socket, boost::asio::buffer(greating));
        boost::asio::streambuf buffer;

        while (true) {

            boost::asio::write(socket, boost::asio::buffer("123"));
            boost::asio::read_until(socket, buffer, '\0'); // Чтение до символа новой строки
            std::string message(boost::asio::buffers_begin(buffer.data()), boost::asio::buffers_end(buffer.data()));
            std::cout << "Received message: " << message << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        std::string message = "";

        mtx.unlock();
    }
    catch (const std::exception& e) {
        std::cerr << "Error while sending message: " << e.what() << std::endl;
    }
    std::string message = "";
    while (true) {
        std::mutex mtx;
        boost::asio::streambuf buffer;
        boost::system::error_code error;
        mtx.lock();
        size_t bytes_transferred = boost::asio::read_until(socket, buffer, '\n', error);
        mtx.unlock();
        if (error == boost::asio::error::eof)
            break; // Connection closed cleanly by peer.
        else if (error)
            throw boost::system::system_error(error); // Some other error.
        std::istream is(&buffer);
        std::getline(is, message);
        mtx.lock();
        std::cout << "Received message: " << message << std::endl;
        mtx.unlock();
    }
}

int main() {
   
    boost::asio::io_context io_context;
    auto endpt = boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 8080);
    boost::asio::ip::tcp::acceptor acceptor(io_context, endpt);
    while (true) {
        boost::asio::ip::tcp::socket socket(io_context);
        acceptor.accept(socket);

     /*   std::string greating = "You`v succesfully connected to the server!\n";
        boost::asio::write(socket, boost::asio::buffer(greating));*/


        std::thread newconnection(handle_connection,std::ref(socket));
        newconnection.join();


    }




    return 0;
}
