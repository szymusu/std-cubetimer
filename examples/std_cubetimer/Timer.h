#ifndef IMGUI_TIMER_H
#define IMGUI_TIMER_H

#include "../../imgui.h"

class CubeTimer {
private:
    float time = 0;
    bool running = false;


    void startTimer() {
        time = 0;
        running = true;
    }

    void stopTimer() {
        running = false;
    }

    bool spacePressed() {
        float downDuration = ImGui::GetIO().KeysData[ImGuiKey::ImGuiKey_Space].DownDuration;
        return (downDuration < ImGui::GetIO().DeltaTime) && downDuration >= 0;
    }

public:
    ImFont* timeFont;

    void renderFrame() {
        ImGui::Begin("std::cubetimer");

        if (running) {
            time += ImGui::GetIO().DeltaTime;
        }

        ImGui::PushFont(timeFont);
        if (spacePressed()) {
            if (running)
                stopTimer();
            else
                startTimer();
        }
        ImGui::Text("%.2f", time);
        ImGui::PopFont();

        ImGui::End();
    }
};

#endif