#include "imgui_theme.hpp"

#include <imgui.h>


void themeGrey() {

    ImGuiStyle& style = ImGui::GetStyle();

    style.FrameRounding = 5;
    style.ChildRounding = 5;
    style.PopupRounding = 5;

    ImVec4* colours = ImGui::GetStyle().Colors;

    colours[ImGuiCol_Text] = ImVec4(1.f, 1.f, 1.f, 1.f);
    colours[ImGuiCol_TextDisabled] = ImVec4(0.5f, 0.5f, 0.5f, 1.f);
    colours[ImGuiCol_WindowBg] = ImVec4(0.25f, 0.25f, 0.25f, 0.94f);
    colours[ImGuiCol_ChildBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.f);
    colours[ImGuiCol_WindowBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.f);
    colours[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.5f, 0.5f);
    colours[ImGuiCol_BorderShadow] = ImVec4(0.22f, 0.22f, 0.22f, 0.f);
    colours[ImGuiCol_FrameBg] = ImVec4(0.35f, 0.35f, 0.35f, 0.54f);
    colours[ImGuiCol_FrameBgHovered] = ImVec4(0.59f, 0.59f, 0.59f, 0.4f);
    colours[ImGuiCol_FrameBgActive] = ImVec4(0.59f, 0.59f, 0.59f, 0.67f);
    colours[ImGuiCol_TitleBg] = ImVec4(0.22f, 0.22f, 0.22f, 1.f);
    colours[ImGuiCol_TitleBgActive] = ImVec4(0.16f, 0.16f, 0.16f, 1.f);
    colours[ImGuiCol_TitleBgCollapsed] = ImVec4(0.14f, 0.14f, 0.14f, 0.51f);
    colours[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.f);
    colours[ImGuiCol_ScrollbarBg] = ImVec4(0.14f, 0.14f, 0.14f, 0.53f);
    colours[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.f);
    colours[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.f);
    colours[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.f);
    colours[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.f);
    colours[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.f);
    colours[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.f);
    colours[ImGuiCol_Button] = ImVec4(0.14f, 0.14f, 0.14f, 0.4f);
    colours[ImGuiCol_ButtonHovered] = ImVec4(0.6f, 0.6f, 0.6f, 0.47f);
    colours[ImGuiCol_ButtonActive] = ImVec4(0.44f, 0.44f, 0.44f, 1.f);
    colours[ImGuiCol_Header] = ImVec4(0.14f, 0.14f, 0.14f, 0.31f);
    colours[ImGuiCol_HeaderHovered] = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
    colours[ImGuiCol_HeaderActive] = ImVec4(0.44f, 0.44f, 0.44f, 1.f);
    colours[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.5f, 0.5f);
    colours[ImGuiCol_SeparatorHovered] = ImVec4(0.61f, 0.6f, 0.6f, 0.78f);
    colours[ImGuiCol_SeparatorActive] = ImVec4(0.52f, 0.52f, 0.52f, 1.f);
    colours[ImGuiCol_ResizeGrip] = ImVec4(0.35f, 0.35f, 0.35f, 1.f);
    colours[ImGuiCol_ResizeGripHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.f);
    colours[ImGuiCol_ResizeGripActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.f);
    colours[ImGuiCol_Tab] = ImVec4(0.14f, 0.14f, 0.14f, 0.86f);
    colours[ImGuiCol_TabHovered] = ImVec4(0.31f, 0.31f, 0.31f, 0.8f);
    colours[ImGuiCol_TabActive] = ImVec4(0.31f, 0.31f, 0.31f, 1.f);
    colours[ImGuiCol_TabUnfocused] = ImVec4(0.17f, 0.17f, 0.17f, 0.97f);
    colours[ImGuiCol_TabUnfocusedActive] = ImVec4(0.18f, 0.18f, 0.18f, 1.f);
    colours[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.f);
    colours[ImGuiCol_PlotLinesHovered] = ImVec4(1.f, 0.43f, 0.35f, 1.f);
    colours[ImGuiCol_PlotHistogram] = ImVec4(0.9f, 0.7f, 0.f, 1.f);
    colours[ImGuiCol_PlotHistogramHovered] = ImVec4(1.f, 0.6f, 0.f, 1.f);
    colours[ImGuiCol_TextSelectedBg] = ImVec4(0.75f, 0.73f, 0.73f, 0.35f);
    colours[ImGuiCol_DragDropTarget] = ImVec4(1.f, 1.f, 0.f, 0.9f);
    colours[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.f);
    colours[ImGuiCol_NavWindowingHighlight] = ImVec4(1.f, 1.f, 1.f, 0.7f);
    colours[ImGuiCol_NavWindowingDimBg] = ImVec4(0.8f, 0.8f, 0.8f, 0.2f);
    colours[ImGuiCol_ModalWindowDimBg] = ImVec4(0.8f, 0.8f, 0.8f, 0.35f);
}
