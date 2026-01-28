#ifndef POT_CONVERTER_H
#define POT_CONVERTER_H

class [[deprecated]] PotConverter {
private:
    // Node structure for linked list
    struct Node {
        double value;   // stores one smoothed reading
        Node* next;     // pointer to next node
    };

    Node* head;          // oldest reading
    Node* tail;          // newest reading
    int count;           // number of samples currently stored
    const int MAX_SAMPLES = 10;  // smoothing window size
    double total;         // running sum for average calculation

public:
    const int MAX_ADC_VALUE = 4095;
    const int MIN_ADC_VALUE = 0;

    PotConverter();
    ~PotConverter();

    // Converts an analog ADC reading (0–4095) to a percentage (0–100)
    // TODO: we need to handle errors better here - Look up "returning errors and passing by reference for return values c++ embedded programming"
    double convertPotReading(int reading);

    // Adds a new value and returns the smoothed (average) result
    double smoothReading(double newValue);
};

#endif