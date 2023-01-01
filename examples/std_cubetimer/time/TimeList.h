#ifndef STD_CUBETIMER_TIMELIST_H
#define STD_CUBETIMER_TIMELIST_H

#include <vector>

#include "TimeEntry.h"
#include "../../../imgui.h"

class TimeList {
private:
    std::vector<TimeEntry> times;

public:
    void add(TimeEntry time) {
        times.push_back(time);
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

    void renderTable() {
        static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;

        // When using ScrollX or ScrollY we need to specify a size for our table container!
        // Otherwise by default the table will fit all available space, like a BeginChild() call.
//        ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * 8);
        if (ImGui::BeginTable("table_scrolly", 3, flags)) {
            ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
            ImGui::TableSetupColumn("Id", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Time", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Scramble", ImGuiTableColumnFlags_None);
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
                    ImGui::Text("%s", times[row].scramble.getNotationChars());
                }
            }
            ImGui::EndTable();
        }
    }
};


#endif //STD_CUBETIMER_TIMELIST_H
