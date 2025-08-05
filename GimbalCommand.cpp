#include "GimbalCommand.h"
#include <iostream>  
#include <cstdio>    
uint8_t GimbalCommand::counter = 0;

std::vector<uint8_t> GimbalCommand::getPacket(uint8_t frameID, const std::vector<uint8_t>& payload) {
    std::vector<uint8_t> packet;

    // 1. Sabit header
    packet.push_back(0x55);
    packet.push_back(0xAA);
    packet.push_back(0xDC);

    // 2. Uzunluk ve sayaç
    uint8_t length = static_cast<uint8_t>(payload.size() + 2);  // FrameID + checkword
    uint8_t cnt = getCounter();
    uint8_t lenCnt = (length & 0x3F) | ((cnt & 0x03) << 6);     // Bit0~5: length, Bit6~7: counter
    packet.push_back(lenCnt);

    // 3. Frame ID
    packet.push_back(frameID);

    // 4. Payload
    packet.insert(packet.end(), payload.begin(), payload.end());

    // 5. Checkword: XOR from lenCnt to end
    std::vector<uint8_t> toCheck(packet.begin() + 3, packet.end());
    uint8_t check = calculateCheckWord(toCheck);
    packet.push_back(check);

    // Paket içeriğini hex formatında ekrana yazdır
    std::cout << "Oluşturulan paket: ";
    for (uint8_t byte : packet) {
        std::printf("0x%02X ", byte);
    }
    std::cout << std::endl;

    return packet;
}

uint8_t GimbalCommand::getCounter() {
    uint8_t current = counter;
    counter = (counter + 1) & 0x03;  // 0–3 arasında döner
    return current;
}

uint8_t GimbalCommand::calculateCheckSum(const std::vector<uint8_t>& data) {
    uint8_t result = 0x00;
    for (uint8_t byte : data) {
        result ^= byte;
    }
    return result;
}
