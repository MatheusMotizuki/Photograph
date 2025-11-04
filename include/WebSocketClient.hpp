#pragma once

#include <ixwebsocket/IXWebSocket.h>
#include "json.hpp"
#include "imgui.h"
#include "imnodes.h"
#include <string>
#include <memory>
#include <atomic>
#include <iostream>

class WebSocketClient {
public:
    enum class Action { None, Join, Create };
    ImVec2 getRemoteMousePos() const { return m_remoteMousePos; }

    explicit WebSocketClient(const std::string& url)
        : m_url(url), m_running(false), m_pendingAction(Action::None) {}

    ~WebSocketClient() {
        disconnect();
    }

    // Prepare to join a room
    void join(const std::string& room) {
        this->setup(room, Action::Join);
    }

    // Prepare to create a room
    void create(const std::string& room) {
        this->setup(room, Action::Create);
    }

    void sendMouse(std::string& room, ImVec2& cursor) {
        this->info(room, cursor);
    }

    void disconnect() {
        if (!m_running.load()) return;
        if (m_webSocket) {
            m_webSocket->stop();
            m_webSocket.reset();
        }
        m_running = false;
        m_pendingAction = Action::None;
        m_pendingRoom.clear();
    }

private:
    ImVec2 m_remoteMousePos = ImVec2(-1, -1); // Default to invalid

    void setup(const std::string& room, Action action) {
        if (m_running.load()) return;
        m_webSocket = std::make_unique<ix::WebSocket>();
        m_webSocket->setUrl(m_url);
        m_pendingAction = action;
        m_pendingRoom = room;

        m_webSocket->setOnMessageCallback([this](const ix::WebSocketMessagePtr& msg) {
            if (!msg) return;
            if (msg->type == ix::WebSocketMessageType::Open) {
                if (!m_pendingRoom.empty()) {
                    nlohmann::json j;
                    j["room_id"] = m_pendingRoom;
                    if (m_pendingAction == Action::Create) {
                        j["topic"] = "room.create";
                    } else if (m_pendingAction == Action::Join) {
                        j["topic"] = "room.join";
                    }
                    std::cout << "Sending: " << j.dump() << std::endl;
                    m_webSocket->send(j.dump());
                }
            }
            if (msg->type == ix::WebSocketMessageType::Message) {
                std::cout << "📩 Message received: " << msg->str << std::endl;
                try {
                    auto j = nlohmann::json::parse(msg->str);
                    ImVec2 cursor{};
                    if (j.contains("metadata")) {
                        const auto& metadata = j["metadata"];
                        if (metadata.contains("mousex") && metadata["mousex"].is_number()) {
                            cursor.x = metadata["mousex"];
                        }
                        if (metadata.contains("mousey") && metadata["mousey"].is_number()) {
                            cursor.y = metadata["mousey"];
                        }
                        m_remoteMousePos = cursor;
                    } else if (j.contains("mousex") && j.contains("mousey") &&
                            j["mousex"].is_number() && j["mousey"].is_number()) {
                        cursor.x = j["mousex"];
                        cursor.y = j["mousey"];
                        m_remoteMousePos = cursor;
                    } else {
                        std::cout << "Received JSON: " << j.dump() << std::endl;
                    }
                } catch (const std::exception& e) {
                    std::cout << "⚠️ JSON parse error: " << e.what() << std::endl;
                }
            }
            if (msg->type == ix::WebSocketMessageType::Error) {
                std::cout << "❌ Error: " << msg->errorInfo.reason << std::endl;
            }
        });

        m_webSocket->start();
        m_running = true;
    }

    // this will send information about the current node position
    void info(std::string& room, ImVec2& cursor) {
        if (!m_running.load()) return;
        if (!m_webSocket) return;
        if (room.empty()) return;

        std::cout << "room id is: " << room << std::endl;

        nlohmann::json msg;
        msg["topic"] = "room.broadcast";
        msg["room_id"] = room;
        // metadata is a any, in the server so we can send anything
        msg["metadata"] = {
            {"mousex", cursor.x},
            {"mousey", cursor.y}
        };
        m_webSocket->send(msg.dump());
    }

    std::string m_url;
    std::unique_ptr<ix::WebSocket> m_webSocket;
    std::atomic<bool> m_running;
    Action m_pendingAction;
    std::string m_pendingRoom;
};