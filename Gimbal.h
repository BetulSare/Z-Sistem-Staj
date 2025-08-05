#ifndef GIMBAL_H
#define GIMBAL_H

#include <string>
#include <netinet/in.h>

class Gimbal {
public:
    Gimbal(const std::string& ip = "192.168.1.253", int port = 4000);
    ~Gimbal();

    bool connect();
    bool sendPacket(const std::vector<uint8_t>& data);
    bool startRTSPStream();
    bool receivePacket();
    void closeSocket();
    
private:
    std::string gimbal_ip;
    int gimbal_port;
    int sockfd;
    struct sockaddr_in gimbal_addr;
};

#endif
