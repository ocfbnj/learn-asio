#include <iostream>

#include "asio.hpp"

asio::awaitable<void> http_request(asio::ip::tcp::socket socket, const asio::ip::tcp::endpoint& endpoint) {
    char read_buf[4096]{};

    try {
        co_await socket.async_connect(endpoint, asio::use_awaitable);
        co_await socket.async_send(asio::buffer("GET / HTTP/1.0\r\n\r\n"), asio::use_awaitable);
        co_await socket.async_receive(asio::buffer(read_buf), asio::use_awaitable);

        std::cout << read_buf;
    } catch (const std::exception& e) {
        std::cout << e.what() << "\n";
    }
}

char read_buf[4096];

int main() {
    asio::io_context ctx;
    asio::ip::tcp::endpoint endpoint{asio::ip::make_address("220.181.38.148"), 80};
    asio::ip::tcp::socket socket{ctx};

    // asio::co_spawn(ctx, http_request(std::move(socket), endpoint), asio::detached);

    socket.async_connect(endpoint, [&](std::error_code err) {
        socket.async_send(asio::buffer("GET / HTTP/1.0\r\n\r\n"), [&](std::error_code err, std::size_t n) {
            socket.async_receive(asio::buffer(read_buf), [&](std::error_code err, std::size_t n) {
                std::cout << read_buf;
            });
        });
    });

    asio::make_strand(ctx);

    ctx.run();
}
