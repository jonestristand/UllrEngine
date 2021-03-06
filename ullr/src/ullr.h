#pragma once

// For use by UllrEngine client applications only
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "ullr/defines.h"
#include "ullr/core.h"
#include "ullr/application.h"
#include "ullr/layer.h"
#include "ullr/log.h"

#include "ullr/events/event.h"
#include "ullr/events/applicationEvents.h"
#include "ullr/events/keyboardEvents.h"
#include "ullr/events/mouseEvents.h"

#include "ullr/input/keyCodes.h"
#include "ullr/input/mouseCodes.h"
#include "ullr/input/inputManager.h"

#include "ullr/graphics/shader.h"
#include "ullr/graphics/model.h"
#include "ullr/graphics/camera.h"
#include "ullr/graphics/loader/assimpModelLoader.h"

// --- Entry point, compiles into client binary
#include "entry.h"
