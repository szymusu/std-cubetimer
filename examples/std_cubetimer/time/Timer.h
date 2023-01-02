#ifndef IMGUI_TIMER_H
#define IMGUI_TIMER_H

#include <vector>

#include "../../../imgui.h"
#include "TimeList.h"
#include "TimeEntry.h"
#include "../cube/Scramble.h"

class CubeTimer {
private:
    float time = 0;
    bool running = false;
    float rollingAo5 = -1;
    TimeList timeList;

    void startTimer() {
        time = 0;
        running = true;
    }

    void stopTimer() {
        running = false;
        Scramble s;
        timeList.add({ time, s });
        rollingAo5 = timeList.getAverage(5);
        timeList.setLastAo5(rollingAo5);
    }

    bool spacePressed() {
        float downDuration = ImGui::GetIO().KeysData[ImGuiKey::ImGuiKey_Space].DownDuration;
        return downDuration >= 0 && (downDuration < ImGui::GetIO().DeltaTime);
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
        if (rollingAo5 < 0)
            ImGui::Text("ao5: N/A");
        else
            ImGui::Text("ao5: %.2f", rollingAo5);
        timeList.renderTable();



        ImGui::End();
    }
};

#endif