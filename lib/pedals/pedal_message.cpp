#include "pedal_message.h"
#include "../core/serialization/binary/encode/binary_unkeyed_encoding_container.h"

namespace Pedals {

Core::DecodeError PedalMessage::decode(Core::iDecoder& decoder) {
auto keyedContainer = decoder.container();
    if (keyedContainer != nullptr) {
        Core::DecodeError err = Core::DecodeError::None;
        err = keyedContainer->decodeUInt8(acceleratorPedalPercentage, ACCELERATOR_PEDAL_PERCENTAGE); if (err != Core::DecodeError::None) return err;
        err = keyedContainer->decodeBool(isBraking, IS_BRAKING); if (err != Core::DecodeError::None) return err;
        err = keyedContainer->decodeUInt16(acceleratorPotentiometer1, ACCELERATOR_POT1); if (err != Core::DecodeError::None) return err;
        err = keyedContainer->decodeUInt16(acceleratorPotentiometer2, ACCELERATOR_POT2); if (err != Core::DecodeError::None) return err;
        err = keyedContainer->decodeUInt16(brakePotentiometer1, BRAKE_POT1); if (err != Core::DecodeError::None) return err;
        err = keyedContainer->decodeUInt16(brakePotentiometer2, BRAKE_POT2); if (err != Core::DecodeError::None) return err;
        err = keyedContainer->decodeUInt8(brakeStatus, BRAKE_STATUS); if (err != Core::DecodeError::None) return err;
        err = keyedContainer->decodeUInt8(acceleratorStatus, ACCELERATOR_STATUS); if (err != Core::DecodeError::None) return err;
        return Core::DecodeError::None;
    } else {
        auto unkeyedContainer = decoder.unkeyedContainer();
        if (unkeyedContainer != nullptr) {
        Core::DecodeError err = Core::DecodeError::None;
        err = unkeyedContainer->decodeUInt8(acceleratorPedalPercentage); if (err != Core::DecodeError::None) return err;
        err = unkeyedContainer->decodeBool(isBraking); if (err != Core::DecodeError::None) return err;
        err = unkeyedContainer->decodeUInt16(acceleratorPotentiometer1); if (err != Core::DecodeError::None) return err;
        err = unkeyedContainer->decodeUInt16(acceleratorPotentiometer2); if (err != Core::DecodeError::None) return err;
        err = unkeyedContainer->decodeUInt16(brakePotentiometer1); if (err != Core::DecodeError::None) return err;
        err = unkeyedContainer->decodeUInt16(brakePotentiometer2); if (err != Core::DecodeError::None) return err;
        err = unkeyedContainer->decodeUInt8(brakeStatus); if (err != Core::DecodeError::None) return err;
        err = unkeyedContainer->decodeUInt8(acceleratorStatus); if (err != Core::DecodeError::None) return err;
        return Core::DecodeError::None;
        }
    }
    return Core::DecodeError::NoContainer;
}

Core::EncodeError PedalMessage::encode(Core::iEncoder& encoder) const {
    auto keyedContainer = encoder.container();
    if (keyedContainer != nullptr) {
        Core::EncodeError err = Core::EncodeError::None;
        err = keyedContainer->encodeUInt8(acceleratorPedalPercentage, ACCELERATOR_PEDAL_PERCENTAGE); if (err != Core::EncodeError::None) return err;
        err = keyedContainer->encodeBool(isBraking, IS_BRAKING); if (err != Core::EncodeError::None) return err;
        err = keyedContainer->encodeUInt16(acceleratorPotentiometer1, ACCELERATOR_POT1); if (err != Core::EncodeError::None) return err;
        err = keyedContainer->encodeUInt16(acceleratorPotentiometer2, ACCELERATOR_POT2); if (err != Core::EncodeError::None) return err;
        err = keyedContainer->encodeUInt16(brakePotentiometer1, BRAKE_POT1); if (err != Core::EncodeError::None) return err;
        err = keyedContainer->encodeUInt16(brakePotentiometer2, BRAKE_POT2); if (err != Core::EncodeError::None) return err;
        err = keyedContainer->encodeUInt8(brakeStatus, BRAKE_STATUS); if (err != Core::EncodeError::None) return err;
        err = keyedContainer->encodeUInt8(acceleratorStatus, ACCELERATOR_STATUS); if (err != Core::EncodeError::None) return err;
        return Core::EncodeError::None;
    } else {
        auto unkeyedContainer = encoder.unkeyedContainer();
        if (unkeyedContainer != nullptr) {
            Core::EncodeError err = Core::EncodeError::None;
            err = unkeyedContainer->encodeUInt8(acceleratorPedalPercentage); if (err != Core::EncodeError::None) return err;
            err = unkeyedContainer->encodeBool(isBraking); if (err != Core::EncodeError::None) return err;
            err = unkeyedContainer->encodeUInt16(acceleratorPotentiometer1); if (err != Core::EncodeError::None) return err;
            err = unkeyedContainer->encodeUInt16(acceleratorPotentiometer2); if (err != Core::EncodeError::None) return err;
            err = unkeyedContainer->encodeUInt16(brakePotentiometer1); if (err != Core::EncodeError::None) return err;
            err = unkeyedContainer->encodeUInt16(brakePotentiometer2); if (err != Core::EncodeError::None) return err;
            err = unkeyedContainer->encodeUInt8(brakeStatus); if (err != Core::EncodeError::None) return err;
            err = unkeyedContainer->encodeUInt8(acceleratorStatus); if (err != Core::EncodeError::None) return err;
            return Core::EncodeError::None;
        }
    }
    return Core::EncodeError::NoContainer;
}

} // namespace Pedals
