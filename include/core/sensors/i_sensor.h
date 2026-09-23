#ifndef CORE_SENSOR_H
#define CORE_SENSOR_H

namespace Core {

/**
 * @brief Generic sensor class. This should be implemented per architecture
 * 
 * This class makes use of configurations so that a wide variety of communication methods can be supported
 */
template<typename T>
class iSensor {
public:
    /**
     * @brief default destructor
     */
    virtual ~iSensor() = default;

    /**
     * @brief Initialize the sensor using configuration
     *
     * @param config: void*
     * @return error code
     */
    virtual int initialize(void* config) = 0;

    /**
     * @brief Updates the sensor's stored value
     *
     * @return value read from sensor
     */
    virtual T read() = 0;

    /**
     * @brief Get stored value
     */
    T get_value() const {
        return data;
    }

protected:
    T data{};
};

}

#endif
