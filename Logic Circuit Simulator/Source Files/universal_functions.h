// universal_functions.h (last modified: 02/07/22)
// header file for declaration of functions and a constant needed in several parts of the program

#ifndef UNIVERSAL_FUNCTIONS_H
#define UNIVERSAL_FUNCTIONS_H

#include <vector>
#include <string>


// definition of constants
const std::string alphabet{ "abcdefghijklmnopqrstuwvxyz" };


// declaration of functions
std::vector<std::vector<bool>> truth_table_inputs_generator(const int& number_of_inputs);

void print_truth_table(const std::vector< std::vector<bool>>&, const std::vector<std::vector<bool>>&);

std::string get_element_letter(const int&);

std::string get_element_type(const std::string&);

bool logic_operation(const std::string gate_type, const std::vector<bool>& input_values);

#endif