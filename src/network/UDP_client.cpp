#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

class udp_client {
public:
    udp_client(boost::asio::io_context& io_context)
        : socket_(io_context),
          resolver_(io_context)
    {
        socket_.open(udp::v4());
    }

    void connect(std::string ipv4, std::string port) {
        receiver_endpoint = *resolver_.resolve(udp::v4(), ipv4, port).begin();

        send_buf.at(0) = 0;
        socket_.send_to(boost::asio::buffer(send_buf), receiver_endpoint);
    }

    void send_message(std::string text) {
        for (int i = 0; i < text.size() || i < send_buf.max_size(); ++i) {
            send_buf.at(i) = text[i];
        }
        //std::cout.write(send_buf.data(), text.size());

        socket_.send_to(boost::asio::buffer(send_buf), receiver_endpoint);
    }

    std::string listen() {
        udp::endpoint sender_endpoint;
        size_t len = socket_.receive_from(boost::asio::buffer(recv_buf), sender_endpoint);

        std::string message;
        for (int i = 0; i < len; ++i) {
            message.push_back(recv_buf[i]);
        }
        return message;
    }

private:
    udp::socket socket_;
    udp::resolver resolver_;
    udp::endpoint receiver_endpoint;
    boost::array<char, 128> send_buf;
    boost::array<char, 128> recv_buf;
};


int main(int argc, char* argv[]) {
    try {
        if (argc != 2) {
            std::cerr << "Usage: client <host>" << std::endl;
            return 1;
        }
        boost::asio::io_context io_context;

        udp_client udp_client(io_context);
        udp_client.connect(argv[1], "8080");
        udp_client.send_message("hi");
        
        for (;;)
        {
            std::cout << udp_client.listen() << std::endl;
        }
        
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}