#include <iostream>
#include <boost/asio.hpp>

void handle_connection(boost::asio::ip::tcp::socket& socket) {
    try {
        // Строка для отправки клиенту
        std::string message = "Привет, вы успешно подключились к серверу!\n";

        // Отправляем сообщение клиенту
        boost::asio::write(socket, boost::asio::buffer(message));
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка при отправке сообщения: " << e.what() << std::endl;
    }
}

int main() {
    setlocale(LC_ALL, "Russian");

    try {
        boost::asio::io_context io_context;
        boost::asio::ip::tcp::acceptor acceptor(io_context, 
        boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 8080));

        std::cout << "Сервер запущен. Ожидание подключений..." << std::endl;

        while (true) {
            boost::asio::ip::tcp::socket socket(io_context);
            acceptor.accept(socket);

            std::cout << "Получено новое подключение" << std::endl;

            // Обрабатываем подключение
            handle_connection(socket);
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }

    return 0;
}
