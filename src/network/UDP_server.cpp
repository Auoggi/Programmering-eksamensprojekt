#include <iostream>
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
    UdpServer(boost::asio::io_context& ioContext) // io_context is used for async
        : socket(ioContext, udp::endpoint(udp::v4(), 8080)),
          timer(ioContext, boost::asio::chrono::seconds(1))
    {
        timer.async_wait(boost::bind(&UdpServer::messageInInterval, this));
        start_receive();
    }

private:
    void start_receive() {
        socket.async_receive_from(
            boost::asio::buffer(recvBuffer), currentClient,
            boost::bind(&UdpServer::handle_receive, this, // bind is used to let Boost know to run a callback function (handle_receive) with these arguments
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    }

    void handle_receive(const boost::system::error_code& error, std::size_t bytesTransferred) {
        if (!error) {
            std::string message;

            if (recvBuffer[0] == 0) { // Client connects to server
                clients.insert(currentClient);
                message = std::string("hello");
            }
            else if (recvBuffer[0] == 4) { // Client disconnects from server
                clients.erase(currentClient);
                message = std::string("goodbye");
            }
            else if (clients.count(currentClient)) { // If none of the above send back what the client sent                
                recvBuffer.at(bytesTransferred) = '\0';
                message = recvBuffer.data();
            }
            send_message(message, currentClient);
            
            start_receive();
        }
    }

    // A callback function used if something needs to be done after sending a message
    void handle_send(boost::shared_ptr<std::string> /*message*/, 
        const boost::system::error_code& /*error*/,
        std::size_t /*bytes_transferred*/)
    {
    }

    void send_message(std::string text, udp::endpoint endpoint) {
        boost::shared_ptr<std::string> message(new std::string(text));
        
        socket.async_send_to(boost::asio::buffer(*message), endpoint,
                    boost::bind(&UdpServer::handle_send, this, message,
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
    }

    // sends message to all of the connected clients every second
    void messageInInterval() {
        for (udp::endpoint client : clients) {
            send_message("Message in interval of 1 second", client);
        }

        timer.expires_at(timer.expiry() + boost::asio::chrono::seconds(1)); // Starts new timer to one second in the future
        timer.async_wait(boost::bind(&UdpServer::messageInInterval, this));
    }

    boost::asio::steady_timer timer;
    udp::socket socket;
    udp::endpoint currentClient; // client who recently send a packet
    boost::array<char, 128> recvBuffer;
    std::set<udp::endpoint> clients;
};

int _main()
{
  try {
    boost::asio::io_context io_context;
    UdpServer server(io_context);
    io_context.run();
  }
  catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}
