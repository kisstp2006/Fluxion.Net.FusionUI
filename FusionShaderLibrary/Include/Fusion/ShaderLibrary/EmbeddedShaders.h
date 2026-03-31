#pragma once

#include "Fusion/ShaderLibrary/ShaderTypes.h"

// Copyright (c) 2026 Neil Mewada
// SPDX-License-Identifier: MIT


namespace Fusion::Shaders {

	// All shaders compiled into the library, in registration order.
	// Supports range-for:
	//   for (const FShader& shader : Fusion::Shaders::All()) { ... }
	FShaderView All();

	// Find a shader by name. Returns nullptr if not found.
	const FShader* FindShader(const char* name);

} // namespace Fusion::Shaders
