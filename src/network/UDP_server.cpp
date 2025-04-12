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
    udp_server(boost::asio::io_context& io_context)
        : socket_(io_context, udp::endpoint(udp::v4(), 8080)),
          timer_(io_context, boost::asio::chrono::seconds(1))
    {
        timer_.async_wait(boost::bind(&udp_server::messageInInterval, this));
        start_receive();
    }

private:
    void start_receive() {
        socket_.async_receive_from(
            boost::asio::buffer(recv_buffer_), remote_endpoint_,
            boost::bind(&udp_server::handle_receive, this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    }

    void handle_receive(const boost::system::error_code& error, std::size_t bytes_transferred) {
        if (!error) {
            std::string message;

            if (recv_buffer_[0] == 0) {
                clients.insert(remote_endpoint_);
                message = std::string("hello\n");
            }
            else if (recv_buffer_[0] == 4) {
                clients.erase(remote_endpoint_);
                message = std::string("goodbye\n");
            }
            else if (clients.count(remote_endpoint_)) {                
                recv_buffer_.at(bytes_transferred) = '\0';
                message = recv_buffer_.data();
            }

            send_message(message, remote_endpoint_);
            
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
        
        socket_.async_send_to(boost::asio::buffer(*message), remote_endpoint_,
                    boost::bind(&udp_server::handle_send, this, message,
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
    }

    void messageInInterval() {
        for (udp::endpoint client : clients) {
            send_message("Message in interval of 1 second", client);
        }

        timer_.expires_at(timer_.expiry() + boost::asio::chrono::seconds(1)); // Will send every second
        timer_.async_wait(boost::bind(&udp_server::messageInInterval, this));
    }

    boost::asio::steady_timer timer_;
    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    boost::array<char, 128> recv_buffer_;
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
