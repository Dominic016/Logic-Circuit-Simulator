// elements.cpp (last modified: 03/07/22)
// Contains definition of all members of element classes not defined in elements.h

#include <vector>
#include <string>
#include <memory>
#include "elements.h"
#include "universal_functions.h"


// base class for all elements
//
// output_value defaults to true, but is updated by the derived element constructors
circuit_element::circuit_element() :
    element_position{ number_of_elements }, output_value{ true },
    is_output_of_circuit{ true }, gate_type{}
{
    number_of_elements++;
};

circuit_element::~circuit_element()
{
    std::cout << "Destructing " << "element '" << get_element_letter(element_position) << "'\n";
};

bool circuit_element::get_output_value() const
{
    return output_value;
}

int circuit_element::get_element_position() const
{
    return element_position;
}

bool circuit_element::get_output_status() const
{
    return is_output_of_circuit;
}

std::string circuit_element::get_gate_type() const
{
    return gate_type;
}

void circuit_element::update_output_status()
{
    is_output_of_circuit = false;
}

void circuit_element::reset_element_count()
{
    number_of_elements = 0;
}
int circuit_element::number_of_elements{};



// derived classes
// 
// circuit input class

input_element::input_element(const bool& input_value) : circuit_element{}
{
    gate_type = "Input";
    output_value = input_value;
}

void input_element::update_output() // flips output
{
    output_value = output_value ? false : true;
}

// input_elements do not have any other elements as inputs, so its own position is returned.
// in the program, this function is only actually called for unary_ or binary_gate_elements
std::vector<int> input_element::get_input_elements_positions() const
{
    return std::vector<int>{get_element_position()};
}



// class for gates with a single input
//
unary_gate_element::unary_gate_element(const std::string new_gate_type,
    const std::shared_ptr<circuit_element>& set_input_element) :
    circuit_element{}, input_element{ set_input_element }
{
    gate_type = new_gate_type;
    update_output();
}

bool unary_gate_element::output_calculator(const std::vector<bool>& inputs) const
{
    return logic_operation(gate_type, inputs);
}

// calculates output based on output_value of its input element
void unary_gate_element::update_output()
{
    std::vector<bool> inputs;
    inputs.push_back(input_element->get_output_value());
    output_value = output_calculator(inputs);
}

std::vector<int> unary_gate_element::get_input_elements_positions() const
{
    std::vector<int> input_elements_positions;
    input_elements_positions.push_back(input_element->get_element_position());
    return input_elements_positions;
}



// class for gates with two inputs
//
binary_gate_element::binary_gate_element(const std::string set_gate_type,
    const std::shared_ptr<circuit_element>& set_input_element1, const std::shared_ptr<circuit_element>& set_input_element2) :
    circuit_element{},
    input_element1{ set_input_element1 }, input_element2{ set_input_element2 }
{
    gate_type = set_gate_type;
    update_output();
}

bool binary_gate_element::output_calculator(const std::vector<bool>& inputs) const
{
    return logic_operation(gate_type, inputs);
}

// calculates output based on output_value's of its 2 input elements
void binary_gate_element::update_output()
{
    std::vector<bool> inputs;
    inputs.push_back(input_element1->get_output_value());
    inputs.push_back(input_element2->get_output_value());
    output_value = output_calculator(inputs);
}

std::vector<int> binary_gate_element::get_input_elements_positions() const
{
    std::vector<int> input_elements_positions;
    input_elements_positions.push_back(input_element1->get_element_position());
    input_elements_positions.push_back(input_element2->get_element_position());
    return input_elements_positions;
}