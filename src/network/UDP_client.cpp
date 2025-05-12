#include <stdlib.h>
#include <stdio.h>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

class udpClient {
public:
    udpClient(boost::asio::io_context& ioContext)
        : socket_(ioContext),
          resolver(ioContext) // DNS lookup like gets ip address from website link
    {
        socket_.open(udp::v4());
    }

    void connect(std::string ipv4, std::string port) {
        receiverEndpoint = *resolver.resolve(udp::v4(), ipv4, port).begin(); // gets ip address and port for server

        socket_.send_to(boost::asio::buffer("\0"), receiverEndpoint);
    }

    void sendMessage(std::string text) {
        socket_.send_to(boost::asio::buffer(text), receiverEndpoint);
    }

    std::string listen() {
        size_t len = socket_.receive_from(boost::asio::buffer(recvBuf), senderEndpoint); // len is number of bytes transferred

        recvBuf.at(len) = '\0';
        return recvBuf.data();
    }

private:
    udp::socket socket_;
    udp::resolver resolver;
    udp::endpoint receiverEndpoint;
    udp::endpoint senderEndpoint;
    boost::array<char, 2048> recvBuf;
};


int _main(int argc, char* argv[]) {
    try {
        if (argc != 2) {
            fprintf(stderr, "Usage: client <host>\n");
            return 1;
        }
        boost::asio::io_context ioContext;

        udpClient udpClient(ioContext);
        udpClient.connect(argv[1], "8080");
        udpClient.sendMessage("HI");
        
        for (;;)
        {
            fprintf(stderr, "text received: %s\n", udpClient.listen().data());
        }
        
    }
    catch (std::exception& e) {
        fprintf(stderr, "Error: %s\n", e.what());
    }

    return 0;
}