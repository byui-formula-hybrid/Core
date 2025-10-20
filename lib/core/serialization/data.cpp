#include "data.h"

namespace Core {

Data::Data(std::size_t capacity) :  _capacity(capacity),
                                    _bitOffset(0),
                                    _byteOffset(0),
                                    _buffer(std::make_unique<uint8_t[]>(capacity)) {
    std::fill_n(_buffer.get(), capacity, 0);
}

Data::Data(const Data& other) : _capacity(other._capacity), 
                                _bitOffset(other._bitOffset), 
                                _byteOffset(other._byteOffset), 
                                _buffer(std::make_unique<uint8_t[]>(other._capacity)) {
    std::copy_n(other._buffer.get(), other._capacity, _buffer.get());
}

Data& Data::operator=(const Data& other) {
    if (this != &other) {
        _capacity = other._capacity;
        _bitOffset = other._bitOffset;
        _byteOffset = other._byteOffset;
        _buffer = std::make_unique<uint8_t[]>(other._capacity);
        std::copy_n(other._buffer.get(), other._capacity, _buffer.get());
    }
    return *this;
}

Data::Error Data::append(uint64_t value) {
    if (_byteOffset >= _capacity) {
        return Data::Error::InsufficientCapacity;
    }

    // Append the value to the buffer
    std::size_t bitsToAppend = 64 - __builtin_clzll(value);
    std::size_t bytesToAppend = (bitsToAppend + 7) / 8;

    if (_byteOffset + bytesToAppend > _capacity) {
        return Data::Error::InsufficientCapacity;
    }

    for (std::size_t i = 0; i < bytesToAppend; ++i) {
        _buffer[_byteOffset + i] = (value >> ((_bitOffset + i * 8) % 64)) & 0xFF;
    }

    _bitOffset += bitsToAppend;
    _byteOffset += bytesToAppend;

    return Data::Error::None;
}

Data::Error Data::read(uint64_t& value, std::size_t bits) {
    if (_byteOffset * 8 + _bitOffset + bits > _capacity * 8) {
        return Data::Error::InsufficientCapacity;
    }

    value = 0;
    for (std::size_t i = 0; i < bits; ++i) {
        std::size_t byteIndex = (_byteOffset + (i + _bitOffset) / 8);
        std::size_t bitIndex = (i + _bitOffset) % 8;
        value |= ((static_cast<uint64_t>(_buffer[byteIndex]) >> bitIndex) & 0x01) << i;
    }

    _bitOffset += bits;
    _byteOffset += (_bitOffset / 8);
    _bitOffset %= 8;

    return Data::Error::None;
}

void Data::clear() {
    _bitOffset = 0;
    _byteOffset = 0;
    std::fill_n(_buffer.get(), _capacity, 0);
}

const uint8_t* Data::bytes() const {
    return _buffer.get();
}

const size_t Data::size() const {
    return _byteOffset;
}

const size_t Data::capacity() const {
    return _capacity;
}

} // namespace Core