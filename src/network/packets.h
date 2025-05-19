#ifndef PACKETS
#define PACKETS

#include <string>
#include <memory>
#include <cstring>

enum class PacketType : uint16_t {
    MOVEMENT,
    ATTACK,
    STAMINA,
    HEALTH,
    USE,
};

// packet:
// [ length: 4-byte ][ type: 1-byte ][ payload... ]
class Packet {
public:
    virtual ~Packet() = default;

    virtual PacketType type() const = 0;
    virtual std::string serialize() const = 0;

    static std::unique_ptr<Packet> deserialize(std::string str);
};

// contains x and y position
class MovementPacket : public Packet {
public:
    MovementPacket(int id, float x, float y);
    
    PacketType type() const override;
    std::string serialize() const override;

    int id;
    float x, y;
};

// contains angle and attacktype
class AttackPacket : public Packet {
public:
    AttackPacket(int id, float attackType, float angle);

    PacketType type() const override;
    std::string serialize() const override;
    
    int id, attackType;
    float angle;
};

// contains current stamina
class StaminaPacket : public Packet {
public:
    StaminaPacket(int id, float stamina);

    PacketType type() const override;
    std::string serialize() const override;
    
    int id;
    float stamina;
};

// contains current health
class HealthPacket : public Packet {
public:
    HealthPacket(int id, float health);

    PacketType type() const override;
    std::string serialize() const override;
    
    int id;
    float health;
};

// contains item used
class UsePacket : public Packet {
public:
    UsePacket(int id, std::string use);

    PacketType type() const override;
    std::string serialize() const override;
    
    int id;
    std::string use;
};

#endif