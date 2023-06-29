#include "ImGUI.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

ImGUI::ImGUI(GLFWwindow *w, Context *c) : ctx(c) {
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

ImGUI::~ImGUI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGUI::RenderMenu() {
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
    ImGui::Dummy(ImVec2(0.0f, 10.0f));
    ImGui::Checkbox("VSync", &ctx->vSync);
    ImGui::SameLine();
    ImGui::Checkbox("Stereogram Rendering", &ctx->stereoRendering);

    ImGui::Dummy(ImVec2(0.0f, 10.0f));
    ImGui::SliderFloat("Depth strength", &ctx->depthStrength, 0.5f, 20.0f, "%.1f");
    ImGui::SliderFloat("Eye separation", &ctx->eyeSep, 0.1f, 2.0f, "%.1f");
    ImGui::SliderFloat("Observer distance", &ctx->obsDistance, 0.1f, 5.0f, "%.1f");

    ImGui::Dummy(ImVec2(0.0f, 10.0f));
    ImGui::SliderFloat("Scene color", &ctx->sceneColorStr, 0.0f, 1.0f, "%.1f");
    ImGui::SliderFloat("Edge strength", &ctx->edgeStr, 0.0f, 1.0f, "%.1f");
    ImGui::SliderFloat("Edge threshold", &ctx->edgeThreshold, 0.0f, 2.0f, "%.2f");
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    const char* items[] = {"Perlin noise", "Perlin noise (RGB)", "RGB noise", "Colorful", "Cubes", "Symbols", "Wavy"};
    ImGui::Combo("Pattern", &ctx->pattern, items, IM_ARRAYSIZE(items));
    ImGui::SameLine();
    ImGui::Checkbox("Static", &ctx->staticPattern);

    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}