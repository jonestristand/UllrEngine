#pragma once

// Use #ifdef in future to use WinMain
#if defined(ULLR_PLATFORM_WINDOWS) && defined(ULLR_DIST)

//extern Ullr::Application* Ullr::CreateApplication();

int32 WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
  Ullr::Core::Startup();

  if (!Ullr::Core::StartupSuccess)
    return -1;

  auto app = Ullr::CreateApplication();
  app->Run();
  delete app;

  Ullr::Core::Shutdown();

  return 0;
}

#else

int32 main()
{
  Ullr::Core::Startup();

  if (!Ullr::Core::StartupSuccess)
    return -1;

  auto app = Ullr::CreateApplication();
  app->Run();
  delete app;

  Ullr::Core::Shutdown();

  return 0;
}

#endif
