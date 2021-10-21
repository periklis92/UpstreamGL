#pragma once

#include <list>
#include <cereal/cereal.hpp>
#include <cereal/archives/xml.hpp>

#include <UpstreamGL/System/EventDispatcher.h>

struct GLFWwindow;


struct WindowResolution
{
	int Width;
	int Height;
	int RefreshRate;

	template<class Archive>
	void save(Archive& ar) const
	{
		ar(	CEREAL_NVP(Width),
			CEREAL_NVP(Height), CEREAL_NVP(RefreshRate));
	}
};

struct WindowSettings
{
	WindowResolution Resolution = {800, 600, 60};
	bool Fullscreen = false, Resizable = true, VSync = false;
	char RedBits = 8, GreenBits = 8, BlueBits = 8;
	char AplhaBits = 8, DepthBits = 24, StencilBits = 8;

	template<class Archive>
	void save(Archive& ar) const
	{
		ar(	CEREAL_NVP(Resolution),
			CEREAL_NVP(Fullscreen), CEREAL_NVP(Resizable),
			CEREAL_NVP(RedBits), CEREAL_NVP(GreenBits), 
			CEREAL_NVP(BlueBits), CEREAL_NVP(AplhaBits), 
			CEREAL_NVP(DepthBits), CEREAL_NVP(StencilBits));
	}
};

class Window
{
public:
	Window(const char* title, WindowSettings settings = {});
	~Window();

	void Update();
	bool ShouldClose();
	void MakeCurrent();

	GLFWwindow* GetNativeHandle() const { return m_GLFWHandle; }
	const WindowResolution* GetSupportedResolutions(int& numResolutions);

	EventDispatcher<WindowCloseEvent>& OnWindowClose() { return m_OnWindowClose; }
	EventDispatcher<WindowResizeEvent>& OnWindowResize() { return m_OnWindowResize; }

	inline bool IsFullscreen() { return m_Settings.Fullscreen; }
	void GetSize(int& width, int& height) { width = m_Settings.Resolution.Width; height = m_Settings.Resolution.Height; }
	void GetMousePosition(float& x, float& y) { x = m_LastMouseX; y = m_LastMouseY; }
	void SetFullscreen(bool fullscreen = true);
	void SetResolution(const WindowResolution& resolution);
	void SetVSync(bool vsync = true);
	void Close();

	template<class Archive>
	void save(Archive& ar) const
	{
		ar(CEREAL_NVP(m_Settings));
	}
private:
	static void __KeyCallback(GLFWwindow*, int, int, int, int);
	static void __MouseButtonCallback(GLFWwindow*, int, int, int);
	static void __MousePosCallback(GLFWwindow*, double, double);
	static void __WindowCloseCallback(GLFWwindow*);
	static void __WindowSizeCallback(GLFWwindow*, int, int);
private:
	GLFWwindow* m_GLFWHandle;
	WindowSettings m_Settings;
	float m_LastMouseX{0.f}, m_LastMouseY{0.f};
	EventDispatcher<WindowCloseEvent> m_OnWindowClose;
	EventDispatcher<WindowResizeEvent> m_OnWindowResize;
	WindowResolution* m_SupportedResolutions; int m_NumSupportedResolutions;
};