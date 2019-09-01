#pragma once

struct SDL_Window;

class SdlApp
{
public:
  SdlApp();
  ~SdlApp();

  bool Init(int WindowWidth = 800, int WindowHeight = 600);
  void Run();
  void Shutdown();

private:
  SDL_Window* window;
  void* glContext;
  int WindowWidth;
  int WindowHeight;
};
