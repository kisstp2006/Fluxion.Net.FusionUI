#pragma once

namespace Fusion
{
    
    class FUSIONCORE_API FUuid
    {
    public:
        FUuid();

        FUuid(u64 low, u64 high);
        FUuid(FHash128 hash);

        static FUuid Random();

        static FUuid New();

        static FUuid Null();
        static FUuid Zero();

        static FUuid FromString(const FString& uuidString);

        bool IsNull() const;
        bool IsValid() const { return !IsNull(); }

        FUuid(const FUuid&) = default;

        bool operator==(const FUuid& other) const
        {
            return uuid[0] == other.uuid[0] && uuid[1] == other.uuid[1];
        }

        bool operator!=(const FUuid& other) const
        {
            return !operator==(other);
        }

        SizeT GetHash() const;

        FString ToString() const;

    private:
        u64 uuid[2] = { 0, 0 };
    };


} // namespace Fusion
