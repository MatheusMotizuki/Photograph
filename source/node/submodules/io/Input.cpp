#include "node/submodules/io/Input.hpp"

InputNode::InputNode(SDL_Renderer* renderer) 
    : NodeBase("Input Node", PinType::Output, "input_node", false, ImVec4(0.2f, 0.7f, 1.0f, 1.0f))
    , m_renderer(renderer) {
    ImNodes::SetNodeScreenSpacePos(GetId(), ImVec2(60, 50));
}

InputNode::~InputNode() {
    if (m_texture) {
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
    }
    if (m_image_data) {
        stbi_image_free(m_image_data);
        m_image_data = nullptr;
    }
}

bool InputNode::ShouldDisplayText() const { return false; }

void InputNode::NodeContent() {
    ImGui::TextDisabled("Input Preview:");
    setStyle();

    static bool openPicker = false;
    const int MAX_SIZE = 2048; // 2K maximum

    if (openPicker) {
        filePicker.ShowFileDialog(&openPicker);

        if (m_texture) { SDL_DestroyTexture(m_texture); m_texture = nullptr; }
        if (m_image_data) { stbi_image_free(m_image_data); m_image_data = nullptr; }

        if (filePicker.GetSelectedFile()) {
            int original_width = 0, original_height = 0, og_chans = 0;
            unsigned char* original_data = stbi_load(filePicker.selected_file, &original_width, &original_height, &og_chans, 0);

            if (original_data) {
                int final_width = original_width;
                int final_height = original_height;
                unsigned char* final_data = original_data;

                // Check if we need to resize (cap at 2K)
                if (original_width > MAX_SIZE || original_height > MAX_SIZE) {
                    // Calculate new dimensions while maintaining aspect ratio
                    float scale = std::min((float)MAX_SIZE / original_width, (float)MAX_SIZE / original_height);
                    final_width = (int)(original_width * scale);
                    final_height = (int)(original_height * scale);

                    // Allocate memory for resized image
                    int final_size = final_width * final_height * og_chans;
                    m_image_data = (unsigned char*)malloc(final_size);

                    if (m_image_data) {
                        // Resize the image
                        if (stbir_resize_uint8_linear(
                            original_data, original_width, original_height, 0,
                            m_image_data, final_width, final_height, 0,
                            (stbir_pixel_layout)og_chans)) {
                            
                            final_data = m_image_data;
                        } else {
                            // Resize failed, use original
                            free(m_image_data);
                            m_image_data = original_data;
                            final_width = original_width;
                            final_height = original_height;
                            final_data = original_data;
                        }
                    } else {
                        // Malloc failed, use original
                        m_image_data = original_data;
                        final_width = original_width;
                        final_height = original_height;
                        final_data = original_data;
                    }
                    
                    // Free original data if we resized successfully
                    if (final_data != original_data) {
                        stbi_image_free(original_data);
                    }
                } else {
                    // No resize needed
                    m_image_data = original_data;
                }

                // Store final dimensions
                m_tex_w = final_width;
                m_tex_h = final_height;
                
                // Create SDL texture
                SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(
                    (void*)final_data, final_width, final_height, og_chans * 8, og_chans * final_width, 
                    0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
                m_texture = SDL_CreateTextureFromSurface(m_renderer, surface);
                SDL_FreeSurface(surface);
                
                // Update output_image
                output_image.width = final_width;
                output_image.height = final_height;
                output_image.channels = og_chans;
                output_image.pixels.assign(final_data, final_data + final_width * final_height * og_chans);
                ImageLoaded = true;
                NewImageUploaded = true;
            }
        }
    }

    // if we have a texture and image_data
    if (m_texture && m_image_data) {
        float m_width = 200.f, m_height = 200.f;
        float image_w = (float)m_tex_w, image_h = (float)m_tex_h;
        float scale = 1.0f;
        ImVec2 size;

        if (image_w > m_width) {
            scale = m_width / image_w;
            size = ImVec2(m_width, image_h * scale);
        } else if (image_h > m_height) {
            scale = m_height / image_h;
            size = ImVec2(image_w * scale, m_height);
        } else {
            size = ImVec2(image_w, image_h);
        }

        ImGui::BeginChild("##input image", ImVec2(m_width, size.y), false);
        ImGui::Image((ImTextureID)(intptr_t)m_texture, size);
        ImGui::EndChild();
    }

    if (ImGui::Button("Upload image", ImVec2(200, 30))) openPicker = true;
    popStyle();
}

void InputNode::setStyle() {
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(18, 18, 18, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(61, 61, 61, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(31, 31, 31, 255));
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.6f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
    ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(80, 160, 200, 255));
    // ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[15]);
}

void InputNode::popStyle() {
    // ImGui::PopFont();
    ImGui::PopStyleColor(5);
    ImGui::PopStyleVar(2);
}

void InputNode::ProcessInternal() {
    // No need to do anything here since we update output_image in NodeContent
    // when a new image is loaded
}

void InputNode::Description() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12, 12));
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 4.0f);

    std::string popup_name = GetInternalTitle() + "_" + std::to_string(GetId());
    if (ImGui::BeginPopup(popup_name.c_str())) {
        ImGui::TextColored(ImVec4(0.2f, 0.7f, 1.0f, 1.0f), "Input Node");
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Text("Loads an image from your computer");
        ImGui::Text("and provides it as the starting point");
        ImGui::Text("for your node graph.");
        ImGui::Spacing();
        ImGui::Text("Supported formats include PNG, JPG,");
        ImGui::Text("and other common image types.");
        ImGui::Spacing();
        ImGui::TextDisabled("Tip: Use this node to import");
        ImGui::TextDisabled("photos or textures for editing.");
        ImGui::EndPopup();
    }
    ImGui::PopStyleVar(2);
}