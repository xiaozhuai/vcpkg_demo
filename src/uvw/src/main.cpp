/**
 * Copyright 2022 xiaozhuai
 */

#include <cstdio>
#include <memory>

#include "uvw.hpp"

class Discover {
public:
    void start() {
        loop_ = uvw::loop::get_default();
        start_server();
        start_client();
        loop_->run();
    }

private:
    void start_client() {
        timer_ = loop_->resource<uvw::timer_handle>();
        udp_client_ = loop_->resource<uvw::udp_handle>();
        udp_client_->bind(kBindAddress, 0);
        udp_client_->broadcast(true);
        timer_->on<uvw::timer_event>([&](const uvw::timer_event &, uvw::timer_handle &) {
            std::string msg = "Ping";
            udp_client_->send(kBroadcastAddress, kBroadcastPort, msg.data(), msg.length());
            printf("Client Send: %s to %s:%d\n", msg.c_str(), kBroadcastAddress, kBroadcastPort);
        });
        udp_client_->on<uvw::udp_data_event>([](const uvw::udp_data_event &event, uvw::udp_handle &) {
            printf("Client Recv: %s from %s:%d\n", std::string(event.data.get(), event.length).c_str(),
                   event.sender.ip.c_str(), event.sender.port);
        });
        timer_->start(uvw::timer_handle::time{0}, uvw::timer_handle::time{1000});
        udp_client_->recv();
    }

    void start_server() {
        udp_server_ = loop_->resource<uvw::udp_handle>();
        udp_server_->bind(kBindAddress, kBroadcastPort);
        udp_server_->on<uvw::udp_data_event>([&](const uvw::udp_data_event &event, uvw::udp_handle &handle) {
            printf("Server Recv: %s from %s:%d\n", std::string(event.data.get(), event.length).c_str(),
                   event.sender.ip.c_str(), event.sender.port);
            std::string msg = "Pong";
            handle.send(event.sender, msg.data(), msg.length());
            printf("Server Reply: %s to %s:%d\n", msg.c_str(), event.sender.ip.c_str(), event.sender.port);
        });
        udp_server_->recv();
    }

private:
    static constexpr uint16_t kBroadcastPort = 7550;
    static constexpr const char *kBroadcastAddress = "255.255.255.255";
    static constexpr const char *kBindAddress = "0.0.0.0";
    std::shared_ptr<uvw::loop> loop_;
    std::shared_ptr<uvw::timer_handle> timer_;
    std::shared_ptr<uvw::udp_handle> udp_client_;
    std::shared_ptr<uvw::udp_handle> udp_server_;
};

int main() {
    Discover discover;
    discover.start();
    return 0;
}
