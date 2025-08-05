#include "Gimbal.h"
#include <iostream>
#include <cstring>      
#include <arpa/inet.h>  
#include <unistd.h>    
#include <vector>
#include <sys/socket.h> 
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp> 

Gimbal::Gimbal()
    : gimbal_ip("192.168.1.253"), gimbal_port(4000), sockfd(-1) {
    memset(&gimbal_addr, 0, sizeof(gimbal_addr));
}

bool Gimbal::receivePacket() {
    if (sockfd < 0) {
        std::cerr << "Bağlantı yok.\n";
        return false;
    }

    constexpr size_t BUFFER_SIZE = 1024;
    std::vector<uint8_t> buffer(BUFFER_SIZE);

    struct sockaddr_in sender_addr;
    socklen_t sender_len = sizeof(sender_addr);

    ssize_t recv_len = recvfrom(sockfd, buffer.data(), buffer.size(), 0,
                                (struct sockaddr*)&sender_addr, &sender_len);
    if (recv_len < 0) {
        std::cerr << "Veri alımı başarısız!\n";
        return false;
    }

    std::cout << "Gimbal'dan " << recv_len << " byte veri alındı: ";

    // Gelen veriyi hex olarak yazdır
    for (ssize_t i = 0; i < recv_len; ++i) {
        printf("%02X ", buffer[i]);
    }
    std::cout << std::endl;

    return true;
}

bool Gimbal::startRTSPStream() {
    cv::VideoCapture cap("rtsp://192.168.1.253:554/stream1");

    if (!cap.isOpened()) {
        std::cerr << "RTSP akışı başlatılamadı: " << "rtsp://192.168.1.253:554/stream1" << std::endl;
        return false;
    }
    std::cout << "RTSP akışı başlatıldı: " << "rtsp://192.168.1.253:554/stream1" << std::endl;
    cv::Mat frame;
    while (true) {
        if (!cap.read(frame)) {
            std::cerr << "Görüntü alınamadı." << std::endl;
            break;
        }
        cv::imshow("Gimbal RTSP Stream", frame);
        // 'q' tuşuna basılırsa çık
        if (cv::waitKey(1) == 'q') {
            std::cout << "Görüntü akışı durduruldu." << std::endl;
            break;
        }
    }
    cap.release();
    cv::destroyAllWindows();
    return true;
}

bool Gimbal::connect() {
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "UDP soket oluşturulamadı!\n";
        return false;
    }

    gimbal_addr.sin_family = AF_INET;
    gimbal_addr.sin_port = htons(gimbal_port);
    if (inet_pton(AF_INET, gimbal_ip.c_str(), &gimbal_addr.sin_addr) <= 0) {
        std::cerr << "Geçersiz IP adresi!\n";
        return false;
    }

    std::cout << "Gimbal bağlantısı hazır: " << gimbal_ip << ":" << gimbal_port << std::endl;
    return true;
}

bool Gimbal::sendPacket(const std::vector<uint8_t>& data) {
    if (sockfd < 0) {
        std::cerr << "Bağlantı yok. Önce connect() çağırılmalı.\n";
        return false;
    }

    ssize_t sent = sendto(sockfd, data.data(), data.size(), 0,
                          (struct sockaddr*)&gimbal_addr, sizeof(gimbal_addr));

    if (sent < 0) {
        std::cerr << "Veri gönderimi başarısız!\n";
        return false;
    }

    std::cout << "Veri gönderildi: " << sent << " byte\n";
    return true;
}

void Gimbal::closeSocket() {
    if (sockfd >= 0) {
        close(sockfd);
        sockfd = -1;
        std::cout << "Gimbal bağlantısı kapatıldı.\n";
    }
}
