#include <string>
#include <memory>
#include <string_view>
#include <vector>

#include <cstdint>
#include <cstring>
#include <iostream>

//https://chatgpt.com/share/68163d5f-c924-8003-97de-ff7cd7625af8

class ByteBuffer {
public:
    std::vector<char> data;
    size_t read_offset = 0;

    template<typename T>
    void put(T value) {
        static_assert(std::is_trivially_copyable_v<T>, "Only trivial types allowed");
        char* p = reinterpret_cast<char*>(&value); // gør så at den givne værdi bliver set som en uint8_t pointer i stedet for den give type
        data.insert(data.end(), p, p + sizeof(T));
    }

    template<typename T>
    T read() {
        static_assert(std::is_trivially_copyable_v<T>, "Only trivial types allowed");
        if (read_offset + sizeof(T) > data.size()) {
            throw std::runtime_error("Read past end of buffer");
        }

        T value;
        std::memcpy(&value, &data[read_offset], sizeof(T));
        read_offset += sizeof(T);
        return value;
    }

    void reset() { read_offset = 0; }

    const char* raw() const { return data.data(); }
    size_t size() const { return data.size(); }
};

enum class PacketType : uint8_t {
    MOVEMENT,
    ATTACK,
    STAMINA,
    HEALTH,
    USE,
};
// [ length: 4-byte ][ type: 1-byte ][ payload... ]
// TODO: lær mere om virtual
struct Packet {
    virtual ~Packet() = default;
    virtual PacketType type() const = 0;
    virtual std::string serialize() const = 0;
    static std::unique_ptr<Packet> deserialize(std::string str);
};

struct MovementPacket : Packet {
    int id;
    float x, y;
    PacketType type() const override { return PacketType::MOVEMENT; }

    MovementPacket(int id_, float x_, float y_) {
        id = id_;
        x = x_;
        y = y_;
    }

    std::string serialize() const override {
        ByteBuffer buffer;
        uint32_t payload_size = sizeof(id) + sizeof(x) + sizeof(y);

        buffer.put<uint32_t>(payload_size);
        buffer.put<uint8_t>(static_cast<uint8_t>(type()));
        buffer.put<int>(id);
        buffer.put<float>(x);
        buffer.put<float>(y);

        std::cout << buffer.read<uint32_t>() << std::endl;
        std::cout << static_cast<uint32_t>(buffer.read<uint8_t>()) << std::endl;
        std::cout << buffer.read<int>() << std::endl;
        std::cout << buffer.read<float>() << std::endl;
        std::cout << buffer.read<float>() << std::endl;

        std::string str(buffer.data.begin(), buffer.data.end());
        return str;
    }
};
/*
std::cout << buffer.read<uint32_t>() << std::endl;
std::cout << static_cast<uint32_t>(buffer.read<uint8_t>()) << std::endl;
std::cout << buffer.read<int>() << std::endl;
std::cout << buffer.read<float>() << std::endl;
std::cout << buffer.read<float>() << std::endl;
*/


std::unique_ptr<Packet> Packet::deserialize(std::string str) {
    ByteBuffer buffer;
    buffer.data.assign(str.begin(), str.end());

    uint32_t length = buffer.read<uint32_t>();
    PacketType type = static_cast<PacketType>(buffer.read<uint8_t>());

    switch (type) {
        case PacketType::MOVEMENT:
            int id = buffer.read<int>();
            float x = buffer.read<float>();
            float y = buffer.read<float>();
            return std::make_unique<MovementPacket>(id, x, y);
        /*default:
            throw std::invalid_argument("Unknown PacketType");*/
    };
}

int main() {
    MovementPacket packet(2, 5.5, 8.2);

    std::string packet_string = packet.serialize();
    std::unique_ptr<Packet> pkt = Packet::deserialize(packet_string);


    switch (pkt->type()) {
        case PacketType::MOVEMENT:
            std::cout << "Packet is of type MOVEMENT" << std::endl;
            break;
    }
}
