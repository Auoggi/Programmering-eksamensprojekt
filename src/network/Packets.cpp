
#include <variant>
#include <string>

enum class PacketType {
    MOVEMENT,
    ATTACK,
    STAMINA,
    HEALTH,
    USE,
};

struct movementPacket {
    int id;
    float x;
    float y;
};

struct attackPacket {
    int id;
    float angle;
    int type;
};

struct staminaPacket {
    int id;
    float stamina;
};

struct healthPacket {
    int id;
    float health;
};

struct usePacket {
    int id;
    std::string use;
};



using PacketType = std::variant<movementPacket, attackPacket, staminaPacket, healthPacket, usePacket>;


