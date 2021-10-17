#include "testMenu.h"
#include "CameraController.h"
#include "imgui.h"

TestMenu::TestMenu()
{
    m_Resolutions = Application::GetInstance()->GetWindow()->GetSupportedResolutions(m_NumResolutions);
    m_ResolutionLabels = new std::string[m_NumResolutions];
    for (int i = 0; i < m_NumResolutions; ++i)
    {
        char buf[64];
        sprintf(buf, "%d x %d @ %dHz", m_Resolutions[i].Width, m_Resolutions[i].Height, m_Resolutions[i].RefreshRate);
        m_ResolutionLabels[i] = std::string(buf);
    }
}

TestMenu::~TestMenu()
{
    delete[] m_ResolutionLabels;
}

void TestMenu::Draw()
{    
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::MenuItem("Toggle Positions"))
        {
            m_ShowPositions = !m_ShowPositions;
        }
        if (ImGui::MenuItem("Toggle Settings"))
        {
            m_ShowResolutions = !m_ShowResolutions;
        }
        if (ImGui::MenuItem("Quit"))
        {
            Application::GetInstance()->Quit();
        }
        ImGui::EndMainMenuBar();
    }

    if (m_ShowPositions)
    {
        auto& cameraNode = Director::GetInstance()->GetScene()->GetNode("camera");
        auto camController = cameraNode.GetComponent<CameraController>();
        auto transform = cameraNode.GetTransform();
        auto pos = transform->GetWorldPosition();
        ImGui::Begin("Camera Info", &m_ShowPositions);
        ImGui::Text("Position:");
        ImGui::Text("Pos X: %.02f", pos.x);
        ImGui::Text("Pos Y: %.02f", pos.y);
        ImGui::Text("Pos Z: %.02f", pos.z);
        ImGui::Separator();
        auto rot = glm::degrees(transform->GetWorldRotation());
        ImGui::Text("Rotation:");
        ImGui::Text("Euler X: %.02f", rot.x);
        ImGui::Text("Euler Y: %.02f", rot.y);
        ImGui::Text("Euler Z: %.02f", rot.z);
        ImGui::Separator();
        ImGui::Text("Mouse World Pos:");
        ImGui::Text("X: %.04f", camController->MousePOsil.x);
        ImGui::Text("Y: %.04f", camController->MousePOsil.y);
        ImGui::Text("Z: %.04f", camController->MousePOsil.z);
        ImGui::Separator();
        ImGui::Text("Mouse Screen Pos:");
        auto mpos = InputManager::GetInstance()->GetMousePosition();
        ImGui::Text("X: %.04f", mpos.x);
        ImGui::Text("Y: %.04f", mpos.y);
        ImGui::End();
    }

    if (m_ShowResolutions)
    {
        if (ImGui::Begin("Settings", &m_ShowResolutions))
        {
            if (ImGui::Button("Toggle Fullscreen"))
                Application::GetInstance()->GetWindow()->SetFullscreen(!Application::GetInstance()->GetWindow()->IsFullscreen());
            if (ImGui::BeginMenu("Resolutions"))
            {
                for (int i = 0; i < m_NumResolutions; ++i)
                {
                    if (ImGui::MenuItem(m_ResolutionLabels[i].c_str()))
                    {
                        Application::GetInstance()->GetWindow()->SetResolution(m_Resolutions[i]);
                    }
                }
                ImGui::EndMenu();
            }
            if (ImGui::Checkbox("V-Sync", &m_VSyncValue))
                Application::GetInstance()->GetWindow()->SetVSync(m_VSyncValue);
        }
        ImGui::End();
    }
	
}