#pragma once
#include <UpstreamGL/UpstreamGL.h>

class TestMenu
{
public:
    TestMenu();
    ~TestMenu();
    void Draw();
private:
    bool m_ShowPositions = false, m_ShowResolutions = false, m_VSyncValue = true;   
	int m_NumResolutions = 0;
	const WindowResolution* m_Resolutions = nullptr;
	std::string* m_ResolutionLabels = nullptr;
};