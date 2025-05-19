#include <stdlib.h>
#include <stdio.h>
#include "UDP_client.h"


UdpClient::UdpClient(boost::asio::io_context& ioContext)
    : socket_(ioContext),
        resolver(ioContext) // DNS lookup like gets ip address from website link
{
    socket_.open(udp::v4());
}

void UdpClient::connect(std::string ipv4, std::string port) {
    receiverEndpoint = *resolver.resolve(udp::v4(), ipv4, port).begin(); // gets ip address and port for server

    socket_.send_to(boost::asio::buffer("\0"), receiverEndpoint);
}

void UdpClient::sendMessage(std::string text) {
    socket_.send_to(boost::asio::buffer(text), receiverEndpoint);
}

std::string UdpClient::listen() {
    size_t len = socket_.receive_from(boost::asio::buffer(recvBuf), senderEndpoint); // len is number of bytes transferred

    recvBuf.at(len) = '\0';
    return recvBuf.data();
}


int client_main(int argc, char* argv[]) {
    try {
        if (argc != 2) {
            fprintf(stderr, "Usage: client <host>\n");
            return 1;
        }
        boost::asio::io_context ioContext;

        UdpClient UdpClient(ioContext);
        UdpClient.connect(argv[1], "8080");
        UdpClient.sendMessage("HI");
        
        for (;;)
        {
            fprintf(stderr, "text received: %s\n", UdpClient.listen().data());
        }
        
    }
    catch (std::exception& e) {
        fprintf(stderr, "Error: %s\n", e.what());
    }

    return 0;
}