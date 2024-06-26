#pragma once
#include "../ecs/ECS.h"
#include <imgui.h>
#include <../imgui/backends/imgui_impl_sdl2.h>
#include <../imgui/backends/imgui_impl_sdlrenderer2.h>
#include "../components/TransformComponent.h"
#include "../components/RigidBodyComponent.h"
#include "../components/SpriteComponent.h"
#include "../components/AnimationComponent.h"
#include "../components/BoxColliderComponent.h"
#include "../components/HealthComponent.h"
#include "../components/ProjectileEmitterComponent.h"
class RenderGUISystem : public System
{
public:
    RenderGUISystem() = default;
    void Update(SDL_Renderer* renderer, std::unique_ptr<EntityManager>& registry)
    {
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
        if (ImGui::Begin("Spawn Enemies"))
        {
            static float enemyXPos = 0.0f;
            static float enemyYPos = 0.0f;
            static float scaleX = 1.0f;
            static float scaleY = 1.0f;
            static float VelocityX = 0.0f;
            static float VelocityY = 0.0f;
            static float rotation = 0.0f;
            static float projectileAngle = 0.0f;
            static float projectileSpeed = 0.0f;
            static int projectileRepeat = 1;
            static float projectileDuration = 1.0f;
            static int damage = 1;
            static int health = 100;
            const char* sprites[] = { "soldier2-image" };
            static int selectedSpriteIndex = 0;

            // Input enemy sprites
            if (ImGui::CollapsingHeader("Sprite", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::Combo("Texture ID", &selectedSpriteIndex, sprites, IM_ARRAYSIZE(sprites));
            }
            ImGui::Spacing();

            // Input Enemy Transform component
            if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::InputFloat("Enemy X Position", &enemyXPos);
                ImGui::InputFloat("Enemy Y Position", &enemyYPos);
                ImGui::SliderFloat("Enemy X Scale", &scaleX, 1, 10);
                ImGui::SliderFloat("Enemy Y Scale", &scaleY, 1, 10);
                ImGui::SliderAngle("Rotation (degrees)", &rotation, 0, 360);
            }
            ImGui::Spacing();

            // Input Enemy Rigidbody component
            if (ImGui::CollapsingHeader("Rigidbody", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::InputFloat("Velocity X", &VelocityX);
                ImGui::InputFloat("Velocity Y", &VelocityY);
            }
            ImGui::Spacing();

            // Input Enemy Projectile Emitter component
            if (ImGui::CollapsingHeader("Projectile Emitter", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::SliderAngle("Angle (degrees)", &projectileAngle, 0, 360);
                ImGui::SliderFloat("Speed (px/sec)", &projectileSpeed, 1, 1000);
                ImGui::InputInt("Repeat (sec)", &projectileRepeat);
                ImGui::InputFloat("Duration (sec)", &projectileDuration);
                ImGui::InputInt("Damage", &damage);
            }
            ImGui::Spacing();

            // Input Enemy Health component
            if (ImGui::CollapsingHeader("Health", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::SliderInt("Health", &health, 1, 100);
            }
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();


            if (ImGui::Button("Create new enemy"))
            {
                // Create some entities
                Entity enemy = registry->CreateEntity();
                // Add components to entity
                enemy.AddComponent<TransformComponent>(glm::vec2(enemyXPos, enemyYPos), glm::vec2(scaleX, scaleY), glm::degrees(rotation));
                enemy.AddComponent<RigidBodyComponent>(glm::vec2(VelocityX, VelocityY));
                enemy.AddComponent<SpriteComponent>(sprites[selectedSpriteIndex], 2, false, 64, 64);
                enemy.AddComponent<AnimationComponent>(8, 10, true);
                enemy.AddComponent<BoxColliderComponent>(32, 32, glm::vec2(16, 16));
                // Convert from angle-speed to x-value
                double projectileVelX = cos(projectileAngle) * projectileSpeed;
                // Convert from angle-speed to y-value
                double projectileVelY = sin(projectileAngle) * projectileSpeed;
                enemy.AddComponent<ProjectileEmitterComponent>(glm::vec2(projectileVelX, projectileVelY), 
                    projectileRepeat * 1000, 
                    projectileDuration * 1000, 
                    damage, 
                    false);
                enemy.AddComponent<HealthComponent>(health);
                enemy.Group("enemies");

                // Reset all input values
                enemyXPos = enemyYPos = VelocityX = VelocityY = rotation = projectileAngle = projectileSpeed = 0.0f;
                scaleX = scaleY = 1.0f;
                selectedSpriteIndex = 0;
                projectileRepeat = damage = 1;
                health = 100;
                projectileDuration = 1.0f;
            }
        }
        ImGui::End();
        ImGui::Render();
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
    }
};