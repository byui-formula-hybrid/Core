#pragma once

#include <cstdint>
#include "../core/serialization/i_codable.h"
#include "../core/serialization/decode/i_decoder.h"
#include "../core/serialization/decode/i_keyed_decoding_container.h"
#include "../core/serialization/decode/i_unkeyed_decoding_container.h"
#include "../core/serialization/decode/decode_error.h"
#include "../core/serialization/encode/i_encoder.h"
#include "../core/serialization/encode/i_keyed_encoding_container.h"
#include "../core/serialization/encode/i_unkeyed_encoding_container.h"
#include "../core/serialization/encode/encode_error.h"

namespace Pedals {

constexpr const char* ACCELERATOR_PEDAL_PERCENTAGE = "acceleratorPedalPercentage";
constexpr const char* IS_BRAKING = "isBraking";
constexpr const char* ACCELERATOR_POT1 = "acceleratorPotentiometer1";
constexpr const char* ACCELERATOR_POT2 = "acceleratorPotentiometer2";
constexpr const char* BRAKE_POT1 = "brakePotentiometer1";
constexpr const char* BRAKE_POT2 = "brakePotentiometer2";
constexpr const char* BRAKE_STATUS = "brakeStatus";
constexpr const char* ACCELERATOR_STATUS = "acceleratorStatus";

struct PedalMessage: public Core::iCodable {
    PedalMessage() = default;

    uint8_t acceleratorPedalPercentage;
    bool isBraking;
    uint16_t acceleratorPotentiometer1;
    uint16_t acceleratorPotentiometer2;
    uint16_t brakePotentiometer1;
    uint16_t brakePotentiometer2;
    uint8_t brakeStatus;
    uint8_t acceleratorStatus;

    Core::DecodeError decode(Core::iDecoder& decoder) override;
    Core::EncodeError encode(Core::iEncoder& encoder) const override;
};

} // namespace Pedals