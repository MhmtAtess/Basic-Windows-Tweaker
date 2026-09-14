#pragma once
#include <vector>
#include <imgui.h>

struct Particle {
    ImVec2 pos;
    ImVec2 velocity;
    float size;
    float alpha;
    float life;
};

class ParticleSystem {
public:
    void Init(int count);
    void UpdateAndDraw(ImDrawList* drawList, ImVec2 boundsMin, ImVec2 boundsMax, float dt, const float* accentColor);

private:
    std::vector<Particle> particles;
    float maxDistance = 150.0f;
};
