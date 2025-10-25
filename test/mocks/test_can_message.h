#include <cstdint>

struct TestCANMessage {
    uint64_t a_bool: 1;
    uint64_t padding: 7;
    uint64_t a_uint8: 8;
    uint64_t a_uint16: 16;
    uint64_t a_uint32: 32;
};