#pragma once

#include "Fusion/Misc/CoreTypes.h"

// Copyright (c) 2026 Neil Mewada
// SPDX-License-Identifier: MIT

namespace Fusion
{

	enum class FShaderStage : u8
	{
		Vertex,
		Fragment,
		Compute
	};

	// Selects which compiled format set to query.
	// Each format is a distinct set -- a shader only appears in a set if every
	// one of its stage modules was compiled for that format.
	enum class FShaderFormat : u8
	{
		SPIRV = 0,
	};

	// A single compiled stage within a shader (e.g. the vertex stage of "Fusion").
	// All pointer members are non-owning; data lives in static storage.
	struct FShaderModule
	{
		FShaderStage m_Stage      = FShaderStage::Vertex;
		const char*  m_EntryPoint = nullptr;

		// SPIR-V bytecode for Vulkan.
		// Aligned to 4 bytes; size is in bytes.
		// Cast to const u32* when passing to vkCreateShaderModule.
		const u8*    m_SPIRVData  = nullptr;
		SizeT        m_SPIRVSize  = 0;

		// Reserved for Metal (MSL). Not supported for now.
		const char*  m_MSLSource     = nullptr;
		SizeT        m_MSLSourceSize = 0;
	};

	// A named shader grouping one or more stage modules.
	// Graphics shaders have a vertex + fragment module; compute shaders have one.
	struct FShader
	{
		const char*          m_Name        = nullptr;
		const FShaderModule* m_Modules     = nullptr;
		SizeT                m_ModuleCount = 0;

		const FShaderModule* FindModule(FShaderStage stage) const
		{
			for (SizeT i = 0; i < m_ModuleCount; ++i)
				if (m_Modules[i].m_Stage == stage) return &m_Modules[i];
			return nullptr;
		}
	};

	// Lightweight non-owning view over a static shader set.
	// Supports range-for and index access.
	struct FShaderLibrary
	{
		const FShader* m_Data  = nullptr;
		SizeT          m_Count = 0;

		const FShader* begin() const { return m_Data; }
		const FShader* end()   const { return m_Data + m_Count; }
		SizeT          size()  const { return m_Count; }
		bool           empty() const { return m_Count == 0; }
		const FShader& operator[](SizeT i) const { return m_Data[i]; }
	};

} // namespace Fusion
