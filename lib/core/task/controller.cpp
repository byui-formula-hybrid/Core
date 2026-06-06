#include "controller.h"

namespace Core {

TaskController* TaskController::get_instance() {
    static TaskController instance;
    return &instance;
}

void TaskController::setup_task(const char* name, int priority, int attrs) {
    setup_info.name = name;
    setup_info.priority = priority;
    setup_info.attrs = attrs;

    task_setup = true;
}

int TaskController::create_task(iThreadStrategy* thread, taskFunc task, void* arg) {
    if(num_of_tasks++ > max_tasks) return -1;
    if(!task_setup) return -1;
    task_setup = false;

    thread->setup(setup_info.name, setup_info.priority, setup_info.attrs);
    
    uint32_t id = thread->create(task, arg);

    task_identifiers->emplace(id, thread);

    return id;
}

bool TaskController::delete_task(int id) {
    task_identifiers->at(id)->kill();

    delete task_identifiers->at(id);
    task_identifiers->erase(id);
    num_of_tasks--;

    return true;
}



void TaskController::set_max_tasks(size_t limit) {
    max_tasks = limit;
}

}