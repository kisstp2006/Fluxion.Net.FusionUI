#include "Fusion/Core.h"

#include <xxhash.h>

namespace Fusion
{

	FUSIONCORE_API FHash128 CalculateHash128(const void* data, SizeT length)
	{
		XXH128_hash_t hash = XXH3_128bits(data, length);
		FHash128 out;
		out.high64 = hash.high64;
		out.low64 = hash.low64;
		return out;
	}

	FUSIONCORE_API SizeT CalculateHash(const void* data, SizeT length)
	{
#if FUSION_64BIT
		return XXH64(data, length, 0);
#else
		return XXH32(data, length, 0);
#endif
	}

	FUSIONCORE_API SizeT CombineHashes(const SizeT* hashes, SizeT numHashes)
	{
		if (numHashes == 0)
			return 0;

		SizeT hash = hashes[0];

		for (int i = 1; i < numHashes; i++)
		{
			hash = GetCombinedHash(hash, hashes[i]);
		}

		return hash;
	}

} // namespace Fusion
