#include <iomanip>
#include <ios>
#include <random>
#include <sstream>

#include "Fusion/Core.h"

namespace Fusion
{

    static std::random_device gRandomDevice{};
    static std::mt19937_64 gRandomEngine(gRandomDevice());
    static std::uniform_int_distribution<u64> gUniformDistribution;

    static std::random_device gRandomDevice32{};
    static std::mt19937 gRandomEngine32(gRandomDevice32());
    static std::uniform_int_distribution<u32> gUniformDistribution32;

    FUuid::FUuid() : FUuid(0, 0)
    {

    }

    FUuid::FUuid(u64 low, u64 high)
    {
        uuid[0] = low;
        uuid[1] = high;
    }

    FUuid::FUuid(FHash128 hash)
    {
        uuid[0] = hash.low64;
        uuid[1] = hash.high64;
    }

    FUuid FUuid::Random()
    {
        return FUuid(gUniformDistribution(gRandomEngine), gUniformDistribution(gRandomEngine));
    }

    FUuid FUuid::New()
    {
        return Random();
    }

    FUuid FUuid::Null()
    {
        return FUuid(0, 0);
    }

    FUuid FUuid::Zero()
    {
        return Null();
    }

    FUuid FUuid::FromString(const FString& uuidString)
    {
        std::string uuidStr = uuidString.ToStdString();

        if (uuidStr.size() != 36 ||
            uuidStr[8] != '-' || uuidStr[13] != '-' || uuidStr[18] != '-' || uuidStr[23] != '-') {
            return FUuid::Zero(); // Invalid format
        }

        FUuid result = FUuid::Zero();

        // TODO: Do not use exception below for error handling.

        try {
            // Parse each section of the UUID
            uint32_t part1 = std::stoul(uuidStr.substr(0, 8), nullptr, 16);
            uint16_t part2 = (uint16_t)std::stoul(uuidStr.substr(9, 4), nullptr, 16);
            uint16_t part3 = (uint16_t)std::stoul(uuidStr.substr(14, 4), nullptr, 16);
            uint16_t part4 = (uint16_t)std::stoul(uuidStr.substr(19, 4), nullptr, 16);
            uint64_t part5 = (uint16_t)std::stoull(uuidStr.substr(24, 12), nullptr, 16);

            // Combine parts into data[0] and data[1]
            result.uuid[0] = (static_cast<uint64_t>(part1) << 32) |
                (static_cast<uint64_t>(part2) << 16) |
                part3;
            result.uuid[1] = (static_cast<uint64_t>(part4) << 48) | part5;

            return result; // Successful conversion
        }
        catch (...) {
            return FUuid::Zero(); // Conversion failed due to invalid hex values
        }
    }

    bool FUuid::IsNull() const
    {
        return uuid[0] == 0 && uuid[1] == 0;
    }

    SizeT FUuid::GetHash() const
    {
        return GetCombinedHash(uuid[0], uuid[1]);
    }

    FString FUuid::ToString() const
    {
        std::stringstream ss;
        ss << std::hex << std::setfill('0')
            << std::setw(8) << (static_cast<uint32_t>(uuid[0] >> 32)) << "-"
            << std::setw(4) << (static_cast<uint16_t>(uuid[0] >> 16)) << "-"
            << std::setw(4) << (static_cast<uint16_t>(uuid[0])) << "-"
            << std::setw(4) << (static_cast<uint16_t>(uuid[1] >> 48)) << "-"
            << std::setw(12) << (uuid[1] & 0x0000FFFFFFFFFFFFULL);
        return ss.str();
    }
    
} // namespace Fusion
