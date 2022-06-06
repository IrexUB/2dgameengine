#pragma once

#include <SDL2/SDL.h>
#include <imgui/imgui_sdl.h>
#include <glm/glm.hpp>
#include <imgui/imgui.h>

#include "../ECS/ECS.h"

class RenderGUISystem : public System
{
public:
    RenderGUISystem() = default;

    void Update(std::unique_ptr<Registry>& registry)
    {
        ImGui::NewFrame();

        if (ImGui::Begin("Spawn new enemy")) {
            
            const char* sprites[] = {"truck-right", "tank-right"};
            static int selectedSpriteIndex;

            if (ImGui::CollapsingHeader("Sprite Component", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::Combo("Texture id", &selectedSpriteIndex, sprites, IM_ARRAYSIZE(sprites));
            }

            static glm::vec2 newEnemyPosition(0.0, 0.0);
            static int newEnemyScaleX;
            static int newEnemyScaleY;
            static float newEnemyRotation = 0;

            if (ImGui::CollapsingHeader("Transform Component", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::InputFloat("Position x", &newEnemyPosition.x, 1);
                ImGui::InputFloat("Position y", &newEnemyPosition.y, 1);

                ImGui::SliderInt("Scale x", &newEnemyScaleX, 0, 3);
                ImGui::SliderInt("Scale y", &newEnemyScaleY, 0, 3);

                ImGui::SliderFloat("Rotation (deg)", &newEnemyRotation, 0, 360, "%0.1f deg");
            }

            static glm::vec2 newEnemyVelocity(0.0, 0.0);
            if (ImGui::CollapsingHeader("RigidBody2D Component", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::InputFloat("Velocity x", &newEnemyVelocity.x, 1);
                ImGui::InputFloat("Velocity y", &newEnemyVelocity.y, 1);
            }

            static glm::vec2 newProjectileEmitterVelocity(0.0, 0.0);
            static float projectileCooldown = 1.0; // Unit: seconds
            if (ImGui::CollapsingHeader("Bullet Emitter Component", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::InputFloat("Projectile velocity x", &newProjectileEmitterVelocity.x, 1);
                ImGui::InputFloat("Projectile velocity y", &newProjectileEmitterVelocity.y, 1);
                ImGui::InputFloat("Duration (sec)", &projectileCooldown, 1);
            }

            if (ImGui::Button("Create new enemy")) {
                auto newEnemy = registry->CreateEntity();
                newEnemy.AddComponent<TransformComponent>(newEnemyPosition, glm::vec2(newEnemyScaleX, newEnemyScaleY), newEnemyRotation);
                newEnemy.AddComponent<RigidBody2D>(newEnemyVelocity);
                newEnemy.AddComponent<SpriteComponent>(sprites[selectedSpriteIndex], SDL_Rect(0, 0, 32, 32), 1);
                newEnemy.AddComponent<BoxColliderComponent>(64, 64);
                newEnemy.AddComponent<BulletEmitterComponent>(newProjectileEmitterVelocity, projectileCooldown);
            }
        }
        ImGui::End();

        ImGui::Render();
        ImGuiSDL::Render(ImGui::GetDrawData());
    }
};