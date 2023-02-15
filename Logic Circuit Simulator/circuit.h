// circuit.h (last modified: 03/07/22)
// header file for circuit class definition and class member declarations
// also contains definition of the destructor

#ifndef CIRCUIT_H
#define CIRCUIT_H

#include <memory>
#include <vector>
#include <unordered_map>
#include "elements.h"


class circuit
{
private:
    std::vector<std::shared_ptr<circuit_element>> circuit_elements;
    std::vector<int> input_positions;
    int number_of_elements;
    int number_of_inputs;

public:
    circuit();
    ~circuit() {};

    void add_element(const bool&);
    void add_element(const std::string, const int&);
    void add_element(const std::string, const int&, const int&);

    int get_circuit_size() const;
    bool get_element_output(const int&) const;
    std::vector<int> get_input_positions() const;
    std::vector<bool> get_current_input_values() const;

    void change_input(const int&);
    void update_circuit(const int&);
    void reset_circuit();

    void restore_input_values(const std::vector<bool>&);
    void print_circuit_output() const;
    void print_input_output_letters(const bool&, const int&);
    void element_truth_table(const int&);
    void circuit_truth_table();
    void circuit_formula() const;
    std::string generate_logic_formula(const std::shared_ptr<circuit_element>&) const;
};

#endif
