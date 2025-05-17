#ifndef UDP_SERVER
#define UDP_SERVER

#include <string>
#include <sstream>
#include <set>
#include <boost/array.hpp>
#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

class UdpServer {
public:
    UdpServer(boost::asio::io_context& ioContext);

    void sendMessage(std::string text, udp::endpoint endpoint);

private:
    void startReceive();
    void handleReceive(const boost::system::error_code& error, std::size_t bytesTransferred);
    void handleSend(boost::shared_ptr<std::string> /*message*/, 
        const boost::system::error_code& /*error*/,
        std::size_t /*bytes_transferred*/);
    void messageInInterval();

    boost::asio::steady_timer timer;
    udp::socket socket;
    udp::endpoint currentClient; // client who recently send a packet
    boost::array<char, 2048> recvBuffer;
    std::set<udp::endpoint> clients;

};

#endif