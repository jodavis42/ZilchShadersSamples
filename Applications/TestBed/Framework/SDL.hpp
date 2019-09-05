#pragma once

struct SDL_Window;
class Application;
class SdlApp
{
public:
  SdlApp();
  ~SdlApp();

  bool Init(int WindowWidth = 800, int WindowHeight = 600);
  void Run();
  void Shutdown();

  SDL_Window* window;
  Application* mApplication;
  void* glContext;
  int WindowWidth;
  int WindowHeight;
};
