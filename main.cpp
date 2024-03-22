#include <iostream>
#include <boost/asio.hpp>

void handle_connection(boost::asio::ip::tcp::socket& socket) {
    try {
        // ������ ��� �������� �������
        std::string message = "������, �� ������� ������������ � �������!\n";

        // ���������� ��������� �������
        boost::asio::write(socket, boost::asio::buffer(message));
    }
    catch (const std::exception& e) {
        std::cerr << "������ ��� �������� ���������: " << e.what() << std::endl;
    }
}

int main() {
    setlocale(LC_ALL, "Russian");

    try {
        boost::asio::io_context io_context;
        boost::asio::ip::tcp::acceptor acceptor(io_context, 
        boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 8080));

        std::cout << "������ �������. �������� �����������..." << std::endl;

        while (true) {
            boost::asio::ip::tcp::socket socket(io_context);
            acceptor.accept(socket);

            std::cout << "�������� ����� �����������" << std::endl;

            // ������������ �����������
            handle_connection(socket);
        }
    }
    catch (const std::exception& e) {
        std::cerr << "������: " << e.what() << std::endl;
    }

    return 0;
}
