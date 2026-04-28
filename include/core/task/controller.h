#ifndef TASK_CONTROLLER_H
#define TASK_CONTROLLER_H

#include <cstdint>
#include <atomic>

namespace Core {

    template<typename T, typename U>
class TaskController {
public:
    /**
    * @brief Returns the singleton instance of the task controller.
    * @return instance: the singleton instance of the task controller.
    */
    TaskController get_instance() {
        static TaskController instance;
        return instance;
    }

    /**
    * @brief Creates a new task and adds it to the controller's management. The task is identified by an identifier of type T, which is used to manage the task (e.g., for deletion). The specifics of how the task is created (e.g., the function it runs, its priority, stack size) can be determined by additional parameters or by a predefined configuration within the controller.
    * @param task: a function pointer or callable object that represents the task to be executed. This could be a lambda, a function pointer, or any callable that matches the expected signature for FreeRTOS tasks (e.g., void (*taskFunction)(void*)).
    * @return identifier: string  
    */
    virtual T create_task(U task) = 0;

    /**
    * @brief Deletes a task from the controller's management.
    * @param identifier: the identifier of the task to be deleted.
    * @return success: true if the task was deleted, false otherwise.
    */
    virtual bool delete_task(T identifier) = 0;

    /**
    * @brief Sets the maximum number of tasks that can be managed by the controller.
    * @param limit: the maximum number of tasks.
    */
    void set_max_tasks(size_t limit) {
        delete[] task_identifiers; // Clean up existing identifiers if any
        task_identifiers = new T[limit](); // Allocate new array for task identifiers
        max_tasks = limit;
    }

    /**
    * @brief Destructor for the task controller.
    */
    ~TaskController() {
        delete[] task_identifiers; // Clean up allocated memory
    }

private:
    TaskController() {
        task_identifiers = new T[max_tasks]();
    }

    // TODO: Make this a O(1) data structure
    /**
    * @brief Array to store task identifiers.
    */
    T* task_identifiers;

    /**
    * @brief The maximum number of tasks that can be managed.
    */
    size_t max_tasks = 0;
};

} // namespace Core

#endif // TASK_CONTROLLER_H