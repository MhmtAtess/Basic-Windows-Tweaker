#pragma once
#include <map>
#include <string>
#include <imgui.h>

class AnimationManager {
public:
    static float Lerp(float current, float target, float speed, float dt) {
        return current + (target - current) * (1.0f - exp(-speed * dt));
    }
    
    static float EaseOutCubic(float t) {
        float f = t - 1.0f;
        return f * f * f + 1.0f;
    }
    
    static float GetAnimatedState(ImGuiID id, bool targetState, float speed = 12.0f) {
        auto* storage = ImGui::GetStateStorage();
        float current = storage->GetFloat(id, 0.0f);
        float target = targetState ? 1.0f : 0.0f;
        float dt = ImGui::GetIO().DeltaTime;
        float updated = Lerp(current, target, speed, dt);
        if (abs(updated - target) < 0.001f) updated = target;
        storage->SetFloat(id, updated);
        return updated;
    }
};
