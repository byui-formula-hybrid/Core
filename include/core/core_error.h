#ifndef CORE_ERROR_H
#define CORE_ERROR_H

namespace Core {

/**
 * @brief Errors returned from iQueue
 */
enum class QueueError : int {
    SUCCESS = 0,
    FULL = -1,
    EMPTY = -2,
    TIMEOUT = -3,
};

/**
 * @brief Errors returned from the TaskController
 */
enum class TaskError : int {
    SUCCESS = 0,
    MAX_LIMIT = -1,
    THREAD_INIT = -2,
};

/**
 * @brief Allows us to return a task id or an error
 *
 * This class is only 4 bytes, so it is easily returned
 */
class TaskResult {
private:
    int value;

public:
    /**
     * @brief Construct with an integer
     */
    constexpr TaskResult(int id) noexcept : value(id) {}
    /**
     * @brief Construct with a TaskError
     */
    constexpr TaskResult(TaskError err) noexcept : value(static_cast<int>(err)) {}

    /**
     * @brief Overload the bool operator to quickly check for error
     */
    constexpr explicit operator bool() const noexcept {
        return value >= 0;
    }

    /**
     * @brief Allow for equality comparisons to TaskErrors
     */
    constexpr friend bool operator==(TaskResult self, TaskError err) noexcept {
        return self.value==static_cast<int>(err);
    }

    /**
     * @brief Get as an integer for use as an id
     */
    constexpr int id() const noexcept { return value; }
    /**
     * @brief Get as a TaskError for use as the error
     */
    constexpr TaskError error() const noexcept { return static_cast<TaskError>(value); }
};

/**
 * @brief Tracks the types of errors we get for handling at a higher level
 */
enum class ErrorType {
    SUCCESS = 0,
    CLASS_NOT_INITIALIZED = -1,
    QUEUE_ERROR = -2,
    TASK_ERROR = -3,
};

/**
 * @brief Stores an error casted to int and the ErrorType for interpreting the errors
 */
class Result {
private:
    /**
     * @brief The Error return from called function, 0 or higher indicates success
     */
    int err;
    /**
     * @brief What kind of error was recieved
     */
    ErrorType type;
public:
    /**
     * @brief Generic Verbose Constructor
     */
    constexpr explicit Result(int err, ErrorType type) noexcept : err(err), type(type) {}

    /**
     * @brief Automatically interpret QueueErrors. Implicit for quick returns
     */
    constexpr Result(QueueError err) noexcept :
                                err(static_cast<int>(err)),
                                type(err == QueueError::SUCCESS ? ErrorType::SUCCESS : ErrorType::QUEUE_ERROR) {}

    /**
    * @brief Automatically interpret TaskErrors. Implicit for quick returns
    */
    constexpr Result(TaskError err) noexcept :
                                err(static_cast<int>(err)),
                                type(err == TaskError::SUCCESS ? ErrorType::SUCCESS : ErrorType::TASK_ERROR) {}

    /**
    * @brief Allow for quick success checks
    */
    constexpr operator bool() const noexcept {
        return err >= 0 && type == ErrorType::SUCCESS;
    }

    /**
    * @brief Allow for checking the ErrorType using == and !=
    */
    constexpr friend bool operator==(Result self, ErrorType type) noexcept {
        return self.type == type;
    }

    /**
     * @brief Get the Error
     */
    constexpr int Error() const noexcept { return err; }
    /**
     * @brief Get the Error Type
     */
    constexpr ErrorType Type() const noexcept { return type; }
};
}

#endif
