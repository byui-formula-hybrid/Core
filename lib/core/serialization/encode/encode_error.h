#pragma once

namespace Core {

enum class EncodeError {
    None = 0,
    EndOfContainer,
    NoContainer,
    NotSupported,
    NotImplemented,
    UnknownError
};

} // namespace Core