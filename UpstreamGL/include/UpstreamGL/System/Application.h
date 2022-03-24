#pragma once

#include <filesystem>
#include <fstream>
#include <ostream>
#include <string>

#include <UpstreamGL/System/Window.h>

namespace upgl {
class Logger;
class Console;
class Scheduler;

class Application {
public:
  Application(const ::std::string &name, int argc, char **argv);
  virtual ~Application();

  static Application *const GetInstance();

  const ::std::filesystem::path &GetRootPath() const { return m_RootPath; }
  const ::std::string &GetAppName() const { return m_AppName; }
  Window *GetWindow() const { return m_Window; }
  Console *GetConsole() const { return m_Console; }
  Scheduler *GetScheduler() const { return m_Scheduler; }
  float GetLastDeltaTime() const { return m_LastDeltaTime; }
  void Quit() {
    m_Window->Close();
    m_IsRunning = false;
  }

  int MainLoop();
  int Render();

  virtual void OnInitialize();
  virtual void OnGui();
  virtual void OnExit();

private:
  void ParseArgs(int argc, char **argv);
  bool __WindowClosedCallback(const WindowCloseEvent *);
  bool __WindowResizeCallback(const WindowResizeEvent *);

  void __InitImGui();

private:
  ::std::string m_AppName;
  ::std::filesystem::path m_RootPath;
  Window *m_Window;
  Console *m_Console;
  Scheduler *m_Scheduler;
  bool m_IsRunning;
  static Application *__AppInstance;
  float m_LastDeltaTime = 0;
  ::std::fstream m_LogStream;
  char m_LogBuf[10];
};
} // namespace upgl