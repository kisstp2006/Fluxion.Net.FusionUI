#pragma once

namespace Fusion
{

    class FUSIONPLATFORM_API IPlatformBackend
	{
    public:

        virtual ~IPlatformBackend() = default;
        
        virtual void Initialize() = 0;

        virtual void Shutdown() = 0;

	};
    
} // namespace Fusion
