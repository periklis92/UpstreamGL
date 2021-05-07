#pragma once

#include <string>
#include <filesystem>

class Window;
class Logger;
class Console;
class Scheduler;
struct WindowCloseEvent;
struct WindowResizeEvent;

class Application
{
public:
	Application(const std::string& name, int argc, wchar_t** argv);
	virtual ~Application();

	static Application* const GetInstance();

	const std::filesystem::path& GetRootPath() const { return m_RootPath; }
	const std::string& GetAppName() const { return m_AppName; }
	Window* GetWindow() const { return m_Window; }
	Console* GetConsole() const { return m_Console; }
	Scheduler* GetScheduler() const { return m_Scheduler; }
	float GetLastDeltaTime() const { return m_DeltaTime; }

	int MainLoop();
	int Render();

	virtual void OnInitialize();
	virtual void OnTick(float deltaTime) {};
	virtual void OnGui();
	virtual void OnExit();

private:
	void ParseArgs(int argc, wchar_t** argv);
	bool __WindowClosedCallback(const WindowCloseEvent*);
	bool __WindowResizeCallback(const WindowResizeEvent*);

private:
	std::string m_AppName;
	std::filesystem::path m_RootPath;
	Window* m_Window;
	Console* m_Console;
	Scheduler* m_Scheduler;
	bool m_IsRunning;
	static Application* __AppInstance;
	float m_DeltaTime = 0, m_LastFrameTime = 0;
};
