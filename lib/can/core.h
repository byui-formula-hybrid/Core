#include "twai_provider.h"

using namespace TWAI;

namespace CAN {

template<typename T>
struct Message {
    uint32_t id;
    T* data;
};

/*
 * Encodes a message of type T into a CAN frame.
 * @param id The identifier for the CAN frame.
 * @param message The message of type T to encode.
 * @returns The encoded CAN frame.
 */
template<typename T>
const Frame encode(const Message<T> &message) {
    const uint8_t* msg_ptr = (const uint8_t*) message.data;
    Frame frame = {};
    frame.identifier = message.id;
    frame.data_length_code = 8;
    frame.data[0] = msg_ptr[0];
    frame.data[1] = msg_ptr[1];
    frame.data[2] = msg_ptr[2];
    frame.data[3] = msg_ptr[3];
    frame.data[4] = msg_ptr[4];
    frame.data[5] = msg_ptr[5];
    frame.data[6] = msg_ptr[6];
    frame.data[7] = msg_ptr[7];
    return frame;
}

/*
 * Decodes a CAN frame into a message of type T.
 * @param frame The CAN frame to decode.
 * @returns The decoded message of type T.
 */ 
template<typename T>
const Message<T> decode(const Frame* frame) {
    Message<T> message = {};
    // todo ensure that frame data length code is 8
    // error handling
    message.data = (T*) frame->data;
    message.id = frame->identifier;
    return message;
}

}