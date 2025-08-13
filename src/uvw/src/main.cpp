/**
 * Copyright 2022 xiaozhuai
 */

#include <cstdio>
#include <memory>

#include "uvw.hpp"

class Discover {
public:
    void start() {
        loop_ = uvw::loop::create();
        start_server();
        start_client();
        start_signal();
        loop_->run();
    }

    void stop() {
        stop_signal();
        stop_client();
        stop_server();
        loop_->stop();
        loop_->close();
        loop_.reset();
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

    void stop_client() {
        timer_->stop();
        timer_->close();
        timer_->reset();
        udp_client_->stop();
        udp_client_->close();
        udp_client_.reset();
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

    void stop_server() {
        udp_server_->stop();
        udp_server_->close();
        udp_server_.reset();
    }

    void start_signal() {
        sigint_handle_ = loop_->resource<uvw::signal_handle>();
        sigint_handle_->on<uvw::signal_event>([&](const uvw::signal_event &event, uvw::signal_handle &) {
            if (event.signum == SIGINT) {
                printf("\rReceived SIGINT, stopping...\n");
                stop();
            }
        });
        sigint_handle_->start(SIGINT);

        sigterm_handle_ = loop_->resource<uvw::signal_handle>();
        sigterm_handle_->on<uvw::signal_event>([&](const uvw::signal_event &event, uvw::signal_handle &) {
            if (event.signum == SIGTERM) {
                printf("\rReceived SIGTERM, stopping...\n");
                stop();
            }
        });
        sigterm_handle_->start(SIGTERM);
    }

    void stop_signal() {
        sigint_handle_->stop();
        sigint_handle_->close();
        sigint_handle_->reset();
        sigterm_handle_->stop();
        sigterm_handle_->close();
        sigterm_handle_->reset();
    }

private:
    static constexpr uint16_t kBroadcastPort = 7550;
    static constexpr const char *kBroadcastAddress = "255.255.255.255";
    static constexpr const char *kBindAddress = "0.0.0.0";
    std::shared_ptr<uvw::loop> loop_;
    std::shared_ptr<uvw::timer_handle> timer_;
    std::shared_ptr<uvw::udp_handle> udp_client_;
    std::shared_ptr<uvw::udp_handle> udp_server_;
    std::shared_ptr<uvw::signal_handle> sigint_handle_;
    std::shared_ptr<uvw::signal_handle> sigterm_handle_;
};

int main() {
    Discover discover;
    discover.start();
    return 0;
}
