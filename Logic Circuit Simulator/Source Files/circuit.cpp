// circuit.cpp (last modified: 03/07/22)
// Contains definition of all circuit class members not defined in circuit.h

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <memory>
#include <sstream>
#include "circuit.h"
#include "elements.h"
#include "universal_functions.h"


circuit::circuit() : circuit_elements{}, input_positions{}, number_of_inputs{}, number_of_elements{} {}

// add_element overloaded for different element types
void circuit::add_element(const bool& input_value)
{
    circuit_elements.push_back(std::make_shared<input_element>(input_value));

    input_positions.push_back(get_circuit_size());
    number_of_inputs++;
    number_of_elements++;
}

void circuit::add_element(const std::string gate_type, const int& input_position)
{
    circuit_elements.push_back(std::make_shared<unary_gate_element>
        (gate_type, circuit_elements[input_position]));

    circuit_elements[input_position]->update_output_status();
    number_of_elements++;
}

void circuit::add_element(const std::string gate_type,
    const int& input1_position, const int& input2_position)
{
    circuit_elements.push_back(std::make_shared<binary_gate_element>
        (gate_type, circuit_elements[input1_position], circuit_elements[input2_position]));

    circuit_elements[input1_position]->update_output_status();
    circuit_elements[input2_position]->update_output_status();
    number_of_elements++;
}


int circuit::get_circuit_size() const
{
    return number_of_elements;
}

bool circuit::get_element_output(const int& element_position) const
{
    return circuit_elements[element_position]->get_output_value();
}

std::vector<int> circuit::get_input_positions() const
{
    return input_positions;
}

std::vector<bool> circuit::get_current_input_values() const
{
    std::vector<bool> current_input_values;

    for (int i{}; i < number_of_inputs; i++) {
        current_input_values.push_back(circuit_elements[input_positions[i]]->get_output_value());
    }
    return current_input_values;
}


// flips value of chosen input then updates the whole circuit
void circuit::change_input(const int& input_position)
{
    circuit_elements[input_position]->update_output();
    update_circuit(input_position);
}


// updates all elements after the given input element
// checks for input_elements so these don't get updated
void circuit::update_circuit(const int& input_position)
{
    int current_input_position = input_position;
    int input_number{};

    for (int i{}; i < number_of_inputs; i++) {
        if (input_position == input_positions[i]) {
            input_number = i;
        }
    }

    for (int i{ input_position }; i < circuit_elements.size(); i++) {
        if (i != current_input_position) {
            circuit_elements[i]->update_output();
        }
        else if (input_number + 1 != number_of_inputs) {
            current_input_position = input_positions[++input_number];
        }
    }
}


void circuit::reset_circuit()
{
    if (get_circuit_size() != 0) {
        circuit_elements[0]->reset_element_count();
    }
    circuit_elements.clear();
    input_positions.clear();
    number_of_inputs = 0;
    number_of_elements = 0;
}


void circuit::restore_input_values(const std::vector<bool>& input_values)
{
    for (int i{}; i < number_of_inputs; i++) {
        if (input_values[i] != circuit_elements[input_positions[i]]->get_output_value()) {
            change_input(i);
        }
    }
}


void circuit::print_circuit_output() const
{
    for (int i{}; i < get_circuit_size(); i++) {
        if (circuit_elements[i]->get_output_status()) {
            std::cout << "Output " << get_element_letter(i) << " is "
                << circuit_elements[i]->get_output_value() << "\n";
        }
    }
    std::cout << "\n";
}


// prints for all outputs or one specific output depending on value of ignore_out_position
void circuit::print_input_output_letters(const bool& ignore_output_position,
    const int& output_position)
{
    for (const int& input : input_positions) {
        std::cout << "   " << get_element_letter(input) << "   |";
    }

    if (ignore_output_position) {
        for (const auto& element : circuit_elements) {
            if (element->get_output_status()) {
                std::cout << "    " << get_element_letter(element->get_element_position())
                    << "   |";
            }
        }
    }
    else {
        std::cout << "    " << get_element_letter(output_position)
            << "   |";
    }
    std::cout << "\n";
}


// prints truth table for a given element, its gate type and its logic formula
// for truth table: stores current input values,
// tries all input combinations and obtains the output values, then restores original input values
void circuit::element_truth_table(const int& element_position)
{
    std::cout << "Gate '" << get_element_letter(element_position)
        << "' type is " << circuit_elements[element_position]->get_gate_type()
        << " gate.\nIt's logic formula is: "
        << generate_logic_formula(circuit_elements[element_position])
        << "\nTruth table:\n\n";

    std::vector<bool> stored_input_values{ get_current_input_values() };
    std::vector<std::vector<bool>> inputs{ truth_table_inputs_generator(number_of_inputs) };
    std::vector<std::vector<bool>> outputs(1);

    for (int i{}; i < inputs[0].size(); i++) {
        for (int j{}; j < number_of_inputs; j++) {
            if (circuit_elements[input_positions[j]]->get_output_value() != inputs[j][i]) {
                circuit_elements[input_positions[j]]->update_output();
            }
        }
        update_circuit(0);
        outputs[0].push_back(circuit_elements[element_position]->get_output_value());
    }

    restore_input_values(stored_input_values);
    print_input_output_letters(false, element_position);
    print_truth_table(inputs, outputs);
}


// stores current input values, gets circuit outputs for all input combinations,
// then restores input values
void circuit::circuit_truth_table()
{
    circuit_formula();
    std::cout << "Truth table for all inputs and outputs:\n\n";

    std::vector<bool> stored_input_values{ get_current_input_values() };
    std::vector<std::vector<bool>> inputs{ truth_table_inputs_generator(number_of_inputs) };
    std::vector<int> output_positions;

    for (auto it = circuit_elements.begin(); it != circuit_elements.end(); it++) {
        if ((*it)->get_output_status()) {
            output_positions.push_back((*it)->get_element_position());
        }
    }
    size_t number_of_outputs = output_positions.size();
    std::vector<std::vector<bool>> outputs(number_of_outputs);

    for (int i{}; i < inputs[0].size(); i++) {
        for (int j{}; j < number_of_inputs; j++) {
            if (circuit_elements[input_positions[j]]->get_output_value() != inputs[j][i]) {
                circuit_elements[input_positions[j]]->update_output();
            }
        }
        update_circuit(0);
        for (int j{}; j < number_of_outputs; j++) {
            outputs[j].push_back(circuit_elements[output_positions[j]]->get_output_value());
        }
    }

    restore_input_values(stored_input_values);
    print_input_output_letters(true, 0);
    print_truth_table(inputs, outputs);
}


// prints formula for all outputs
void circuit::circuit_formula() const
{
    std::string output_logic_formula{};
    for (const auto& element : circuit_elements) {
        if (element->get_output_status()) {
            std::cout << "Output " << get_element_letter(element->get_element_position())
                << " logic formula: ";
            std::cout << generate_logic_formula(element);
            std::cout << "\n";
        }
    }
    std::cout << "\n";
}


// creates a formula for the argument element by recursively creating formulas for its input elements
std::string circuit::generate_logic_formula(const std::shared_ptr<circuit_element>& element) const
{
    std::stringstream current_logic_formula;
    std::string logic_formula;
    std::string element_type{ get_element_type(element->get_gate_type()) };

    if (element_type == "input") {
        logic_formula = get_element_letter(element->get_element_position());
    } 
    else if (element_type == "unary") {
        current_logic_formula << "("
            << element->get_gate_type() << " ";

        int input_element_position{ (element->get_input_elements_positions())[0] };
        current_logic_formula << generate_logic_formula(circuit_elements[input_element_position])
            << ")";

        logic_formula = current_logic_formula.str();
    } 
    else if (element_type == "binary") {
        current_logic_formula << "(";

        int input1_element_position{ (element->get_input_elements_positions())[0] };
        current_logic_formula << generate_logic_formula(circuit_elements[input1_element_position])
            << " " << element->get_gate_type() << " ";

        int input2_element_position{ (element->get_input_elements_positions())[1] };
        current_logic_formula << generate_logic_formula(circuit_elements[input2_element_position])
            << ")";

        logic_formula = current_logic_formula.str();
    }
    return logic_formula;
}
