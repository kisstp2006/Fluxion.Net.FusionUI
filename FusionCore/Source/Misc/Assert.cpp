#include "Fusion/Misc/Assert.h"
#include "Fusion/Misc/Exception.h"
#include "Fusion/Containers/String.h"

#include <cassert>

namespace Fusion::Internal
{
    [[noreturn]] void AssertFailed(const char* condition, const char* message, const char* file, int line)
    {
        FString fullMessage = FString(file) + ":" + std::to_string(line) + ": Assertion failed: (" + condition + ") — " + message;
#if FUSION_EXCEPTIONS
        throw FException(fullMessage);
#else
        FUSION_LOG_CRITICAL("Assert", fullMessage.CStr());
        assert(false && fullMessage.CStr());
#endif
    }

} // namespace Fusion::Internal
