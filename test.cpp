#include "Gimbal.h"
#include "GimbalCommand.h"
#include <iostream>
#include <vector>

int main() {
    Gimbal gimbal;

    if (!gimbal.connect()) {
        std::cerr << "Gimbal bağlantısı başarısız!\n";
        return 1;
    }else{
        std::cout << "Gimbal bağlantısı başarıyla kuruldu!\n";
    }

    if (!gimbal.startRTSPStream()) {
        std::cerr << "RTSP akışı başlatılamadı!\n";
        return 1;
    }else{
        std::cout << "RTSP akışı başarıyla başlatıldı!\n";
    }
 
 /*
    uint8_t frameID = 0x30;

    std::vector<uint8_t> payload = {
        0xA1, 0x05,       // Servo command + subcommand (azimuth scan)
        0xD0, 0x07,       // Azimuth speed (20.00°/s)
        0xF4, 0x01,       // Pitch speed (5.00°/s)
        0x58, 0x15,       // Amplitude (30°)
        0x00, 0x00        // Center (0°)
    };

   std::vector<uint8_t> packet = GimbalCommand::getPacket(frameID, payload);

    if (!gimbal.sendPacket(packet)) {
        std::cerr << "Paket Gönderilemedi!\n";
        return 1;
    }else{
        std::cout << "Paket başarıyla gönderildi!\n";
    }

    if(!gimbal.receivePacket()) {
        std::cerr << "Paket alınamadı!\n";
        return 1;
    }else{
        std::cout << "Paket başarıyla alındı!\n";   
    }
 */
    gimbal.closeSocket();   

    return 0;
}