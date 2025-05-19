#include <stdlib.h>
#include <stdio.h>

#include "byte_buffer.h"
#include "packets.h"

//https://chatgpt.com/share/68163d5f-c924-8003-97de-ff7cd7625af8

template<typename T>
T& to_packet(Packet& pkt) {
    return dynamic_cast<T&>(pkt); // Will throw std::bad_cast if type mismatch
};

// contains x and y position
MovementPacket::MovementPacket(int id, float x, float y)
    : id(id), x(x), y(y) {}

PacketType MovementPacket::type() const { return PacketType::MOVEMENT; }

std::string MovementPacket::serialize() const {
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

// contains angle and attacktype
AttackPacket::AttackPacket(int id, float attackType, float angle)
    : id(id), attackType(attackType), angle(angle) {}

PacketType AttackPacket::type() const { return PacketType::ATTACK; }

std::string AttackPacket::serialize() const {
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

// contains current stamina

StaminaPacket::StaminaPacket(int id, float stamina)
    : id(id), stamina(stamina) {}

PacketType StaminaPacket::type() const { return PacketType::STAMINA; }

std::string StaminaPacket::serialize() const {
    ByteBuffer buffer;
    uint32_t payload_size = sizeof(id) + sizeof(stamina);

    buffer.put<uint32_t>(payload_size);
    buffer.put<uint16_t>(static_cast<uint16_t>(type()));
    buffer.put<int>(id);
    buffer.put<float>(stamina);

    std::string str(buffer.data.begin(), buffer.data.end());
    return str;
}

// contains current health

HealthPacket::HealthPacket(int id, float health)
    : id(id), health(health) {}

PacketType HealthPacket::type() const { return PacketType::HEALTH; }

std::string HealthPacket::serialize() const {
    ByteBuffer buffer;
    uint32_t payload_size = sizeof(id) + sizeof(health);

    buffer.put<uint32_t>(payload_size);
    buffer.put<uint16_t>(static_cast<uint16_t>(type()));
    buffer.put<int>(id);
    buffer.put<float>(health);

    std::string str(buffer.data.begin(), buffer.data.end());
    return str;
}


// contains item used
UsePacket::UsePacket(int id, std::string use)
    : id(id), use(use) {}

PacketType UsePacket::type() const { return PacketType::USE; }

std::string UsePacket::serialize() const {
    ByteBuffer buffer;
    uint32_t payload_size = sizeof(id) + use.size();

    buffer.put<uint32_t>(payload_size);
    buffer.put<uint16_t>(static_cast<uint16_t>(type()));
    buffer.put<int>(id);
    buffer.data.insert(buffer.data.end(), use.begin(), use.end());

    std::string str(buffer.data.begin(), buffer.data.end());
    return str;
}


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
            throw "Unknown PacketType";
    };
}

int main() {
    UsePacket packet(2, "hello");

    std::string packet_string = packet.serialize();
    std::unique_ptr<Packet> pkt = Packet::deserialize(packet_string);

    switch (pkt->type()) {
        case PacketType::MOVEMENT: {
            fprintf(stdout, "Packet is of type MOVEMENT\n");
            MovementPacket& mov = to_packet<MovementPacket>(*pkt);
            fprintf(stdout, "id: %i\n", mov.id);
            fprintf(stdout, "x: %f\n", mov.x);
            fprintf(stdout, "y: %f\n", mov.y);
            break;
        }
        case PacketType::ATTACK: {
            fprintf(stdout, "Packet is of type ATTACK\n");
            AttackPacket& atk = to_packet<AttackPacket>(*pkt);
            fprintf(stdout, "id: %i\n", atk.id);
            fprintf(stdout, "attackType: %i\n", atk.attackType);
            fprintf(stdout, "angle: %f\n", atk.angle);
            break;
        }
        case PacketType::STAMINA: {
            fprintf(stdout, "Packet is of type STAMINA\n");
            StaminaPacket& sta = to_packet<StaminaPacket>(*pkt);
            fprintf(stdout, "id: %i\n", sta.id);
            fprintf(stdout, "stamina: %f\n", sta.stamina);
            break;
        }
        case PacketType::HEALTH: {
            fprintf(stdout, "Packet is of type HEALTH\n");
            HealthPacket& hea = to_packet<HealthPacket>(*pkt);
            fprintf(stdout, "id: %i\n", hea.id);
            fprintf(stdout, "health: %f\n", hea.health);
            break;
        }
        case PacketType::USE: {
            fprintf(stdout, "Packet is of type USE\n");
            UsePacket& use = to_packet<UsePacket>(*pkt);
            fprintf(stdout, "id: %i\n", use.id);
            fprintf(stdout, "use: %s\n", use.use.data());
            break;
        }
    }
}
