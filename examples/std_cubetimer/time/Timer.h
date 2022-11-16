#ifndef IMGUI_TIMER_H
#define IMGUI_TIMER_H

#include <vector>

#include "../../../imgui.h"

class CubeTimer {
private:
    float time = 0;
    bool running = false;
    float rollingAo5 = 0;
    std::vector<float> times;

    void startTimer() {
        time = 0;
        running = true;
    }

    void stopTimer() {
        running = false;
        times.push_back(time);
        if (times.size() >= 5) {
            rollingAo5 = getAo5(times.data() + times.size() - 5);
        }
    }

    bool spacePressed() {
        float downDuration = ImGui::GetIO().KeysData[ImGuiKey::ImGuiKey_Space].DownDuration;
        return downDuration >= 0 && (downDuration < ImGui::GetIO().DeltaTime);
    }

    float getAo5(float times[]) {
        int minIndex = 0, maxIndex = 0;
        for (int i = 1; i < 5; i++) {
            if (times[i] > times[maxIndex]) {
                maxIndex = i;
            }
            else if (times[i] < times[minIndex]) {
                minIndex = i;
            }
        }
        float sumOf3 = 0;
        for (int i = 0; i < 5; ++i) {
            if (i != minIndex && i != maxIndex) {
                sumOf3 += times[i];
            }
        }
        return sumOf3 / 3;
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
        ImGui::Text("ao5: %.2f", rollingAo5);

        ImGui::End();
    }
};

#endif