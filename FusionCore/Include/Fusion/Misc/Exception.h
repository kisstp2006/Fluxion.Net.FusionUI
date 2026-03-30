#pragma once

#include "Containers/String.h"

#include <exception>
#include "cpptrace/cpptrace.hpp"

#if FUSION_EXCEPTIONS

#define FUSION_TRY try
#define FUSION_CATCH(exceptionStatement) catch (exceptionStatement)

#else

#define FUSION_TRY if constexpr (true)
#define FUSION_CATCH(exceptionStatement) else if constexpr (false)

#endif

namespace Fusion
{

    class FUSIONCORE_API FException : public std::exception
    {
    public:

        FException() : message("Unknown error"), stackTrace(cpptrace::generate_trace())
        {
	        
        }

        FException(const FString& message) : message(message), stackTrace(cpptrace::generate_trace())
        {
	        
        }

        const char* what() const throw () 
    	{
            return message.CStr();
        }

        const cpptrace::stacktrace& GetStackTrace() const
        {
            return stackTrace;
		}

        FString GetStackTraceString(bool useColors) const
        {
            return FString(stackTrace.to_string(useColors));
		}

    private:

        FString message;
        cpptrace::stacktrace stackTrace;
    };

} // namespace Fusion
