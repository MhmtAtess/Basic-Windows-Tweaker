#pragma once
#include "../components/Controls.h"
#include "../core/Logger.h"

class LogsPage {
public:
    static void Render() {
        ImGui::Text("Operation Logs & System Events");
        ImGui::Separator();
        ImGui::Spacing();

        if (UI::ButtonAnimated("Clear Operation Logs", ImVec2(180, 32))) {
            Logger::Get().Clear();
            Logger::Get().AddLog("Logs cleared by user.", LOG_INFO);
        }

        ImGui::Spacing();
        ImGui::BeginChild("LogConsole", ImVec2(0, 400), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);

        auto logs = Logger::Get().GetLogs();
        if (logs.empty()) {
            ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "No system operation logs available.");
        } else {
            for (const auto& log : logs) {
                ImVec4 color = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
                const char* prefix = "[INFO]";
                if (log.type == LOG_SUCCESS) {
                    color = ImVec4(0.2f, 0.9f, 0.4f, 1.0f);
                    prefix = "[SUCCESS]";
                } else if (log.type == LOG_WARNING) {
                    color = ImVec4(1.0f, 0.8f, 0.2f, 1.0f);
                    prefix = "[WARNING]";
                } else if (log.type == LOG_ERROR) {
                    color = ImVec4(0.9f, 0.3f, 0.3f, 1.0f);
                    prefix = "[ERROR]";
                }

                ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "[%s]", log.timestamp.c_str());
                ImGui::SameLine();
                ImGui::TextColored(color, "%s %s", prefix, log.message.c_str());
            }
            if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                ImGui::SetScrollHereY(1.0f);
        }

        ImGui::EndChild();
    }
};
