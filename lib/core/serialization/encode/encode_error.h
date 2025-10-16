#pragma once

namespace Core {

enum class EncodeError {
    None = 0,
    EndOfContainer,
    NoContainer,
    NotSupported,
    UnknownError
};

} // namespace Core