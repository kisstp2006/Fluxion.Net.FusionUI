#pragma once

#include <Fusion/Core.h>
#include <Fusion/Platform.h>

#if FUSION_USE_SDL3
#include <Fusion/SDL3Platform.h>
#endif

#include <Fusion/RHI.h>

#if FUSION_USE_VULKAN
#include <Fusion/VulkanRHI.h>
#endif

#include "Application/ApplicationInstance.h"
#include "Application/Service.h"
#include "Application/Application.h"

