#pragma once
#include <cstdint>
#include <cstddef>
#include <algorithm>
#include <memory>
#include <stdexcept> // for std::out_of_range

namespace Core {

/*
* @brief A class representing a data buffer for serialization
* 
* This class allows storing and retrieving data in a bit-packed format, 
* automatically packing them into bytes. It behaves like a dynamic array of bits,
* where bits are added sequentially and packed into bytes as needed.
*/
class Data {
public:
    /*
    * @brief Enumeration representing potential errors that can occur during data operations
    */
    enum class Error { None, InsufficientCapacity };

    /*
    * @brief Constructor to initialize the data buffer with a specified capacity
    * @param capacity The capacity of the data buffer in bytes
    */
    Data(std::size_t capacity = 8);

    /*
    * @brief Deep copy constructor
    * @param other The Data object to copy from
    */
    Data(const Data& other);

    /*
    * @brief Assign from another Data object
    * @param other The Data object to copy from
    */
    Data& operator=(const Data& other);

    /*
    * @brief Append a value to the current end of the data buffer
    * @param value The value to append
    * @return Error indicating success or failure
    */
    template <typename T>
    Error write(const T value);

    // TODO: Comment
    template <typename T>
    Error readFromBit(T& value, size_t bitOffset = 0) {
        return Error::None;
    }

    // TODO: Comment
    template <typename T>
    Error readFromBit(T& value, size_t byteOffset = 0, size_t bitOffset = 0) {
        return Error::None;
    }

    // TODO: Comment
    template <typename T>
    Error readFromByte(T& value, size_t byteOffset = 0) {
        return Error::None;
    }

    /*
    * @brief Reset the data buffer
    * @return Error indicating success or failure
    */
    void clear();

    /*
    * @brief Get the raw bytes of the data buffer
    * @param outBuffer The output buffer to copy the data into
    * @param length The number of bytes to copy
    * @return Error indicating success or failure
    */
    const uint8_t* bytes() const;

    /*
    * @brief Get the size of the data buffer
    * @param outSize The size of the data buffer
    * @return Error indicating success or failure
    */
    const size_t size() const;

    /*
    * @brief Get the capacity of the data buffer
    * @param outCapacity The capacity of the data buffer
    * @return Error indicating success or failure
    */
    const size_t capacity() const;

    /*
    * @brief Destructor for the Data class
    */
    ~Data() = default;

private:
    std::size_t _capacity;
    std::size_t _bitOffset;
    std::size_t _byteOffset;
    std::unique_ptr<uint8_t[]> _buffer;
};

} // namespace Core
