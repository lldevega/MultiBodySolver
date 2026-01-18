#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "misc/cpp/imgui_stdlib.h"

#include <cmath>
#include <vector>
#include <string>

#include "../../include/Body.hpp"
#include "../../include/Universe.hpp"

static const ImU32 color_palette[] = {
  IM_COL32(255, 0, 0, 255),
  IM_COL32(0, 255, 0, 255),
  IM_COL32(0, 0, 255, 255),
  IM_COL32(255, 255, 0, 255),
  IM_COL32(0, 255, 255, 255),
  IM_COL32(255, 0, 255, 255)
};

struct Point2D
{
  double x;
  double y;
};

int main() 
{
  if (!glfwInit())
    return 1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

  GLFWwindow* window = glfwCreateWindow(1280, 720, "n-body problem simulator", NULL, NULL);
  if (!window) 
    return 1;

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);
  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsLight();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 120");

  // create universe
  auto universe = Universe(6.67e-11);

  // points in 2D
  std::vector<Point2D> points;

  // data defining a body
  std::string name = "body";
  double mass = 0.0;
  double x0 = 0.0;
  double y0 = 0.0;
  double vx0 = 0.0;
  double vy0 = 0.0;

  // data for simulation
  bool running = false;
  int it = 0;
  double G = 1.0; 
  double dt = 1.0;

  // plot scale
  double max_dist = 1.0;

  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // input window for body data
    ImGui::Begin("Bodies");

    ImGui::InputText("name", &name);

    ImGui::InputDouble("mass", &mass);

    ImGui::InputDouble("x initial", &x0);
    ImGui::InputDouble("y initial", &y0);

    ImGui::InputDouble("vx initial", &vx0);
    ImGui::InputDouble("vy initial", &vy0);

    if (ImGui::Button("Add body", ImVec2(-1, 0)))
    {
      auto body = Body(name, mass, std::vector<double>{x0, y0, 0.0}, std::vector<double>{vx0, vy0, 0.0});
      universe.AddBody(body);
      points.push_back({body.GetPosition()[0], body.GetPosition()[1]});
      // compute max distance
      double r = std::sqrt(x0 * x0 + y0 * y0);
      max_dist = std::max(max_dist, r);
    }

    if (ImGui::Button("Clear", ImVec2(-1, 0)))
    {
      universe.Reset();
      points.clear();
    }

    ImGui::End();

    // input window for simulation
    ImGui::Begin("Simulation");

    ImGui::InputDouble("G constant", &G);
    ImGui::InputDouble("time step", &dt);

    if (ImGui::Button("Start", ImVec2(-1, 0))) 
    {
      running = true;
      it = 0;
    }

    if (ImGui::Button("Stop", ImVec2(-1, 0))) 
    {
      running = false;
    }

    ImGui::End();

    // run simulation
    if (running)
    {
      universe.AdvanceInTime(dt);
      for (int i = 0; auto &p : points)
      {
        p.x = universe.GetBodies()[i].GetPosition()[0];
        p.y = universe.GetBodies()[i].GetPosition()[1];
        ++i;
      }
    }

    // Plot window
    ImGui::Begin("2D Plot");

    // Get the current window's draw list
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
    ImVec2 canvas_size = ImGui::GetContentRegionAvail();

    // Draw a light background rectangle for the plot area
    draw_list->AddRectFilled(
      canvas_pos,
      ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y),
      IM_COL32(240, 240, 240, 255));

    // Origin point (Center of the window)
    ImVec2 origin = ImVec2(canvas_pos.x + canvas_size.x / 2, canvas_pos.y + canvas_size.y / 2);

    // Draw Axis Lines (Black)
    draw_list->AddLine(
      ImVec2(canvas_pos.x, origin.y), 
      ImVec2(canvas_pos.x + canvas_size.x, origin.y),
      IM_COL32(0, 0, 0, 255),
      2.0f);
    draw_list->AddLine(
      ImVec2(origin.x, canvas_pos.y),
      ImVec2(origin.x, canvas_pos.y + canvas_size.y),
      IM_COL32(0, 0, 0, 255),
      2.0f);

    // Calculate scale so the furthest point stays within the canvas (with 20% padding)
    double scale = (std::min(canvas_size.x, canvas_size.y) / 2.0 / max_dist) * 0.8 /*padding*/;

    // Plot bodies
    for (int i = 0; const auto &p : points)
    {
      // Scale points
      double px = origin.x + p.x * scale;
      double py = origin.y - p.y * scale; // Invert Y because screen space Y grows downwards

      // Only draw if within window bounds
      if (px > canvas_pos.x && px < canvas_pos.x + canvas_size.x && py > canvas_pos.y && py < canvas_pos.y + canvas_size.y)
      {
        draw_list->AddCircleFilled(ImVec2(px, py), 5.0f, color_palette[i % 6]);
      }
      ++i;
    }

    ImGui::End();

    // Rendering
    ImGui::Render();
    glViewport(0, 0, 1280, 720);
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}