#ifndef STD_CUBETIMER_TIMELIST_H
#define STD_CUBETIMER_TIMELIST_H

#include <vector>
#include <cstdio>

#include "TimeEntry.h"
#include "../../../imgui.h"

class TimeList {
private:
    char scrambleBuffer[75];

public:
    TimeEntry bestTime = {-1, -1};
    TimeEntry worstTime = {-1, -1};
    float lastAo5 = -1;
    float bestAo5 = -1;
    std::vector<TimeEntry> times;

    void add(TimeEntry time) {
        times.push_back(time);
        if (bestTime.time < 0 || time.time < bestTime.time) bestTime = time;
        if (worstTime.time < 0 || time.time > worstTime.time) worstTime = time;
    }

    size_t length() {
        return times.size();
    }

    float getAverage(size_t numOfTimes, bool includeFastestAndSlowest = false) {
        if (!numOfTimes || numOfTimes > times.size()) return -1;

        TimeEntry fastest = times.at(times.size() - 1);
        TimeEntry slowest = times.at(times.size() - 1);
        float total = 0;
        for (size_t i = 1; i <= numOfTimes; ++i) {
            TimeEntry t = times.at(times.size() - i);
            total += t.time;
            if (t.time > slowest.time) slowest = t;
            if (t.time < fastest.time) fastest = t;
        }

        if (includeFastestAndSlowest) {
            return total / numOfTimes;
        }
        else {
            return (total - fastest.time - slowest.time) / (numOfTimes - 2);
        }
    }

    void setLastAo5(float ao5) {
        lastAo5 = ao5;
        times.at(times.size()-1).rollingAo5 = ao5;
        if (bestAo5 < 0 || ao5 < bestAo5) bestAo5 = ao5;
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

    void renderTable() {
        ImGui::BeginChild("Time history", ImVec2(800, 600));

        if (bestTime.time < 0)
            ImGui::Text("Best time: N/A");
        else
            ImGui::Text("Best time: %.2f", bestTime.time);
        ImGui::SameLine(ImGui::GetWindowWidth() - 300);
        if (lastAo5 < 0)
            ImGui::Text("Current ao5: N/A");
        else
            ImGui::Text("Current ao5: %.2f", lastAo5);

        if (worstTime.time < 0)
            ImGui::Text("Best time: N/A");
        else
            ImGui::Text("Worst time: %.2f", worstTime.time);
        ImGui::SameLine(ImGui::GetWindowWidth() - 300);
        if (bestAo5 < 0)
            ImGui::Text("Best ao5: N/A");
        else
            ImGui::Text("Best ao5: %.2f", bestAo5);
        ImGui::PopFont();

        static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;

        // When using ScrollX or ScrollY we need to specify a size for our table container!
        // Otherwise by default the table will fit all available space, like a BeginChild() call.
//        ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * 8);
        if (ImGui::BeginTable("table_scrolly", 4, flags)) {
            ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
            ImGui::TableSetupColumn("Id", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Time", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Scramble", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Ao5", ImGuiTableColumnFlags_None);
            ImGui::TableHeadersRow();

            // Demonstrate using clipper for large vertical lists
            ImGuiListClipper clipper;
            clipper.Begin(times.size());
            while (clipper.Step()) {
                for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++) {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("%d", row);
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%.2f", times[row].time);
                    ImGui::TableSetColumnIndex(2);

                    generateScrambleNotation(times[row].scramble.moves);
                    ImGui::Text("%s", scrambleBuffer);
                    ImGui::TableSetColumnIndex(3);
                    float ao5 = times[row].rollingAo5;
                    if (ao5 < 0)
                        ImGui::Text("N/A");
                    else
                        ImGui::Text("%.2f", ao5);
                }
            }
            ImGui::EndTable();
        }
        ImGui::EndChild();
    }
};


#endif //STD_CUBETIMER_TIMELIST_H
