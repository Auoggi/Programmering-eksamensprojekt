#ifndef UDP_CLIENT
#define UDP_CLIENT

#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

class UdpClient {
public:
    UdpClient(boost::asio::io_context& ioContext);
    
    void connect(std::string ipv4, std::string port);
    void sendMessage(std::string text);
    std::string listen();

private:
    udp::socket socket_;
    udp::resolver resolver;
    udp::endpoint receiverEndpoint;
    udp::endpoint senderEndpoint;
    boost::array<char, 2048> recvBuf;
};

#endif