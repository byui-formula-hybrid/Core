#pragma once
#include <cstdint>
#include <cstddef>
#include <algorithm>
#include <memory>
#include <stdexcept> // for std::out_of_range

namespace Core {

class Data {
public:
    explicit Data(std::size_t capacity)
    : _capacity(capacity), _size(0), _buffer(std::make_unique<uint8_t[]>(capacity)) {}

    // Copy constructor
    Data(const Data& other)
    : _capacity(other._capacity), _size(other._size),
      _buffer(std::make_unique<uint8_t[]>(other._capacity)) {
        std::copy(other._buffer.get(), other._buffer.get() + other._size, _buffer.get());
    }

    // Move constructor
    Data(Data&& other) noexcept
    : _capacity(other._capacity), _size(other._size), _buffer(std::move(other._buffer)) {
        other._size = 0;
        other._capacity = 0;
    }

    // Append array of bytes
    void append(const uint8_t* bytes, std::size_t length) {
        std::size_t available = _capacity - _size;
        std::size_t toCopy = (length < available) ? length : available;
        std::copy(bytes, bytes + toCopy, _buffer.get() + _size);
        _size += toCopy;
    }

    // Append single byte
    void append(uint8_t byte) {
        if (_size < _capacity)
            _buffer[_size++] = byte;
    }

    // Subscript operators
    uint8_t& operator[](std::size_t index) {
        if (index >= _size)
            throw std::out_of_range("Data index out of range");
        return _buffer[index];
    }

    const uint8_t& operator[](std::size_t index) const {
        if (index >= _size)
            throw std::out_of_range("Data index out of range");
        return _buffer[index];
    }

    // Accessors
    const uint8_t* bytes() const { return _buffer.get(); }
    uint8_t* bytes() { return _buffer.get(); }

    std::size_t size() const { return _size; }
    std::size_t capacity() const { return _capacity; }

    void clear() { _size = 0; }

private:
    std::size_t _capacity;
    std::size_t _size;
    std::unique_ptr<uint8_t[]> _buffer;
};

} // namespace Core
