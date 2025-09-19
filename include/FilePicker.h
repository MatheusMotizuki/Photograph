#ifndef FILEPICKER_H
#define FILEPICKER_H

#include <imgui.h>
#include <imgui_internal.h>
#include <filesystem>
#include <vector>
#include <string>

class FilePicker {
private:
    std::filesystem::path current_path;
    std::filesystem::path parent_path;
    std::string current_file;

    std::string selected_file_str;

public:
    ImGuiIO& io = ImGui::GetIO();
    ImFont* consolas = nullptr;

    const char* selected_file;

    inline const char* GetSelectedFile() const { return selected_file; }

    FilePicker() {
        current_path = std::filesystem::current_path();
        parent_path = current_path.parent_path();
        current_file = "";
        selected_file = "";
        selected_file_str = "";
    }

    inline void ShowFileDialog(bool* open) {
        if (!*open) return;

        if (!io.Fonts->Fonts.empty()) consolas = io.Fonts->Fonts.back();
        if (consolas) ImGui::PushFont(consolas);

        ImGui::SetNextWindowSize(ImVec2(760.0f, 400.0f));
        ImGui::SetNextWindowFocus();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.0f, 10.0f));

        ImGui::PushStyleColor(ImGuiCol_WindowBg,        IM_COL32(18, 18, 18, 240));
        ImGui::PushStyleColor(ImGuiCol_Border,          IM_COL32(90, 90, 90, 255));
        ImGui::PushStyleColor(ImGuiCol_TitleBg,         IM_COL32(30, 30, 30, 255));
        ImGui::PushStyleColor(ImGuiCol_TitleBgActive,   IM_COL32(40, 40, 40, 255));
        ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed,IM_COL32(15, 15, 15, 200));
        ImGui::PushStyleColor(ImGuiCol_FrameBg,         IM_COL32(20, 20, 20, 255));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered,  IM_COL32(60, 60, 60, 255));
        ImGui::PushStyleColor(ImGuiCol_Header,          IM_COL32(30, 30, 30, 255));

        ImGui::Begin("File Picker", open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

        ImGui::PushStyleColor(ImGuiCol_Header,          IM_COL32(24, 24, 24, 255));
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered,   IM_COL32(48, 48, 48, 255));
        ImGui::PushStyleColor(ImGuiCol_HeaderActive,    IM_COL32(72, 72, 72, 255));
        ImGui::PushStyleColor(ImGuiCol_Border,          IM_COL32(0, 0, 0, 0));

        const std::string abs_path = std::filesystem::exists(current_path) ? std::filesystem::absolute(current_path).string() : "";
        ImGui::Text("Current Path -> %s", abs_path.c_str());

        ImGui::Separator();

        float window_height = ImGui::GetWindowHeight();
        float button_height = ImGui::GetFrameHeightWithSpacing();
        float child_height = window_height - ImGui::GetCursorPosY() - button_height - 20.0f;

        ImGui::BeginChild("##parents", ImVec2(180, child_height), true);
        if (ImGui::BeginListBox("##", ImVec2(-1, -1))) {
            parent_path = current_path.parent_path();

            if(current_path != current_path.root_path()) {
                if (ImGui::Selectable("..", false, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {
                    if (ImGui::IsMouseDoubleClicked(0)) {
                        current_path = parent_path;
                    }
                }
            }

            for (auto &entry : std::filesystem::directory_iterator(parent_path)) {
                if (!entry.is_directory()) continue;
                std::string name = entry.path().filename().string() + "/";
                std::string id = "##" + entry.path().string();
                if (ImGui::Selectable((name + id).c_str(), false, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick, ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
                    current_path = entry.path();
                }
            }

            ImGui::EndListBox();
        }
        ImGui::EndChild();

        ImGui::SameLine();
        ImGui::BeginChild("##actual", ImVec2(-1, child_height), true);
        if (ImGui::BeginListBox("##", ImVec2(-1, -1))) {
            for (auto &entry : std::filesystem::directory_iterator(current_path)) {
                std::string name = entry.path().filename().string();
                std::string id = "##" + entry.path().string();
                bool is_selected = (!current_file.empty() && std::filesystem::path(current_file) == entry.path());
                if (entry.is_directory()) {
                    if (ImGui::Selectable((name + "/" + id).c_str(), false, ImGuiSelectableFlags_AllowDoubleClick)) {
                        if (ImGui::IsMouseDoubleClicked(0)) {
                            current_path = entry.path();
                            current_file.clear();
                        }
                    }
                } else {
                    auto ext = entry.path().extension().string();
                    if (ext == ".png" || ext == ".jpg" || ext == ".jpeg") {
                        if (ImGui::Selectable((name + id).c_str(), is_selected, ImGuiSelectableFlags_AllowDoubleClick)) {
                            current_file = entry.path().string();
                        }
                    }
                }
            }
            ImGui::EndListBox();
        }
        ImGui::EndChild();

        if (ImGui::Button("Close")) {
            *open = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Select")) {
                if (!current_file.empty()) {
                    selected_file_str = current_file;
                    selected_file = selected_file_str.c_str();
                    *open = false;
                }
            }

        if (consolas) ImGui::PopFont();
        ImGui::PopStyleColor(4);
        ImGui::PopStyleColor(8);
        ImGui::PopStyleVar(4);

        ImGui::End();
    }
};

#endif // FILEPICKER_H