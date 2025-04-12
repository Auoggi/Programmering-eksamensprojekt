#include <iostream>
#include <string>
#include <sstream>
#include <set>
#include <boost/array.hpp>
#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

class udp_server {
public:
    udp_server(boost::asio::io_context& ioContext)
        : socket(ioContext, udp::endpoint(udp::v4(), 8080)),
          timer(ioContext, boost::asio::chrono::seconds(1))
    {
        timer.async_wait(boost::bind(&udp_server::messageInInterval, this));
        start_receive();
    }

private:
    void start_receive() {
        socket.async_receive_from(
            boost::asio::buffer(recvBuffer), currentClient,
            boost::bind(&udp_server::handle_receive, this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    }

    void handle_receive(const boost::system::error_code& error, std::size_t bytesTransferred) {
        if (!error) {
            std::string message;

            if (recvBuffer[0] == 0) {
                clients.insert(currentClient);
                message = std::string("hello\n");
            }
            else if (recvBuffer[0] == 4) {
                clients.erase(currentClient);
                message = std::string("goodbye\n");
            }
            else if (clients.count(currentClient)) {                
                recvBuffer.at(bytesTransferred) = '\0';
                message = recvBuffer.data();
            }
            send_message(message, currentClient);
            
            start_receive();
        }
    }

    void handle_send(boost::shared_ptr<std::string> /*message*/,
        const boost::system::error_code& /*error*/,
        std::size_t /*bytes_transferred*/)
    {
    }

    void send_message(std::string text, udp::endpoint endpoint) {
        boost::shared_ptr<std::string> message(new std::string(text));
        
        socket.async_send_to(boost::asio::buffer(*message), endpoint,
                    boost::bind(&udp_server::handle_send, this, message,
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
    }

    void messageInInterval() {
        for (udp::endpoint client : clients) {
            send_message("Message in interval of 1 second", client);
        }

        timer.expires_at(timer.expiry() + boost::asio::chrono::seconds(1)); // Will send every second
        timer.async_wait(boost::bind(&udp_server::messageInInterval, this));
    }

    boost::asio::steady_timer timer;
    udp::socket socket;
    udp::endpoint currentClient;
    boost::array<char, 128> recvBuffer;
    std::set<udp::endpoint> clients;
};

int main()
{
  try {
    boost::asio::io_context io_context;
    udp_server server(io_context);
    io_context.run();
  }
  catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}
