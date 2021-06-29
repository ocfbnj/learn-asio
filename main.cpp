#include <iostream>

#include "asio.hpp"

namespace net = asio;

net::awaitable<void> connect(net::ip::tcp::socket socket, const net::ip::tcp::endpoint& endpoint) {
    char read_buf[4096]{};

    try {
        co_await socket.async_connect(endpoint, net::use_awaitable);
        co_await socket.async_send(net::buffer("GET / HTTP/1.0\r\n\r\n"), net::use_awaitable);
        co_await socket.async_receive(net::buffer(read_buf), net::use_awaitable);

        std::cout << read_buf;
    } catch (const std::exception& e) {
        std::cout << e.what() << "\n";
    }
}

int main() {
    net::io_context ctx;
    net::ip::tcp::endpoint endpoint{net::ip::make_address("220.181.38.148"), 80};
    net::ip::tcp::socket socket{ctx};

    // socket.async_connect(endpoint, [&](std::error_code err) {
    //     if (!err) {
    //         socket.async_send(net::buffer("GET / HTTP/1.0\r\n\r\n"), [&](std::error_code err, std::size_t n) {
    //             if (!err) {
    //                 char read_buf[4096]{};
    //                 socket.async_receive(net::buffer(read_buf), [&](std::error_code err, std::size_t n) {
    //                     if (!err) {
    //                         std::cout << read_buf;
    //                     } else {
    //                         std::cout << "receive error: " << err.message() << "\n";
    //                     }
    //                 });
    //             } else {
    //                 std::cout << "send error: " << err.message() << "\n";
    //             }
    //         });
    //     } else {
    //         std::cout << "connect error: " << err.message() << "\n";
    //     }
    // });

    net::co_spawn(ctx, connect(std::move(socket), endpoint), net::detached);

    ctx.run();
}
