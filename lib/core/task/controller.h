#ifndef TASK_CONTROLLER_H
#define TASK_CONTROLLER_H

#include <cstdint>
#include <atomic>
#include <map>

#include <core/thread/i_thread_strategy.h>

namespace Core {

class TaskController {
public:
    /**
    * @brief Returns the singleton instance of the task controller.
    * @return instance: the singleton instance of the task controller.
    */
    static TaskController* get_instance();

    /**
    * @brief Setups up the task before creating the thread
    * @param name: The task name
    * @param priority: The task priority
    * @param attrs: The task attributes
    */
   void setup_task(const char* name, int priority, int attrs);

    /**
    * @brief Creates a new task and adds it to the controller's management. The task is identified by an identifier of type T, which is used to manage the task (e.g., for deletion). The specifics of how the task is created (e.g., the function it runs, its priority, stack size) can be determined by additional parameters or by a predefined configuration within the controller.
    * @param task: a function pointer or callable object that represents the task to be executed. This could be a lambda, a function pointer, or any callable that matches the expected signature for FreeRTOS tasks (e.g., void (*taskFunction)(void*)).
    * @return the identifier of the task
    */
    int create_task(iThreadStrategy* thread, taskFunc task, void* arg);

    /**
    * @brief Deletes a task from the controller's management.
    * @param idx: the index of the task to delete
    * @return success: true if the task was deleted, false otherwise.
    */
    bool delete_task(int id);

    /**
    * @brief Sets the maximum number of tasks that can be managed by the controller.
    * @param limit: the maximum number of tasks.
    */
    void set_max_tasks(size_t limit);

    /**
    * @brief Destructor for the task controller.
    */
    ~TaskController() {
        //delete[] task_identifiers; // Clean up allocated memory
    }

protected:
    // TODO: Make this a O(1) data structure
    /**
    * @brief Array to store task identifiers.
    */
    std::map<uint32_t, iThreadStrategy*>* task_identifiers;

    /**
    * @brief The maximum number of tasks that can be managed.
    */
    size_t max_tasks = 3;
    size_t num_of_tasks = 0;

    bool task_setup = false;

    struct {
        const char* name;
        int priority;
        int attrs;
    } setup_info;

    TaskController() {
        task_identifiers = new std::map<uint32_t, iThreadStrategy*>();
    }
};

} // namespace Core

#endif // TASK_CONTROLLER_H
