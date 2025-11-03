#include "node/submodules/io/Preview.hpp"

PreviewNode::PreviewNode(SDL_Renderer* renderer) 
    : NodeBase("Preview Node", PinType::Both, "preview_node", true, ImVec4(0.4f, 0.6f, 1.0f, 1.0f))
    , m_renderer(renderer) {}

PreviewNode::~PreviewNode() {
    if (m_texture) {
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
    }
    if (m_image_data) {
        stbi_image_free(m_image_data);
        m_image_data = nullptr;
    }
}

unsigned int PreviewNode::GetBorderColor() const {
    return IM_COL32(102, 153, 255, 255);
}

bool PreviewNode::ShouldDisplayText() const { return false; }

void PreviewNode::NodeContent() {
    ImGui::TextDisabled("Output Preview:");
    setStyle();

    if (m_texture) {
        float m_width = 350.0f, m_height = 300.0f;
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
        
        ImGui::BeginChild("##preview_image", ImVec2(m_width, size.y), false);
        ImGui::Image((ImTextureID)(intptr_t)m_texture, size);
        ImGui::EndChild();
    } else {
        // do nothing!!11!
    }

    popStyle();
}

void PreviewNode::setStyle() {
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.6f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
    ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(80, 255, 180, 255));
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[15]);
}

void PreviewNode::popStyle() {
    ImGui::PopFont();
    ImGui::PopStyleColor(2);
    ImGui::PopStyleVar(2);
}

void PreviewNode::ClearPreview() {
    if (m_texture) {
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
    }
    m_tex_w = 0;
    m_tex_h = 0;
    input_image = ImageData(); 
    output_image = ImageData();
}

void PreviewNode::ProcessInternal() {
    if (!input_image.isValid()) {
        ClearPreview();
        return;
    }

    // Only recreate texture if dimensions changed
    if (!m_texture || 
        m_tex_w != input_image.width || 
        m_tex_h != input_image.height) {
        
        if (m_texture) {
            SDL_DestroyTexture(m_texture);
            m_texture = nullptr;
        }
        
        m_tex_w = input_image.width;
        m_tex_h = input_image.height;
        
        Uint32 format = (input_image.channels == 4) ? SDL_PIXELFORMAT_RGBA32 : SDL_PIXELFORMAT_RGB24;
        
        m_texture = SDL_CreateTexture(
            m_renderer,
            format,
            SDL_TEXTUREACCESS_STREAMING,
            m_tex_w,
            m_tex_h
        );
    }

    if (m_texture) {
        SDL_UpdateTexture(
            m_texture,
            nullptr,
            input_image.pixels.data(),
            m_tex_w * input_image.channels
        );
    }
    
    // Pass through the input to output
    output_image = input_image;
}

void PreviewNode::Description() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12, 12));
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 4.0f);

    std::string popup_name = GetInternalTitle() + "_" + std::to_string(GetId());
    if (ImGui::BeginPopup(popup_name.c_str())) {
        ImGui::TextColored(ImVec4(0.4f, 0.6f, 1.0f, 1.0f), "Output Node");
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Text("Displays the final result of your node graph.");
        ImGui::Text("Use this node to preview the processed image.");
        ImGui::Spacing();
        ImGui::TextDisabled("Tip: Connect your last processing node");
        ImGui::TextDisabled("to this node to see the final effect.");
        ImGui::EndPopup();
    }
    ImGui::PopStyleVar(2);
}