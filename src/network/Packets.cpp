
#include <variant>
#include <string>
#include <iostream>
#include <format>
#include <sstream>

enum class PacketType {
    MOVEMENT,
    ATTACK,
    STAMINA,
    HEALTH,
    USE,
};

struct packetHeader {
    int id;
};

struct movementPacket : packetHeader {
    float x,y;
    float* pos[2] = {&x, &y};
};

struct attackPacket : packetHeader {
    int type;
    float angle;
};

struct staminaPacket : packetHeader {
    float stamina;
};

struct healthPacket : packetHeader {
    float health;
};

struct usePacket : packetHeader {
    std::string use;
};

using packetType = std::variant<movementPacket, attackPacket, staminaPacket, healthPacket, usePacket>;


std::string createPacket(packetType packet) {

    // gets packet header
    packetHeader header;
    std::visit([&header](auto&& pkt) {
        packetHeader& base = pkt;
        header = base;
    }, packet);

    std::ostringstream msg;
    msg << "id=" << header.id << "\n";

    msg << "type=";
    if (std::holds_alternative<movementPacket>(packet)) {
        movementPacket movement = std::get<movementPacket>(packet);
        msg << "movement" << "\n";
        
        // payload
        msg << "\n";
        msg << movement.x << "\n";
        msg << movement.y << "\n";
    }
    else if (std::holds_alternative<attackPacket>(packet)) {
        attackPacket attack = std::get<attackPacket>(packet);
        msg << "attack" << "\n";
        
        // payload
        msg << "\n";
        msg << attack.type << "\n";
        msg << attack.angle << "\n";
    }
    else if (std::holds_alternative<staminaPacket>(packet)) {
        staminaPacket stamina = std::get<staminaPacket>(packet);
        msg << "stamina" << "\n";
        
        // payload
        msg << "\n";
        msg << stamina.stamina << "\n";
    }
    else if (std::holds_alternative<healthPacket>(packet)) {
        healthPacket health = std::get<healthPacket>(packet);
        msg << "health" << "\n";
        
        // payload
        msg << "\n";
        msg << health.health << "\n";
    }
    else if (std::holds_alternative<usePacket>(packet)) {
        usePacket use = std::get<usePacket>(packet);
        msg << "use" << "\n";
        
        // payload
        msg << "\n";
        msg << use.use << "\n";
    }
    
    return msg.str();
}

std::string msg = "Hello,\n"
                  "This is line 2\n"
                  "And this is line 3\n";

packetType parsePacket(std::string) {

}

int main() {
    movementPacket packet;
    packet.id = 2;
    packet.x = 5.5;
    packet.y = 8.2;

    std::cout << createPacket(packet) << std::endl;
}

