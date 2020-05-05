#include "imgui_theme.hpp"

#include <imgui.h>


#ifdef IMGUI_LAYER
    void themeGrey() {

        ImGui::GetStyle().FrameRounding = 5;
        ImGui::GetStyle().ChildRounding = 5;
        ImGui::GetStyle().PopupRounding = 5;

        ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(1.f, 1.f, 1.f, 1.f);
        ImGui::GetStyle().Colors[ImGuiCol_TextDisabled] = ImVec4(0.5f, 0.5f, 0.5f, 1.f);
        ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImVec4(0.25f, 0.25f, 0.25f, 0.94f);
        ImGui::GetStyle().Colors[ImGuiCol_ChildBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.f);
        ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.f);
        ImGui::GetStyle().Colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.5f, 0.5f);
        ImGui::GetStyle().Colors[ImGuiCol_BorderShadow] = ImVec4(0.22f, 0.22f, 0.22f, 0.f);
        ImGui::GetStyle().Colors[ImGuiCol_FrameBg] = ImVec4(0.35f, 0.35f, 0.35f, 0.54f);
        ImGui::GetStyle().Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.59f, 0.59f, 0.59f, 0.4f);
        ImGui::GetStyle().Colors[ImGuiCol_FrameBgActive] = ImVec4(0.59f, 0.59f, 0.59f, 0.67f);
        ImGui::GetStyle().Colors[ImGuiCol_TitleBg] = ImVec4(0.22f, 0.22f, 0.22f, 1.f);
        ImGui::GetStyle().Colors[ImGuiCol_TitleBgActive] = ImVec4(0.16f, 0.16f, 0.16f, 1.f);
        ImGui::GetStyle().Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.14f, 0.14f, 0.14f, 0.51f);
        ImGui::GetStyle().Colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.f);
        ImGui::GetStyle().Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.14f, 0.14f, 0.14f, 0.53f);
        ImGui::GetStyle().Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.f);
        ImGui::GetStyle().Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.f);
        ImGui::GetStyle().Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.f);
        ImGui::GetStyle().Colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.f);
        ImGui::GetStyle().Colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.f);
        ImGui::GetStyle().Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.f);
        ImGui::GetStyle().Colors[ImGuiCol_Button] = ImVec4(0.14f, 0.14f, 0.14f, 0.4f);
        ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered] = ImVec4(0.6f, 0.6f, 0.6f, 0.47f);
        ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] = ImVec4(0.44f, 0.44f, 0.44f, 1.f);
        ImGui::GetStyle().Colors[ImGuiCol_Header] = ImVec4(0.14f, 0.14f, 0.14f, 0.31f);
        ImGui::GetStyle().Colors[ImGuiCol_HeaderHovered] = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
        ImGui::GetStyle().Colors[ImGuiCol_HeaderActive] = ImVec4(0.44f, 0.44f, 0.44f, 1.f);
        ImGui::GetStyle().Colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.5f, 0.5f);
        ImGui::GetStyle().Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.61f, 0.6f, 0.6f, 0.78f);
        ImGui::GetStyle().Colors[ImGuiCol_SeparatorActive] = ImVec4(0.52f, 0.52f, 0.52f, 1.f);
        ImGui::GetStyle().Colors[ImGuiCol_ResizeGrip] = ImVec4(0.35f, 0.35f, 0.35f, 1.f);
        ImGui::GetStyle().Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.f);
        ImGui::GetStyle().Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.f);
        ImGui::GetStyle().Colors[ImGuiCol_Tab] = ImVec4(0.14f, 0.14f, 0.14f, 0.86f);
        ImGui::GetStyle().Colors[ImGuiCol_TabHovered] = ImVec4(0.31f, 0.31f, 0.31f, 0.8f);
        ImGui::GetStyle().Colors[ImGuiCol_TabActive] = ImVec4(0.31f, 0.31f, 0.31f, 1.f);
        ImGui::GetStyle().Colors[ImGuiCol_TabUnfocused] = ImVec4(0.17f, 0.17f, 0.17f, 0.97f);
        ImGui::GetStyle().Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.18f, 0.18f, 0.18f, 1.f);
        ImGui::GetStyle().Colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.f);
        ImGui::GetStyle().Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.f, 0.43f, 0.35f, 1.f);
        ImGui::GetStyle().Colors[ImGuiCol_PlotHistogram] = ImVec4(0.9f, 0.7f, 0.f, 1.f);
        ImGui::GetStyle().Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.f, 0.6f, 0.f, 1.f);
        ImGui::GetStyle().Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.75f, 0.73f, 0.73f, 0.35f);
        ImGui::GetStyle().Colors[ImGuiCol_DragDropTarget] = ImVec4(1.f, 1.f, 0.f, 0.9f);
        ImGui::GetStyle().Colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.f);
        ImGui::GetStyle().Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.f, 1.f, 1.f, 0.7f);
        ImGui::GetStyle().Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.8f, 0.8f, 0.8f, 0.2f);
        ImGui::GetStyle().Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.8f, 0.8f, 0.8f, 0.35f);
    }
#endif
