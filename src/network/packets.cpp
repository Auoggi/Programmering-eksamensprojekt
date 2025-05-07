#include <string>
#include <memory>
#include <vector>
#include <iostream>

//https://chatgpt.com/share/68163d5f-c924-8003-97de-ff7cd7625af8

enum class PacketType : uint16_t {
    MOVEMENT,
    ATTACK,
    STAMINA,
    HEALTH,
    USE,
};

class ByteBuffer {
public:
    std::vector<char> data;
    size_t read_offset = 0;

    // inserts data into the vector in the form of bytes
    template<typename T>
    void put(T value) { 
        static_assert(std::is_trivially_copyable_v<T>, "Only trivial types allowed");
        char* p = reinterpret_cast<char*>(&value); // reinterprets the given values information to be the type specified (so as a char*)
        data.insert(data.end(), p, p + sizeof(T));
    }

    // copies data from vector and puts it into specified type
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

// packet:
// [ length: 4-byte ][ type: 1-byte ][ payload... ]
class Packet {
public:
    // virtual keyword is used to support runtime polymorphism, 
    // so the compiler knows to use the child methods rather than the parent methods with a vtable
    virtual ~Packet() = default;
    virtual PacketType type() const = 0;
    // will make packet contents to a string
    virtual std::string serialize() const = 0; 
    // parses string contents to make it a packet
    static std::unique_ptr<Packet> deserialize(std::string str); // unique_ptr is used so that you can use base methods like type
};

// contains x and y position
class MovementPacket : public Packet {
public:
    MovementPacket(int id, float x, float y)
        : id(id), x(x), y(y) {}
    
    PacketType type() const override { return PacketType::MOVEMENT; }

    std::string serialize() const override {
        ByteBuffer buffer;
        uint32_t payload_size = sizeof(id) + sizeof(x) + sizeof(y);

        buffer.put<uint32_t>(payload_size);
        buffer.put<uint16_t>(static_cast<uint16_t>(type()));
        buffer.put<int>(id);
        buffer.put<float>(x);
        buffer.put<float>(y);

        std::string str(buffer.data.begin(), buffer.data.end());
        return str;
    }

    int id;
    float x, y;
};

// contains angle and attacktype
class AttackPacket : public Packet {
public:
    AttackPacket(int id, float attackType, float angle)
        : id(id), attackType(attackType), angle(angle) {}

    PacketType type() const override { return PacketType::ATTACK; }

    std::string serialize() const override {
        ByteBuffer buffer;
        uint32_t payload_size = sizeof(id) + sizeof(attackType) + sizeof(angle);

        buffer.put<uint32_t>(payload_size);
        buffer.put<uint16_t>(static_cast<uint16_t>(type()));
        buffer.put<int>(id);
        buffer.put<int>(attackType);
        buffer.put<float>(angle);

        std::string str(buffer.data.begin(), buffer.data.end());
        return str;
    }
    
    int id, attackType;
    float angle;
};

// contains current stamina
class StaminaPacket : public Packet {
public:
    StaminaPacket(int id, float stamina)
        : id(id), stamina(stamina) {}

    PacketType type() const override { return PacketType::STAMINA; }

    std::string serialize() const override {
        ByteBuffer buffer;
        uint32_t payload_size = sizeof(id) + sizeof(stamina);

        buffer.put<uint32_t>(payload_size);
        buffer.put<uint16_t>(static_cast<uint16_t>(type()));
        buffer.put<int>(id);
        buffer.put<float>(stamina);

        std::string str(buffer.data.begin(), buffer.data.end());
        return str;
    }
    
    int id;
    float stamina;
};

// contains current health
class HealthPacket : public Packet {
public:
    HealthPacket(int id, float health)
        : id(id), health(health) {}

    PacketType type() const override { return PacketType::HEALTH; }

    std::string serialize() const override {
        ByteBuffer buffer;
        uint32_t payload_size = sizeof(id) + sizeof(health);

        buffer.put<uint32_t>(payload_size);
        buffer.put<uint16_t>(static_cast<uint16_t>(type()));
        buffer.put<int>(id);
        buffer.put<float>(health);

        std::string str(buffer.data.begin(), buffer.data.end());
        return str;
    }
    
    int id;
    float health;
};

// contains item used
class UsePacket : public Packet {
public:
    UsePacket(int id, std::string use)
        : id(id), use(use) {}

    PacketType type() const override { return PacketType::USE; }

    std::string serialize() const override {
        ByteBuffer buffer;
        uint32_t payload_size = sizeof(id) + use.size();

        buffer.put<uint32_t>(payload_size);
        buffer.put<uint16_t>(static_cast<uint16_t>(type()));
        buffer.put<int>(id);
        buffer.data.insert(buffer.data.end(), use.begin(), use.end());

        std::string str(buffer.data.begin(), buffer.data.end());
        return str;
    }
    
    int id;
    std::string use;
};

std::unique_ptr<Packet> Packet::deserialize(std::string str) {
    ByteBuffer buffer;
    buffer.data.assign(str.begin(), str.end());

    uint32_t length = buffer.read<uint32_t>();
    PacketType type = static_cast<PacketType>(buffer.read<uint16_t>());

    switch (type) {
        case PacketType::MOVEMENT: {
            int id = buffer.read<int>();
            float x = buffer.read<float>();
            float y = buffer.read<float>();
            return std::make_unique<MovementPacket>(id, x, y);
        }
        case PacketType::ATTACK: {
            int id = buffer.read<int>();
            int attackType = buffer.read<int>();
            float angle = buffer.read<float>();
            return std::make_unique<AttackPacket>(id, attackType, angle);
        }
        case PacketType::STAMINA: {
            int id = buffer.read<int>();
            float stamina = buffer.read<float>();
            return std::make_unique<StaminaPacket>(id, stamina);
        }
        case PacketType::HEALTH: {
            int id = buffer.read<int>();
            float health = buffer.read<float>();
            return std::make_unique<HealthPacket>(id, health);
        }
        case PacketType::USE: {
            int id = buffer.read<int>();
            std::string use;
            use.insert(use.end(), &buffer.data[buffer.read_offset], &buffer.data[buffer.read_offset] + length);
            buffer.read_offset += length;
            return std::make_unique<UsePacket>(id, use);
        }
        default:
            throw std::invalid_argument("Unknown PacketType");
    };
}

// will make the unique pointer to the type specified
template<typename T>
T& to_packet(Packet& pkt) {
    return dynamic_cast<T&>(pkt); // Throws if wrong type
}

int _main() {
    UsePacket packet(2, "hello");

    std::string packet_string = packet.serialize();
    std::unique_ptr<Packet> pkt = Packet::deserialize(packet_string);

    switch (pkt->type()) {
        case PacketType::MOVEMENT: {
            std::cout << "Packet is of type MOVEMENT" << std::endl;
            MovementPacket& mov = to_packet<MovementPacket>(*pkt);
            std::cout << "id: " << mov.id << std::endl;
            std::cout << "x: " << mov.x << std::endl;
            std::cout << "y: " << mov.y << std::endl;
            break;
        }
        case PacketType::ATTACK: {
            std::cout << "Packet is of type ATTACK" << std::endl;
            AttackPacket& atk = to_packet<AttackPacket>(*pkt);
            std::cout << "id: " << atk.id << std::endl;
            std::cout << "attackType: " << atk.attackType << std::endl;
            std::cout << "angle: " << atk.angle << std::endl;
            break;
        }
        case PacketType::STAMINA: {
            std::cout << "Packet is of type STAMINA" << std::endl;
            StaminaPacket& sta = to_packet<StaminaPacket>(*pkt);
            std::cout << "id: " << sta.id << std::endl;
            std::cout << "stamina: " << sta.stamina << std::endl;
            break;
        }
        case PacketType::HEALTH: {
            std::cout << "Packet is of type HEALTH" << std::endl;
            HealthPacket& hea = to_packet<HealthPacket>(*pkt);
            std::cout << "id: " << hea.id << std::endl;
            std::cout << "health: " << hea.health << std::endl;
            break;
        }
        case PacketType::USE: {
            std::cout << "Packet is of type USE" << std::endl;
            UsePacket& use = to_packet<UsePacket>(*pkt);
            std::cout << "id: " << use.id << std::endl;
            std::cout << "use: " << use.use << std::endl;
            break;
        }
    }
}
