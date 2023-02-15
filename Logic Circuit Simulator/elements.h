// elements.h (last modified: 03/07/22)
// header file containing definitions of element classes, and declarations of their members
// also contains definition of virtual member functions and the derived class's destructors

#ifndef ELEMENTS_H
#define ELEMENTS_H

#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include "universal_functions.h"


// base class for unique elements of a circuit
class circuit_element
{
private:
    const int element_position;
    static int number_of_elements;
    bool is_output_of_circuit;

protected:
    std::string gate_type;
    bool output_value;

public:
    circuit_element();
    virtual ~circuit_element();

    virtual void update_output() = 0;
    virtual std::vector<int> get_input_elements_positions() const = 0;

    bool get_output_value() const;
    int get_element_position() const;
    bool get_output_status() const;
    std::string get_gate_type() const;

    void update_output_status();
    void reset_element_count();
};


// derived classes for different types of elements

// class for inputs to the circuit
class input_element : public circuit_element
{
public:
    input_element(const bool& input_value);
    ~input_element() {};

    void update_output();
    std::vector<int> get_input_elements_positions() const;
};


// class for gates with a single input
class unary_gate_element : public circuit_element
{
private:
    const std::shared_ptr<circuit_element> input_element;

public:
    unary_gate_element(const std::string new_gate_type,
        const std::shared_ptr<circuit_element>& set_input_element);
    ~unary_gate_element() {};

    bool output_calculator(const std::vector<bool>& inputs) const;
    void update_output();
    std::vector<int> get_input_elements_positions() const;
};


// class for gates with two inputs
class binary_gate_element : public circuit_element
{
private:
    const std::shared_ptr<circuit_element> input_element1;
    const std::shared_ptr<circuit_element> input_element2;

public:
    binary_gate_element(const std::string set_gate_type,
        const std::shared_ptr<circuit_element>& set_input_element1,
        const std::shared_ptr<circuit_element>& set_input_element2);
    ~binary_gate_element() {};

    bool output_calculator(const std::vector<bool>& inputs) const;
    void update_output();
    std::vector<int> get_input_elements_positions() const;
};


#endif