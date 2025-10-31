#include "pot_converter.h"

PotConverter::PotConverter() {
    head = nullptr;
    tail = nullptr;
    count = 0;
    total = 0;
}

PotConverter::~PotConverter() {
    // Free all nodes in the linked list to avoid memory leaks
    Node* current = head;
    while (current) {
        Node* temp = current;
        current = current->next;
        // TODO: consider using a memory pool to avoid fragmentation
        delete temp;
    }
}

// Converts a raw ADC value (0–4095) to a percentage (0–100)
// TODO: return error as enum instead of -1.0
double PotConverter::convertPotReading(int reading) {
    if (reading < MIN_ADC_VALUE || reading > MAX_ADC_VALUE) {
        return -1.0;
    }
    return (reading / 4095.0) * 100.0;
}

// Adds a new reading to the linked list and computes the moving average
double PotConverter::smoothReading(double newValue) {
    // Create a new node for the new reading
    // TODO: new allocates memory and could lead to fragmentation over time
    // TODO: consider using a fixed-size circular buffer instead
    Node* newNode = new Node { newValue, nullptr };
    total += newValue;

    // Add it to the end of the linked list
    if (!head) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
    count++;

    // If we exceed the max number of samples, remove the oldest one
    if (count > MAX_SAMPLES) {
        Node* oldNode = head;
        total -= oldNode->value;
        head = head->next;
        delete oldNode;
        count--;
    }

    // Return the current average
    return total / count;
}