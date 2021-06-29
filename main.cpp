#include <iostream>

#include "asio.hpp"

namespace net = asio;

int main() {
    net::io_context ctx;
    net::ip::tcp::endpoint endpoint{net::ip::make_address("220.181.38.148"), 80};
    net::ip::tcp::socket socket{ctx};

    socket.async_connect(endpoint, [&](std::error_code err) {
        if (!err) {
            char buf[] = "GET / HTTP/1.0\r\n\r\n";
            socket.async_send(net::buffer(buf), [&](std::error_code err, std::size_t n) {
                if (!err) {
                    char buf[32768]{};
                    socket.async_receive(net::buffer(buf), [&](std::error_code err, std::size_t n){
                        if (!err) {
                            std::cout << buf;
                        } else {
                            std::cout << "receive error: " << err.message() << "\n";
                        }
                    });
                } else {
                    std::cout << "send error: " << err.message() << "\n";
                }
            });
        } else {
            std::cout << "connect error: " << err.message() << "\n";
        }
    });

    ctx.run();
}
