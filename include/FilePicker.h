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

        // Modern window styling
        ImGui::SetNextWindowSize(ImVec2(900.0f, 550.0f));
        ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        // Clean modern style variables
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 6.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.0f, 8.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(12.0f, 8.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 6.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding, 3.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, 3.0f);

        // Monochrome black minimalistic palette
        ImGui::PushStyleColor(ImGuiCol_WindowBg,          IM_COL32(10, 10, 10, 255));
        ImGui::PushStyleColor(ImGuiCol_Border,            IM_COL32(30, 30, 30, 180));
        ImGui::PushStyleColor(ImGuiCol_TitleBg,           IM_COL32(15, 15, 15, 255));
        ImGui::PushStyleColor(ImGuiCol_TitleBgActive,     IM_COL32(20, 20, 20, 255));
        ImGui::PushStyleColor(ImGuiCol_ChildBg,           IM_COL32(8, 8, 8, 240));
        ImGui::PushStyleColor(ImGuiCol_FrameBg,           IM_COL32(18, 18, 18, 200));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered,    IM_COL32(28, 28, 28, 220));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive,     IM_COL32(38, 38, 38, 240));
        ImGui::PushStyleColor(ImGuiCol_Header,            IM_COL32(18, 18, 18, 200));
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered,     IM_COL32(28, 28, 28, 220));
        ImGui::PushStyleColor(ImGuiCol_HeaderActive,      IM_COL32(38, 38, 38, 240));
        ImGui::PushStyleColor(ImGuiCol_Button,            IM_COL32(18, 18, 18, 200));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered,     IM_COL32(28, 28, 28, 220));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,      IM_COL32(38, 38, 38, 240));
        ImGui::PushStyleColor(ImGuiCol_Text,              IM_COL32(220, 220, 220, 255));
        ImGui::PushStyleColor(ImGuiCol_TextDisabled,      IM_COL32(100, 100, 100, 255));

        ImGui::Begin("Select Image File", open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

        // Header with current path
        const std::string abs_path = std::filesystem::exists(current_path) ? 
            std::filesystem::absolute(current_path).string() : "";
        
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(180, 180, 190, 255));
        ImGui::TextWrapped("%s", abs_path.c_str());
        ImGui::PopStyleColor();
        
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // Calculate layout
        float window_height = ImGui::GetWindowHeight();
        float button_height = ImGui::GetFrameHeightWithSpacing() + 20.0f;
        float child_height = window_height - ImGui::GetCursorPosY() - button_height - 34.f;

        // Left panel - Parent directories
        ImGui::BeginChild("##navigation", ImVec2(220, child_height), true, ImGuiWindowFlags_NoScrollbar);
        ImGui::Text("Navigation");
        ImGui::Spacing();
        
        parent_path = current_path.parent_path();
        
        // Back button
        ImGui::PushStyleColor(ImGuiCol_Button,            IM_COL32(0, 0, 0, 255));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered,     IM_COL32(70, 70, 75, 220));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,      IM_COL32(95, 95, 105, 240));
        if(current_path != current_path.root_path()) {
            if (ImGui::Button("Parent Directory", ImVec2(-1, 0))) {
                current_path = parent_path;
                current_file.clear();
            }
        }
        ImGui::PopStyleColor(3);
        
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // Directory listing
        if (ImGui::BeginListBox("##dirs", ImVec2(-1, -1))) {
            for (auto &entry : std::filesystem::directory_iterator(parent_path)) {
                if (!entry.is_directory()) continue;
                
                std::string name = entry.path().filename().string();
                bool is_current = (entry.path() == current_path);
                
                ImGui::PushStyleColor(ImGuiCol_Text, is_current ? 
                    IM_COL32(0, 200, 200, 255) : IM_COL32(200, 200, 205, 255));
                
                if (ImGui::Selectable((name).c_str(), is_current)) {
                    current_path = entry.path();
                    current_file.clear();
                }
                
                ImGui::PopStyleColor();
            }
            ImGui::EndListBox();
        }
        ImGui::EndChild();

        ImGui::SameLine();

        // Right panel - Files in current directory
        ImGui::BeginChild("##files", ImVec2(-1, child_height), true);
        ImGui::Text("Files");
        ImGui::Spacing();

        if (ImGui::BeginListBox("##filelist", ImVec2(-1, -1))) {
            // Show subdirectories first
            for (auto &entry : std::filesystem::directory_iterator(current_path)) {
                if (!entry.is_directory()) continue;
                
                std::string name = entry.path().filename().string();
                if (ImGui::Selectable((name).c_str(), false, ImGuiSelectableFlags_AllowDoubleClick)) {
                    if (ImGui::IsMouseDoubleClicked(0)) {
                        current_path = entry.path();
                        current_file.clear();
                    }
                }
            }

            for (auto &entry : std::filesystem::directory_iterator(current_path)) {
                if (entry.is_directory()) continue;

                std::string name = entry.path().filename().string();
                bool is_selected = (!current_file.empty() &&
                    std::filesystem::path(current_file) == entry.path());

                ImGui::PushStyleColor(ImGuiCol_Header, is_selected ?
                    IM_COL32(65, 105, 165, 200) : IM_COL32(45, 45, 55, 150));

                if (ImGui::Selectable((name).c_str(), is_selected)) {
                    current_file = entry.path().string();
                }

                ImGui::PopStyleColor();
            }
            ImGui::EndListBox();
        }
        ImGui::EndChild();

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // Selected file display
        if (!current_file.empty()) {
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(120, 200, 120, 255));
            ImGui::Text("Selected: %s", std::filesystem::path(current_file).filename().string().c_str());
            ImGui::PopStyleColor();
        } else {
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(160, 160, 165, 255));
            ImGui::Text("No file selected");
            ImGui::PopStyleColor();
        }

        ImGui::Spacing();

        // Bottom buttons
        float button_width = 120.0f;
        float available_width = ImGui::GetContentRegionAvail().x;
        float button_spacing = available_width - (button_width * 2);
        
        if (ImGui::Button("Cancel", ImVec2(button_width, 0))) {
            *open = false;
        }
        
        ImGui::SameLine(0, button_spacing);
        
        ImGui::PushStyleColor(ImGuiCol_Button, !current_file.empty() ? 
            IM_COL32(65, 130, 65, 200) : IM_COL32(80, 80, 90, 150));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, !current_file.empty() ? 
            IM_COL32(85, 150, 85, 220) : IM_COL32(100, 100, 110, 180));
        
        bool can_select = !current_file.empty();
        if (!can_select) ImGui::BeginDisabled();
        
        if (ImGui::Button("Select", ImVec2(button_width, 0))) {
            if (!current_file.empty()) {
                selected_file_str = current_file;
                selected_file = selected_file_str.c_str();
                *open = false;
            }
        }
        
        if (!can_select) ImGui::EndDisabled();
        ImGui::PopStyleColor(2);

        if (consolas) ImGui::PopFont();
        ImGui::PopStyleColor(16);
        ImGui::PopStyleVar(8);

        ImGui::End();
    }
};

#endif // FILEPICKER_H
