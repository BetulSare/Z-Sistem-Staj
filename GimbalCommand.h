#ifndef GIMBALCOMMAND_H
#define GIMBALCOMMAND_H

#include <vector>
#include <cstdint>

class GimbalCommand {
public:
    static std::vector<uint8_t> getPacket(uint8_t frameID, const std::vector<uint8_t>& payload);

private:
    static uint8_t counter;
    static uint8_t getCounter();
    static uint8_t calculateCheckSum(const std::vector<uint8_t>& data);
};

#endif
