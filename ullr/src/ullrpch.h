#pragma once

#include <iostream>
#include <fstream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "ullr/log.h"

#ifdef ULLR_PLATFORM_WINDOWS
  #include <windows.h>
#endif

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "ullr/utils/glmLogger.h"

#include "glad/glad.h"
