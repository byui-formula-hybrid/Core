#ifndef CAN_TYPES_H
#define CAN_TYPES_H

#include <stdint.h>
#include <cstring>

namespace CAN {

/**
* @brief Enum for specifying the type of CAN service configuration.
*/
enum ServiceConfigType {
    TWAI
};

/**
 * @brief Base class for CAN service configuration.
 */
struct BaseServiceConfig {
    ServiceConfigType type;
    explicit BaseServiceConfig(ServiceConfigType t) : type(t) {}
};

/**
 * @brief Structure for representing a CAN frame.
 */
struct Frame {
    union {
        struct {
            // The order of these bits must match deprecated message flags for compatibility reasons
            uint32_t extd: 1;           /**< Extended Frame Format (29bit ID) */
            uint32_t rtr: 1;            /**< Message is a Remote Frame */
            uint32_t ss: 1;             /**< Transmit as a Single Shot Transmission. Unused for received. */
            uint32_t self: 1;           /**< Transmit as a Self Reception Request. Unused for received. */
            uint32_t dlc_non_comp: 1;   /**< Message's Data length code is larger than 8. This will break compliance with ISO 11898-1 */
            uint32_t reserved: 27;      /**< Reserved bits */
        };
        uint32_t flags;                 /**< Deprecated: Alternate way to set bits using message flags */
    };
    uint32_t identifier;                /**< 11 or 29 bit identifier */
    uint8_t data_length_code;           /**< Data length code */
    uint8_t data[8];    /**< Data bytes (not relevant in RTR frame) */

    /*
    ** @brief Default constructor for the CAN frame.
    */
    Frame() {}

    /**
     * @brief Constructor for the CAN frame.
     * @param identifier: The frame identifier.
     * @param message: A pointer to the message data.
     */
    template<typename T>
    Frame(uint32_t identifier, T* message) {
        static_assert(sizeof(T) <= 8, "Message too large for CAN frame");
        
        this->data_length_code = 8;
        this->identifier = identifier;

        const uint8_t* msg_ptr = (const uint8_t*) message;
        this->data[0] = msg_ptr[0];
        this->data[1] = msg_ptr[1];
        this->data[2] = msg_ptr[2];
        this->data[3] = msg_ptr[3];
        this->data[4] = msg_ptr[4];
        this->data[5] = msg_ptr[5];
        this->data[6] = msg_ptr[6];
        this->data[7] = msg_ptr[7];
    }

    /**
     * @brief Constructor for the CAN frame.
     * @param identifier: The frame identifier.
     * @param data: A reference to the data array.
     */
    Frame(uint32_t identifier, uint8_t (&data)[8]) {
        this->data_length_code = 8;
        this->identifier = identifier;
        this->data[0] = data[0];
        this->data[1] = data[1];
        this->data[2] = data[2];
        this->data[3] = data[3];
        this->data[4] = data[4];
        this->data[5] = data[5];
        this->data[6] = data[6];
        this->data[7] = data[7];
    }

    /**
     * @brief Decodes the frame data into a specific type.
     * @return pointer to the decoded data.
     */
    template<typename T>
    T* decode() {
        return (T*) data;
    }
};

/**
 * @brief Interface for handling CAN frames.
 */
class IHandler {
public:
    /**
     * @brief Handles a CAN frame.
     * @param data: The frame to handle.
     */
    virtual void handle(const Frame& data) = 0;

    /**
     * @brief Destructor for the CAN handler.
     */
    virtual ~IHandler() = default;
};

} // namespace CAN

#endif // CAN_TYPES_H