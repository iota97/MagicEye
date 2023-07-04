#include "GUI.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

GUI::GUI(GLFWwindow *w, Context *c) : ctx(c) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.IniFilename = NULL;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(w, true);
    ImGui_ImplOpenGL3_Init("#version 430");
}

GUI::~GUI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GUI::RenderMenu() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    
    ImGui::Begin("Settings");
    ImGuiIO& io = ImGui::GetIO();
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::SameLine();
    ImGui::Dummy(ImVec2(10.0f, 0.0f));
    ImGui::SameLine();
    if (ImGui::Button("Quit")) {
        ctx->shouldClose = true;
    }
    const char* scene[] = {"Standard", "Fairy", "Church", "Fight", "Windmill", "Space", "Dance", "Cat"};
    ImGui::Combo("Scene", &ctx->sceneId, scene, IM_ARRAYSIZE(scene));
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    ImGui::Text("Misc settings");
    ImGui::SliderFloat("Movement speed", &ctx->moveSpeed, 1.0f, 100.0f, "%.1f");
    ImGui::SliderFloat("Mouse sensitivity", &ctx->mouseSensitivity, 0.05f, 2.0f, "%.2f");
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    ImGui::Text("Rendering settings");
    ImGui::Checkbox("Fullscreen", &ctx->fullscreen);
    ImGui::SameLine();
    ImGui::Checkbox("VSync", &ctx->vSync);
    ImGui::SameLine();
    ImGui::Checkbox("Toon shading", &ctx->toon);
    ImGui::SameLine();
    ImGui::Checkbox("DQ skinning", &ctx->dqSkinning);

    ImGui::Dummy(ImVec2(0.0f, 1.0f));
    ImGui::SliderFloat("Color resolution", &ctx->colorResolution, 0.01f, 1.0f, "%.2f");
    ImGui::SliderFloat("Animation speed", &ctx->animSpeed, 0.0f, 3.0f, "%.1f");

    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    ImGui::Checkbox("Stereogram rendering", &ctx->stereoRendering);
    ImGui::Dummy(ImVec2(0.0f, 1.0f));
    
    ImGui::Text("Depth settings");
    ImGui::SliderFloat("Depth strength", &ctx->depthStrength, 0.5f, 20.0f, "%.1f");
    ImGui::SliderFloat("Eye separation", &ctx->eyeSep, 0.1f, 2.0f, "%.2f");
    ImGui::SliderFloat("Observer distance", &ctx->obsDistance, 0.1f, 5.0f, "%.2f");
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    ImGui::Text("Pattern settings");
    const char* pattern[] = {"Perlin noise", "Perlin noise (RGB)", "RGB noise", "Colorful", "Cubes", "Symbols", "Wavy"};
    ImGui::Combo("Pattern", &ctx->pattern, pattern, IM_ARRAYSIZE(pattern));
    ImGui::SameLine();
    ImGui::Checkbox("Static", &ctx->staticPattern);
    ImGui::SliderFloat("Scene color", &ctx->sceneColorStr, 0.0f, 1.0f, "%.1f");
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    ImGui::Text("Edge detection settings");
    ImGui::SliderFloat("Edge strength", &ctx->edgeStr, 0.0f, 1.0f, "%.1f");
    ImGui::SliderFloat("Edge threshold", &ctx->edgeThreshold, 0.0f, 10.0f, "%.2f");
    ImGui::Checkbox("Use normals for edge detection", &ctx->edgeNormal);

    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}