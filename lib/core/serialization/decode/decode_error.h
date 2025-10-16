#pragma once

namespace Core {

enum class DecodeError {
    None = 0,
    NoContainer,
    NotSupported,
    UnknownError
};

} // namespace Core