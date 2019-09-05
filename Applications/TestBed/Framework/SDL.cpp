#include "SDL.hpp"

#include "Mouse.hpp"
//#include "SDL_main.h"

#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "ImGuiHelper.hpp"

#include <stdio.h>
#include <time.h>
#include "Application.hpp"

//-----------------------------------------------------------------------------
// Idle function where we update
void Idle(Application* application, clock_t& lastTime)
{
  // Determine the new clock ticks
  clock_t newTime = clock();

  // Get the difference between the new clock tick and the last clock tick
  clock_t ticks = newTime - lastTime;

  // Divide out by tick frequency to get the frame time passed
  float frameTime = ticks / (float)CLOCKS_PER_SEC;

  // Update the last tick count
  lastTime = newTime;

  // Now update the application with the frame time
  application->Update(frameTime);
}

//-----------------------------------------------------------------------------
// Display function where we draw
void Display(Application* application, SDL_Window* window)
{
  // Draw all the AntTweakBar windows / ui
  //TwDraw();

  application->Draw();

  // Finally, present the back buffer to the screen
  SDL_GL_SwapWindow(window);
}

//-----------------------------------------------------------------------------
// Window resize/reshape function
void Reshape(Application* application, int width, int height)
{
  // Tell AntTweakBar that our window has been resized
//  TwWindowSize(width, height);

  // Prevent a divide by zero, when window is too short
  // (you cant make a window of zero width).
  if(height <= 0)
  {
    height = 1;
  }

  // Compute the aspect ratio as w/h
  float aspectRatio = width / (float)height;

  // Let the application handle it's own resizing
  application->Reshape(width, height, aspectRatio);
}

int TranslateSDLKey(SDL_Event& event)
{
  int key = event.key.keysym.sym;
  if('a' <= key && key <= 'z')
    key -= 'a' - 'A';
  return key;
}

void MainLoop(SDL_Window* window, Application* application)
{
  clock_t lastTime = clock();

  bool quit = false;
  while(!quit)
  {
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
      //bool handled = TwCustomEventSDL(event);
      //// If ant-tweakbar handled the event then don't do anything (don't want clicks to fall through)
      //if(handled)
      //  continue;

      switch(event.type)
      {
      case SDL_QUIT:
      {
        quit = true;
        break;
      }

      case SDL_MOUSEMOTION:
      {
        application->OnMouseMove(event.motion.x, event.motion.y);
        break;
      }

      case SDL_MOUSEBUTTONDOWN:
      case SDL_MOUSEBUTTONUP:
      {
        int buttonIndex = (int)MouseButtons::Left;
        if(event.button.button == SDL_BUTTON_LEFT)
          buttonIndex = (int)MouseButtons::Left;
        else if(event.button.button == SDL_BUTTON_MIDDLE)
          buttonIndex = (int)MouseButtons::Middle;
        else if(event.button.button == SDL_BUTTON_RIGHT)
          buttonIndex = (int)MouseButtons::Right;
        application->OnMouseInput(buttonIndex, event.button.state == SDL_PRESSED, event.button.x, event.button.y);
        break;
      }

      case SDL_MOUSEWHEEL:
      {
        application->OnMouseScroll(event.wheel.x, event.wheel.y);
        break;
      }

      case SDL_KEYDOWN:
      {
        int key = TranslateSDLKey(event);
        application->OnKeyDown(key);
        break;
      }

      case SDL_KEYUP:
      {
        int key = TranslateSDLKey(event);
        application->OnKeyUp(key);
        break;
      }

      case SDL_WINDOWEVENT:
      {
        switch(event.window.event)
        {
          // Handle the window being resized
        case SDL_WINDOWEVENT_RESIZED:
          Reshape(application, event.window.data1, event.window.data2);
          break;
        }
        break;
      }
      }
    }

    Idle(application, lastTime);
    Display(application, window);
  }
}

SdlApp::SdlApp()
{

}

SdlApp::~SdlApp()
{

}

bool SdlApp::Init(int windowWidth, int windowHeight)
{
  WindowWidth = windowWidth;
  WindowHeight = windowHeight;
  if(SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    printf("SDL_Init Error: %s:\n", SDL_GetError());
    return false;
  }

  // Get the 1st monitor's size
  SDL_DisplayMode current;
  int shouldBeZero = SDL_GetCurrentDisplayMode(0, &current);
  if(shouldBeZero != 0)
  {
    printf("SDL_GetCurrentDisplayMode Error: %s:\n", SDL_GetError());
    return false;
  }

  // Create our window centered in the screen
  const int WindowStartX = (current.w - WindowWidth) / 2;
  const int WindowStartY = (current.h - WindowHeight) / 2;
  window = SDL_CreateWindow("CS350 Framework", WindowStartX, WindowStartY, WindowWidth, WindowHeight,
    SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  if(window == nullptr)
  {
    printf("SDL_CreateWindow Error: %s:\n", SDL_GetError());
    return false;
  }

  // Create the open gl context
  glContext = SDL_GL_CreateContext(window);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);

  mApplication = new Application();
  mApplication->Initialize();
  mApplication->mImGui = new ImGuiHelper();
  mApplication->mImGui->Init(this);

  return true;
}

void SdlApp::Run()
{
  Reshape(mApplication, WindowWidth, WindowHeight);
  // Run the main message pump
  MainLoop(window, mApplication);
}

void SdlApp::Shutdown()
{
  SDL_GL_DeleteContext(glContext);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

