#include "imgui_external.hpp"


void ImGui::HorizontalSeparator(float cursor_pad)
{
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 p = ImGui::GetCursorScreenPos();;
    draw_list->AddLine(ImVec2(p.x - 9999, p.y), ImVec2(p.x + 9999, p.y), ImGui::GetColorU32(ImGuiCol_Border));
    ImGui::SetCursorScreenPos(ImVec2(p.x, p.y + cursor_pad));
}
