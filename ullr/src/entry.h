#pragma once

// Use #ifdef in future to use WinMain
#ifdef ULLR_PLATFORM_WINDOWS

//extern Ullr::Application* Ullr::CreateApplication();

int32 main()
{
  Ullr::Core::Startup();

  if (!Ullr::Core::StartupSuccess)
    return -1;

  auto app = Ullr::CreateApplication();
  app->Run();

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
