#pragma once

enum Platforms {
  Windows,
  Linux,
  MacOS,

  OpenGL,
  DirectX
};

static Platforms GetPlatform() {
  return Platforms::Windows | Platforms::OpenGL;
}
