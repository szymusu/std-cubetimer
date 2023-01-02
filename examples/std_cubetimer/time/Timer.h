#ifndef IMGUI_TIMER_H
#define IMGUI_TIMER_H

#include <vector>

#include "../../../imgui.h"
#include "../../../implot.h"
#include "TimeList.h"
#include "TimeEntry.h"
#include "../cube/Scramble.h"

class CubeTimer {
private:
    float time = 0;
    bool running = false;
    float rollingAo5 = -1;
    TimeList timeList;
    Scramble newScramble;
    char scrambleBuffer[75];

    void startTimer() {
        time = 0;
        running = true;
    }

    void stopTimer() {
        running = false;
        timeList.add({ static_cast<float>(timeList.length()), time, newScramble });
        rollingAo5 = timeList.getAverage(5);
        timeList.setLastAo5(rollingAo5);
        newScramble = Scramble();
        generateScrambleNotation(newScramble.moves);
    }

    bool spacePressed() {
        float downDuration = ImGui::GetIO().KeysData[ImGuiKey::ImGuiKey_Space].DownDuration;
        return downDuration >= 0 && (downDuration < ImGui::GetIO().DeltaTime);
    }


public:
    ImFont* timeFont;
    ImFont* secondaryFont;

    CubeTimer() {
        newScramble = Scramble();
        generateScrambleNotation(newScramble.moves);
    }

    void renderFrame() {
        ImGui::Begin("std::cubetimer");

        if (running) {
            time += ImGui::GetIO().DeltaTime;
        }

        ImGui::PushFont(secondaryFont);
        ImGui::Text("%s", scrambleBuffer);
        ImGui::PopFont();

        ImGui::PushFont(timeFont);
        if (spacePressed()) {
            if (running)
                stopTimer();
            else
                startTimer();
        }
        ImGui::Dummy(ImVec2(200, 1));
        ImGui::SameLine();
        ImGui::Text("%.2f", time);
        ImGui::PopFont();

        ImGui::SameLine(ImGui::GetWindowWidth() - 800);
        ImGui::PushFont(secondaryFont);
        timeList.renderTable();
        renderGraph();
        ImGui::End();
//        ImPlot::ShowDemoWindow();
    }

    void renderGraph() {
        ImGui::BeginChild("Performance over time", ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight() - 600));
        if (ImPlot::BeginPlot("Performance over time")) {

            ImPlot::SetupAxes("No.","Time", ImPlotAxisFlags_NoDecorations, 0);
            const int GRAPH_WIDTH = 20;
            const int GRAPH_ADVANCE = 1;
            int x_min = 0;
            int x_max = GRAPH_WIDTH;
            if (timeList.length() > GRAPH_WIDTH - GRAPH_ADVANCE) {
                x_min = timeList.length() - (GRAPH_WIDTH - GRAPH_ADVANCE);
                x_max = timeList.length() + GRAPH_ADVANCE;
            }
            ImPlot::SetupAxesLimits(x_min, x_max,0,timeList.worstTime.time + 1, ImPlotCond_Always);
            ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);
            ImPlot::PlotLine("Solves", &timeList.times[0].id, &timeList.times[0].time, timeList.length(), 0, 0, sizeof(TimeEntry));
            ImPlot::EndPlot();
        }
        ImGui::EndChild();
    }

    void generateScrambleNotation(std::array<Move, 25> moves) {
        for (size_t i = 0; i < moves.size(); i++) {
            scrambleBuffer[3*i] = moves[i].faceChar();
            char suffix = ' ';
            if (moves[i].modifier == PRIME)
                suffix = '\'';
            else if (moves[i].modifier == TWO)
                suffix = '2';
            scrambleBuffer[3*i + 1] = suffix;
            scrambleBuffer[3*i + 2] = ' ';
        }
        scrambleBuffer[74] = '\0';
    }
};

#endif