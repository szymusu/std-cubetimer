#ifndef IMGUI_TIMER_H
#define IMGUI_TIMER_H

#include "../../imgui.h"

void CubeTimer(ImFont* font) {
    static float timer = 0;

    ImGui::Begin("std::cubetimer");

    timer += ImGui::GetIO().DeltaTime;

    ImGui::PushFont(font);
    ImGui::Text("%.2f", timer);
    ImGui::PopFont();

    ImGui::End();
}

#endif //IMGUI_TIMER_H
