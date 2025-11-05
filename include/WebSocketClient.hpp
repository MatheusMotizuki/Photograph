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
#include <functional>
#include <unordered_map>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Easy.hpp>

#include "stb_image_write.h"

class WebSocketClient {
public:
    enum class Action { None, Join, Create };

    // Callback types
    using NodeCreatedCallback = std::function<void(int node_id, int node_type, ImVec2 pos)>;
    using LinkCreatedCallback = std::function<void(int link_id, int start_attr, int end_attr)>;
    using MouseMovedCallback = std::function<void(const std::string& sender, ImVec2 pos)>;
    using SelectedNodeCallback = std::function<void(int node_id, ImVec2 pos)>;
    using NodeDeletedCallback = std::function<void(int node_id)>;
    using LinkDeletedCallback = std::function<void(int link_id)>;
    using ImageUploadCallback = std::function<void(std::string url, int width, int height, int channels)>;

    // Setters for callbacks
    void setNodeCreatedCallback(NodeCreatedCallback cb) { m_nodeCreatedCallback = std::move(cb); }
    void setLinkCreatedCallback(LinkCreatedCallback cb) { m_linkCreatedCallback = std::move(cb); }
    void setMouseMovedCallback(MouseMovedCallback cb) { m_mouseMovedCallback = std::move(cb); }
    void setSelectedNodeCallback(SelectedNodeCallback cb) { m_selectedNodeCallback = std::move(cb); }
    void setNodeDeletedCallback(NodeDeletedCallback cb) { m_nodeDeletedCallback = std::move(cb); }
    void setLinkDeletedCallback(LinkDeletedCallback cb) { m_linkDeletedCallback = std::move(cb); }
    void setImageUploadCallback(ImageUploadCallback cb) { m_imageUploadCallback = std::move(cb); }

    explicit WebSocketClient(const std::string& url)
        : m_url(url), m_running(false), m_pendingAction(Action::None) {}

    ~WebSocketClient() { disconnect(); }

    void join(const std::string& room) { this->setup(room, Action::Join); }
    void create(const std::string& room) { this->setup(room, Action::Create); }

    void sendMouse(const std::string& room, const ImVec2& cursor) { info(room, cursor); }
    void newNode(const std::string& room, int nodeID, ImVec2 position, int nodeType) { createNode(room, nodeID, nodeType, position); }
    void newLink(const std::string& room, int linkID, int start_attr, int end_attr) { createLink(room, linkID, start_attr, end_attr); }
    void sendSelectedNode(const std::string& room, int nodeID, ImVec2 position) { selectNodePos(room, nodeID, position); }
    void sendDeadNode(const std::string& room, int nodeID) { deadNode(room, nodeID); }
    void sendDeadLink(const std::string& room, int linkID) { deadLink(room, linkID); }
    void sendImage(const std::string& room, std::vector<uint8_t> pixels, int width, int height, int channels) { imageInfo(room, pixels, width, height, channels); }

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

    NodeCreatedCallback m_nodeCreatedCallback;
    LinkCreatedCallback m_linkCreatedCallback;
    MouseMovedCallback m_mouseMovedCallback;
    SelectedNodeCallback m_selectedNodeCallback;
    NodeDeletedCallback m_nodeDeletedCallback;
    LinkDeletedCallback m_linkDeletedCallback;
    ImageUploadCallback m_imageUploadCallback;

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
                try {
                    auto j = nlohmann::json::parse(msg->str);

                    if (!j.contains("topic") || !j["topic"].is_string()) return;
                    std::string topic = j["topic"].get<std::string>();

                    std::string sender;
                    if (j.contains("sender") && j["sender"].is_string()) {
                        sender = j["sender"].get<std::string>();
                    }

                    const nlohmann::json* data_ptr = nullptr;
                    if (j.contains("data")) data_ptr = &j["data"];
                    else if (j.contains("metadata")) data_ptr = &j["metadata"];
                    else data_ptr = &j;

                    if (topic == "room.mouse.pos") {
                        ImVec2 cursor{ (*data_ptr)[0].get<float>(), (*data_ptr)[1].get<float>() };
                        {
                            std::lock_guard<std::mutex> lock(m_positionsMutex);
                            m_remoteMousePositions[sender.empty() ? "unknown" : sender] = cursor;
                        }
                        if (m_mouseMovedCallback) m_mouseMovedCallback(sender, cursor);
                    } else if (topic == "room.node.create") {
                        int node_id = (*data_ptr)[0].get<int>();
                        float pos_x = (*data_ptr)[1].get<float>();
                        float pos_y = (*data_ptr)[2].get<float>();
                        int node_type = (*data_ptr)[3].get<int>();
                        ImVec2 pos{pos_x, pos_y};
                        if (m_nodeCreatedCallback) m_nodeCreatedCallback(node_id, node_type, pos);
                    } else if (topic == "room.link.create") {
                        int link_id = (*data_ptr)[0].get<int>();
                        int start_attr = (*data_ptr)[1].get<int>();
                        int end_attr = (*data_ptr)[2].get<int>();
                        if (m_linkCreatedCallback) m_linkCreatedCallback(link_id, start_attr, end_attr);
                    } else if (topic == "room.node.select.pos") {
                        int node_id = (*data_ptr)[0].get<int>();
                        float pos_x = (*data_ptr)[1].get<float>();
                        float pos_y = (*data_ptr)[2].get<float>();
                        ImVec2 pos{pos_x, pos_y};
                        if (m_selectedNodeCallback) m_selectedNodeCallback(node_id, pos);
                    } else if (topic == "room.node.delete") {
                        int node_id = (*data_ptr)[0].get<int>();
                        if (m_nodeDeletedCallback) m_nodeDeletedCallback(node_id);
                    } else if (topic == "room.link.delete") {
                        int link_id = (*data_ptr)[0].get<int>();
                        if (m_linkDeletedCallback) m_linkDeletedCallback(link_id);
                    } else if (topic == "room.image.share") {
                        std::string url = (*data_ptr)[0].get<std::string>();
                        int width = (*data_ptr)[1].get<int>();
                        int height = (*data_ptr)[2].get<int>();
                        int channels = (*data_ptr)[3].get<int>();
                        if (m_imageUploadCallback) m_imageUploadCallback(url, width, height, channels);
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
        msg["topic"] = "room.mouse.pos";
        msg["room_id"] = room;
        msg["data"] = { cursor.x, cursor.y };
        m_webSocket->send(msg.dump());
    }

    void createNode(const std::string& room, int nodeID, int NodeType, ImVec2 position) {
        if (!m_running.load() || !m_webSocket || room.empty()) return;
        nlohmann::json msg;
        msg["topic"] = "room.node.create";
        msg["room_id"] = room;
        msg["data"] = { nodeID, position.x, position.y, NodeType };
        m_webSocket->send(msg.dump());
    }

    void createLink(const std::string& room, int linkID, int start_attr, int end_attr) {
        if (!m_running.load() || !m_webSocket || room.empty()) return;
        nlohmann::json msg;
        msg["topic"] = "room.link.create";
        msg["room_id"] = room;
        msg["data"] = { linkID, start_attr, end_attr };
        m_webSocket->send(msg.dump());
    }

    void selectNodePos(const std::string& room, int nodeID, ImVec2 position) {
        if (!m_running.load() || !m_webSocket || room.empty()) return;
        nlohmann::json msg;
        msg["topic"] = "room.node.select.pos";
        msg["room_id"] = room;
        msg["data"] = { nodeID, position.x, position.y };
        m_webSocket->send(msg.dump());
    }

    void deadNode(const std::string& room, int nodeID) {
        if (!m_running.load() || !m_webSocket || room.empty()) return;
        nlohmann::json msg;
        msg["topic"] = "room.node.delete";
        msg["room_id"] = room;
        msg["data"] = { nodeID };
        m_webSocket->send(msg.dump());
    }

    void deadLink(const std::string& room, int linkID) {
        if (!m_running.load() || !m_webSocket || room.empty()) return;
        nlohmann::json msg;
        msg["topic"] = "room.link.delete";
        msg["room_id"] = room;
        msg["data"] = { linkID };
        m_webSocket->send(msg.dump());
    }

    void imageInfo(const std::string& room, std::vector<uint8_t> pixels, int width, int height, int channels) {
        try{
            curlpp::Cleanup myCleanup;
            curlpp::Easy request;

            std::vector<uint8_t> compressed;
            auto write_func = [](void* context, void* data, int size) {
                auto* buf = static_cast<std::vector<uint8_t>*>(context);
                buf->insert(buf->end(), (uint8_t*)data, (uint8_t*)data + size);
            };
            int quality = 90;
            stbi_write_jpg_to_func(write_func, &compressed, width, height, channels, pixels.data(), quality);

            std::string url = "http://localhost:58058/upload";
            request.setOpt(curlpp::options::Url(url));
            std::string post_data(reinterpret_cast<char*>(compressed.data()), compressed.size());
            request.setOpt(curlpp::options::PostFields(post_data));
            request.setOpt(curlpp::options::PostFieldSize(post_data.size()));
            request.setOpt(curlpp::options::HttpHeader({"Content-Type: image/jpg"}));

            std::ostringstream response_stream;
            request.setOpt(curlpp::options::WriteStream(&response_stream));
            request.perform();
            std::string response = response_stream.str();

            auto resp = nlohmann::json::parse(response);
            std::string image_url = resp["id"];

            nlohmann::json msg;
            msg["topic"] = "room.image.share";
            msg["room_id"] = room;
            msg["data"] = { image_url, width, height, channels };
            m_webSocket->send(msg.dump());

        } catch (const std::exception& e) {
            std::cout << "❌ Image upload failed: " << e.what() << std::endl;
        }
    }

    std::string m_url;
    std::unique_ptr<ix::WebSocket> m_webSocket;
    std::atomic<bool> m_running;
    Action m_pendingAction;
    std::string m_pendingRoom;
};