#include "ParticleSystem.h"
#include <cstdlib>
#include <cmath>

float RandomFloat(float min, float max) {
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / (max - min));
}

void ParticleSystem::Init(int count) {
    particles.reserve(count);
    for (int i = 0; i < count; ++i) {
        particles.push_back({
            ImVec2(RandomFloat(0, 2000), RandomFloat(0, 1500)),
            ImVec2(RandomFloat(-8, 8), RandomFloat(-8, 8)),
            RandomFloat(1.0f, 2.2f),
            RandomFloat(0.1f, 0.4f),
            RandomFloat(0, 100)
        });
    }
}

void ParticleSystem::UpdateAndDraw(ImDrawList* drawList, ImVec2 boundsMin, ImVec2 boundsMax, float dt, const float* accentColor) {
    ImColor baseColor = ImColor(accentColor[0], accentColor[1], accentColor[2], 1.0f);
    
    drawList->AddRectFilled(boundsMin, boundsMax, IM_COL32(5, 5, 7, 255));

    for (auto& p : particles) {
        p.pos.x += p.velocity.x * dt;
        p.pos.y += p.velocity.y * dt;
        
        if (p.pos.x < boundsMin.x) p.pos.x = boundsMax.x;
        if (p.pos.x > boundsMax.x) p.pos.x = boundsMin.x;
        if (p.pos.y < boundsMin.y) p.pos.y = boundsMax.y;
        if (p.pos.y > boundsMax.y) p.pos.y = boundsMin.y;
    }

    float maxDist = 130.0f;
    for (size_t i = 0; i < particles.size(); ++i) {
        for (size_t j = i + 1; j < particles.size(); ++j) {
            float dx = particles[i].pos.x - particles[j].pos.x;
            float dy = particles[i].pos.y - particles[j].pos.y;
            float distSq = dx * dx + dy * dy;
            
            if (distSq < maxDist * maxDist) {
                float dist = sqrt(distSq);
                float lineAlpha = (1.0f - (dist / maxDist)) * 0.4f;
                ImColor lineColor = baseColor;
                lineColor.Value.w = lineAlpha * particles[i].alpha;
                drawList->AddLine(particles[i].pos, particles[j].pos, lineColor, 1.0f);
            }
        }
    }

    for (const auto& p : particles) {
        ImColor pColor = baseColor;
        pColor.Value.w = p.alpha * 0.8f;
        
        drawList->AddCircleFilled(p.pos, p.size * 2.5f, ImColor(accentColor[0], accentColor[1], accentColor[2], p.alpha * 0.15f));
        drawList->AddCircleFilled(p.pos, p.size, pColor);
    }
}
