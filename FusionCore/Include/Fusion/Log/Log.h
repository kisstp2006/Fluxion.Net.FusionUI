#pragma once

#include "Fusion/Misc/CoreDefines.h"

namespace Fusion
{
    class FString;

    enum class FLogLevel
    {
        Trace,
        Info,
        Warning,
        Error,
        Critical
    };

    // Signature for a host-provided log sink.
    // Called on every FUSION_LOG_* invocation.
    // Thread-safety of the sink is the host's responsibility.
    using FLogSink = void(*)(FLogLevel level, const char* category, const char* message);

    // Register the host's log sink. Call once at application startup before
    // any threads are spawned. If no sink is registered, logs go to stderr.
    FUSIONCORE_API void SetLogSink(FLogSink sink);

    // Internal dispatch — use the macros below instead of calling this directly.
    FUSIONCORE_API void Log(FLogLevel level, const char* category, const FString& message);

    template<typename... Args>
    void LogFormat(FLogLevel level, const char* category, const char* message, Args&&... args);

} // namespace Fusion

// clang-format off
#define FUSION_LOG(level, category, message, ...)          ::Fusion::LogFormat(level, category, message, ##__VA_ARGS__)
#define FUSION_LOG_TRACE(category, message, ...)           ::Fusion::LogFormat(::Fusion::FLogLevel::Trace,    category, message, ##__VA_ARGS__)
#define FUSION_LOG_INFO(category, message, ...)            ::Fusion::LogFormat(::Fusion::FLogLevel::Info,     category, message, ##__VA_ARGS__)
#define FUSION_LOG_WARNING(category, message, ...)         ::Fusion::LogFormat(::Fusion::FLogLevel::Warning,  category, message, ##__VA_ARGS__)
#define FUSION_LOG_ERROR(category, message, ...)           ::Fusion::LogFormat(::Fusion::FLogLevel::Error,    category, message, ##__VA_ARGS__)
#define FUSION_LOG_CRITICAL(category, message, ...)        ::Fusion::LogFormat(::Fusion::FLogLevel::Critical, category, message, ##__VA_ARGS__)
// clang-format on
