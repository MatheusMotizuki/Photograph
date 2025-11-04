#pragma once

#include <ixwebsocket/IXWebSocket.h>
#include "json.hpp"
#include "imgui.h"
#include "imnodes.h"
#include <mutex>
#include <string>
#include <memory>
#include <atomic>
#include <iostream>
#include <unordered_map>

#include "node/NodeMenu.hpp"

class WebSocketClient {
public:
    enum class Action { None, Join, Create };

    std::unordered_map<std::string, ImVec2> getAllRemoteMousePositions() const {
        std::lock_guard<std::mutex> lock(m_positionsMutex);
        return m_remoteMousePositions;
    }

    explicit WebSocketClient(const std::string& url)
        : m_url(url), m_running(false), m_pendingAction(Action::None) {}

    ~WebSocketClient() {
        disconnect();
    }

    void join(const std::string& room) {
        this->setup(room, Action::Join);
    }

    void create(const std::string& room) {
        this->setup(room, Action::Create);
    }

    void sendMouse(const std::string& room, const ImVec2& cursor) { 
        info(room, cursor); 
    }

    void newNode(std::string& room, const int& nodeID, const NodeMenu::NodeType& NodeType, ImVec2& position) {
        createNode(room, nodeID, NodeType, position);
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
    mutable std::mutex m_positionsMutex;
    std::unordered_map<std::string, ImVec2> m_remoteMousePositions;

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
                    j["topic"] = (m_pendingAction == Action::Create) ? "room.create" : "room.join";
                    m_webSocket->send(j.dump());
                }
                return;
            }
            
            if (msg->type == ix::WebSocketMessageType::Error) {
                std::cout << "❌ Error: " << msg->errorInfo.reason << std::endl;
                return;
            }
            
            if (msg->type == ix::WebSocketMessageType::Message) {
                std::cout << "📩 Message received\n";
                try {
                    auto j = nlohmann::json::parse(msg->str);

                    // Check for "topic" field
                    if (!j.contains("topic") || !j["topic"].is_string()) {
                        std::cout << "⚠️ No topic in message: " << j.dump() << std::endl;
                        return;
                    }
                    std::string topic = j["topic"].get<std::string>();

                    // Extract sender (required for multi-user)
                    std::string sender;
                    if (j.contains("sender") && j["sender"].is_string()) {
                        sender = j["sender"].get<std::string>();
                    }

                    // Normalize data location
                    const nlohmann::json* data_ptr = nullptr;
                    if (j.contains("data")) {
                        data_ptr = &j["data"];
                    } else if (j.contains("metadata")) {
                        data_ptr = &j["metadata"];
                    } else {
                        data_ptr = &j;
                    }

                    if (topic == "room.mouse") {
                        // Accept both array and object for coordinates
                        if (data_ptr->is_array()) {

                            ImVec2 cursor{
                                (*data_ptr)[0].get<float>(),
                                (*data_ptr)[1].get<float>()
                            };

                            std::lock_guard<std::mutex> lock(m_positionsMutex);
                            m_remoteMousePositions[sender.empty() ? "unknown" : sender] = cursor;

                        } else {
                            std::cout << "⚠️ Received room.mouse without coordinates: " << j.dump() << std::endl;
                        }
                    } else if (topic == "room.create.node") {
                        // Handle node creation (add your logic here)
                        std::cout << "🟩 Node creation event: " << j.dump() << std::endl;
                        // Example: extract node info if needed
                        // int node_id = (*data_ptr)["node_id"].get<int>();
                        // int node_type = (*data_ptr)["node_type"].get<int>();
                        // ImVec2 pos{(*data_ptr)["node_position_x"].get<float>(), (*data_ptr)["node_position_y"].get<float>()};
                        // ... your handling code ...
                    } else {
                        std::cout << "⚠️ Unhandled topic: " << topic << " | " << j.dump() << std::endl;
                    }
                } catch (const std::exception& e) {
                    std::cout << "⚠️ JSON parse error: " << e.what() << std::endl;
                }
            }
        });

        m_webSocket->start();
        m_running = true;
    }

    void info(const std::string& room, const ImVec2& cursor) {
        if (!m_running.load() || !m_webSocket || room.empty()) return;

        nlohmann::json msg;
        msg["topic"] = "room.mouse";
        msg["room_id"] = room;
        msg["mousex"] = cursor.x;
        msg["mousey"] = cursor.y;
        
        m_webSocket->send(msg.dump());
    }

    void createNode(std::string& room, const int& nodeID, const NodeMenu::NodeType& NodeType, ImVec2& position) {
        if (!m_running.load() || !m_webSocket || room.empty()) return;

        nlohmann::json msg;
        msg["topic"] = "room.create.node";
        msg["room_id"] = room;
        msg["node_id"] = nodeID;
        msg["node_type"] = NodeType;
        msg["node_position_x"] = position.x;
        msg["node_position_y"] = position.y;

        m_webSocket->send(msg.dump());
    }

    std::string m_url;
    std::unique_ptr<ix::WebSocket> m_webSocket;
    std::atomic<bool> m_running;
    Action m_pendingAction;
    std::string m_pendingRoom;
};