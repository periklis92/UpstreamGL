#pragma once

#include <list>

#include <UpstreamGL/System/EventDispatcher.h>

struct GLFWwindow;

namespace upgl {
struct WindowResolution {
  int Width;
  int Height;
  int RefreshRate;
};

struct WindowSettings {
  WindowResolution Resolution = {800, 600, 60};
  bool Fullscreen = false, Resizable = true, VSync = false;
  char RedBits = 8, GreenBits = 8, BlueBits = 8;
  char AplhaBits = 8, DepthBits = 24, StencilBits = 8;
};

/**
 * @brief Wrapper class for glfw windows.
 *
 */
class Window {
public:
  Window(const char *title, WindowSettings settings = {});
  ~Window();

  /**
   * @brief Swap buffers and poll window events.
   *
   */
  void Update();

  /**
   * @brief Whether the window should close or not.
   *
   * @return true if the window should close, false otherwise.
   */
  bool ShouldClose();

  /**
   * @brief Make this window the current context.
   *
   */
  void MakeCurrent();

  /**
   * @return GLFWwindow* the native glfw window handle.
   */
  GLFWwindow *GetNativeHandle() const { return m_GLFWHandle; }
  const WindowResolution *GetSupportedResolutions(int &numResolutions);

  EventDispatcher<WindowCloseEvent> &OnWindowClose() { return m_OnWindowClose; }
  EventDispatcher<WindowResizeEvent> &OnWindowResize() { return m_OnWindowResize; }

  inline bool IsFullscreen() { return m_Settings.Fullscreen; }
  void GetSize(int &width, int &height) {
    width = m_Settings.Resolution.Width;
    height = m_Settings.Resolution.Height;
  }
  void GetMousePosition(float &x, float &y) {
    x = m_LastMouseX;
    y = m_LastMouseY;
  }
  void SetFullscreen(bool fullscreen = true);
  void SetResolution(const WindowResolution &resolution);
  void SetVSync(bool vsync = true);
  void Close();

private:
  static void __KeyCallback(GLFWwindow *, int, int, int, int);
  static void __MouseButtonCallback(GLFWwindow *, int, int, int);
  static void __MousePosCallback(GLFWwindow *, double, double);
  static void __WindowCloseCallback(GLFWwindow *);
  static void __WindowSizeCallback(GLFWwindow *, int, int);

private:
  GLFWwindow *m_GLFWHandle;
  WindowSettings m_Settings;
  float m_LastMouseX{0.f}, m_LastMouseY{0.f};
  EventDispatcher<WindowCloseEvent> m_OnWindowClose;
  EventDispatcher<WindowResizeEvent> m_OnWindowResize;
  WindowResolution *m_SupportedResolutions;
  int m_NumSupportedResolutions;
};
} // namespace upgl