#include "Fusion/Resources/EmbeddedResources.h"

#include <cstring>

namespace Fusion::Resources
{

    const FResource* Find(const char* path)
    {
        const FResource* data  = Internal::GetData();
        const SizeT      count = Internal::GetCount();

        SizeT lo = 0;
        SizeT hi = count;

        while (lo < hi)
        {
            const SizeT mid = lo + (hi - lo) / 2;
            const int   cmp = std::strcmp(data[mid].Path, path);

            if      (cmp < 0) lo = mid + 1;
            else if (cmp > 0) hi = mid;
            else              return &data[mid];
        }

        return nullptr;
    }

    FResourceView ListAll(const char* folderPath)
    {
        const FResource* data      = Internal::GetData();
        const SizeT      count     = Internal::GetCount();
        const SizeT      prefixLen = std::strlen(folderPath);

        // Find the first entry whose path is >= folderPath using strncmp so that
        // entries that START with folderPath compare as equal and are included.
        SizeT lo = 0;
        SizeT hi = count;

        while (lo < hi)
        {
            const SizeT mid = lo + (hi - lo) / 2;
            if (std::strncmp(data[mid].Path, folderPath, prefixLen) < 0)
                lo = mid + 1;
            else
                hi = mid;
        }

        const FResource* start   = data + lo;
        const FResource* end     = start;
        const FResource* dataEnd = data + count;

        while (end < dataEnd && std::strncmp(end->Path, folderPath, prefixLen) == 0)
            ++end;

        return { start, end };
    }

} // namespace Fusion::Resources
