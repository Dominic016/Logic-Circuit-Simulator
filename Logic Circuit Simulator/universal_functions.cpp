// universal_functions.cpp (last modified: 03/07/22)
// definition of functions declared in universal_functions.h

#include <vector>
#include <iostream>
#include <cmath>
#include <string>
#include "universal_functions.h"


// creates all truth table input value combinations for n inputs
// (eg. 00, 01, 10, 11 for n = 2)
// uses a vector for each input, which are all stored in one vector of vectors.
std::vector<std::vector<bool>> truth_table_inputs_generator(const int& number_of_inputs)
{
    // manual calculation of 2^n because cmath pow() does not output integers
    int number_of_input_combinations{ 1 };
    for (int i{}; i < number_of_inputs; i++) {
        number_of_input_combinations *= 2;
    }

    std::vector<std::vector<bool>> inputs(number_of_inputs, std::vector<bool>(number_of_input_combinations));
    for (int i{}; i < number_of_inputs; i++) {
        bool set_input_value{ true };
        int l{};

        for (int j{}; j < pow(2, i + 1); j++) {
            set_input_value = set_input_value ? false : true;

            for (int k{}; k < pow(2, number_of_inputs - (i + 1)); k++) {
                inputs[i][l + k] = set_input_value;
                if (k == pow(2, number_of_inputs - (i + 1)) - 1) {
                    l = (k + 1) * (j + 1);
                }
            }
        }
    }
    return inputs;
}


// prints columns for each input and output elements of the circuit
void print_truth_table(const std::vector< std::vector<bool>>& inputs, const std::vector<std::vector<bool>>& outputs)
{
    using namespace std;
    for (int i{}; i < inputs.size(); i++) {
        cout << "Input " << i + 1 << "|";
    }

    for (int i{}; i < outputs.size(); i++) {
        cout << "Output " << i + 1 << "|";
    }
    cout << "\n";

    for (int i{}; i < inputs.size(); i++) {
        cout << "-------|";
    }

    for (int i{}; i < outputs.size(); i++) {
        cout << "--------|";
    }
    cout << "\n";

    for (int i{}; i < inputs[0].size(); i++) {
        for (int j{}; j < inputs.size(); j++) {
            cout << "   " << inputs[j][i] << "   |";
        }
        for (int j{}; j < outputs.size(); j++) {
            cout << "    " << outputs[j][i] << "   |";
        }
        cout << "\n";
    }
}


// elements in the circuit are named for the user with letter identifiers
// creates a letter representation for a given digit (0,1,2,...,25,26,27,... --> a,b,c,...,z,aa,ab,...)
std::string get_element_letter(const int& element_position)
{
    std::string name{};
    if (element_position < 26) {
        name += alphabet.at(element_position);
    } 
    else {
        name += get_element_letter(element_position / 26 - 1);
        name += get_element_letter(element_position % 26);
    }
    return name;
}


// gets element type (input, unary or binary) for various formats of gate_type
std::string get_element_type(const std::string& gate_type)
{
    std::vector<std::string> input_element_names{ "input","Input" };
    std::vector<std::string> unary_element_names{ "not", "NOT", "buffer", "BUFFER" };
    std::vector<std::string> binary_element_names{
        "and","AND","or","OR","nand","NAND","nor","NOR","xor","XOR","xnor","XNOR" };

    auto check_names = [&gate_type](std::vector<std::string> names)
    {
        for (const std::string& name : names) {
            if (name == gate_type) {
                return true;
            }
        }
        return false;
    };

    try {
        if (check_names(input_element_names)) {
            return "input";
        } else if (check_names(unary_element_names)) {
            return "unary";
        } else if (check_names(binary_element_names)) {
            return "binary";
        } else {
            throw - 1;
        }
    }
    catch (int error) {
        if (error == -1) {
            std::cerr << "\nError: element type does not exist\n";
            exit(error);
        }
    }
    // try-catch statement means program will exit if gate_type does not exist,
    // instead of returning it
    return gate_type;
}


// handles all available logic operations, specified by gate_type
bool logic_operation(const std::string gate_type, const std::vector<bool>& input_values)
{
    using namespace std;
    try {
        if (gate_type == "NOT") {
            return input_values[0] ? false : true;
        } 
        else if (gate_type == "BUFFER") {
            return input_values[0] ? true : false;
        } 
        else if (gate_type == "AND") {
            return (input_values[0] && input_values[1]) ? true : false;
        } 
        else if (gate_type == "OR") {
            return (input_values[0] || input_values[1]) ? true : false;
        } 
        else if (gate_type == "NAND") {
            return (input_values[0] && input_values[1]) ? false : true;
        } 
        else if (gate_type == "NOR") {
            return (input_values[0] || input_values[1]) ? false : true;
        } 
        else if (gate_type == "XOR") {
            return ((input_values[0] || input_values[1]) && (input_values[0] != input_values[1])) ? true : false;
        } 
        else if (gate_type == "XNOR") {
            return (input_values[0] == input_values[1]) ? true : false;
        } 
        else {
            throw - 1;
        }
    }

    catch (int error) {
        if (error == -1) {
            std::cerr << "\nError: element type does not exist\n";
            exit(error);
        }
    }
    return 0;
// try-catch statement means program will exit if gate_type does not exist,
// instead of returning 0 for the logic operation
}