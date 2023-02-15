// main.cpp
// OOP in c++ project: Logic Circuits
// Dominic Bradley (last modified: 03/07/22)
// Allows user to create, modify, and view information about digital circuits,
// via a type-based menu interface.
// main.cpp handles the interface, and contains functions used only for the interface

#include <string>
#include <memory>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <sstream>
#include <type_traits>
#include "universal_functions.h"
#include "elements.h"
#include "circuit.h"


// declaring functions used in the interface
template <class class_type> class_type get_user_option(const std::vector<class_type>& options);
template <class class_type> void print_options(const std::vector<class_type>& options);
void create_gate(std::unordered_map<std::string, std::string>& gate_library,
    const std::string& gate_type);
std::vector<std::string> get_all_element_letters(const int& circuit_size);
int get_element_position(const std::string& element_letter);
void print_gate_truth_table(const std::string& gate_type);


int main()
{

    // pre-interface setup
    bool end_program_condition{ false };
    circuit user_circuit;
    static bool does_circuit_exist{ false };
    std::unordered_map<std::string, std::string> gate_library;

    std::vector<std::string> available_logic_gate_options{
        "all","not","buffer","and","or","nand","nor","xor","xnor" };
    const std::vector<std::string> yes_no_options{ "y","n"};

    std::cout << "Welcome to Logic Circuit Simulator!\n";


    // interface loops until end_program_condition is updated
    while (true) {

        // main menu list of user options
        std::cout << "------------------------------------------------------------------------------\n"
            << "Please type an option and press enter.\n\n"
            << "(1)--Create new circuit\n"
            << "(2)--Create logic gates\n"
            << "(3)--Add inputs/gates to circuit\n"
            << "(4)--Print logic formula/e and current input/output values\n"
            << "(5)--Print full circuit truth table\n"
            << "(6)--Print logic formula and truth table for a particular gate in the circuit\n"
            << "(7)--Print truth table for a logic gate in the library\n"
            << "(8)--Change value of an input\n"
            << "(9)--Exit program\n"
            << "(0)--help";
        std::vector<int> main_menu_options{ 1,2,3,4,5,6,7,8,9,0 };

        
        // switch statement handles all user interaction
        // case numbers correspond to main menu options listed above
        switch (get_user_option(main_menu_options)) {

            case 1: // resets circuit if the user has used this option before
                
                if (does_circuit_exist) {
                    std::cout << "Are you sure?"
                        << " This will delete your current circuit and create a new one.";
                    std::string option{ get_user_option(yes_no_options) };

                    if (option == "n") {
                        break;
                    } 
                    else {
                        gate_library.clear();
                        std::cout << "Gate library cleared!\n\n";
                        available_logic_gate_options = {
                            "all","not","buffer","and","or","nand","nor","xor","xnor" };
                        user_circuit.reset_circuit();
                        std::cout << "\nNew circuit created! Now create some logic gates.\n\n";
                    }
                } 
                else {
                    std::cout << "\nNew circuit created! Now create some logic gates.\n\n";
                    does_circuit_exist = true;
                }
                break;

            
            case 2: { // adds chosen gates to gate_library

                using namespace std;
                
                if (!does_circuit_exist) {
                    cout << "Please create a circuit first!\n\n";
                    break;
                }

                if (gate_library.size() == 8) {
                    cout << "Gate library already contains all gate types!\n\n";
                    break;
                }

                string new_gate_option{ "y" };
                while (new_gate_option != "n") {
                    cout << "What logic gate would you like to add?\n"
                        << "Type 'all' to add all gates to the library.";

                    string gate_type_option{ get_user_option(available_logic_gate_options) };

                    for (auto it = available_logic_gate_options.begin();
                            it != available_logic_gate_options.end(); it++) {
                        if (gate_type_option == *it) {
                            it = available_logic_gate_options.erase(it);
                            break;
                        }
                    }

                    if (gate_type_option == "all") {
                        for (const string& option : available_logic_gate_options) {
                            create_gate(gate_library, option);
                            cout << "\n" << gate_library[option]
                                << " gate added to the gate library.";
                        }
                    }
                    
                    else {
                        create_gate(gate_library, gate_type_option);
                        cout << "\n" << gate_library[gate_type_option]
                            << " gate added to the gate library.\n";
                    }

                    if (gate_library.size() == 8) {
                        cout << "\n\nGate library now contains all gate types!\n\n";
                        break;
                    }

                    cout << "Would you like to add another gate?";
                    new_gate_option = get_user_option(yes_no_options);
                }
                break;
            }


            case 3: { // adds chosen elements to user_circuit
                
                using namespace std;

                if (gate_library.size() == 0) {
                    cout << "Please create some gates first!\n\n";
                    break;
                }

                string new_element_option{ "y" };
                while (new_element_option != "n") {
                    cout << "You can add inputs and gates [hint: add an input first]\n"
                        << "Each input/gate will be given a reference letter (a,b,c,d,e etc)"
                        << " and can output to many other components.\n"
                        << "What type of gate would you like to add to the circuit?";

                    vector<string> element_options;
                    element_options.push_back("input");

                    for (auto it = gate_library.begin(); it != gate_library.end(); it++) {
                        element_options.push_back(it->first);
                    }

                    string gate_type_option{ get_user_option(element_options) };
                    string element_type{ get_element_type(gate_type_option) };

                    // gets the letter of the element the user wants to use as an input
                    auto get_input_element = [&user_circuit](string gate_type) {
                        cout << "\nWhich element would you like to input to the "
                            << gate_type << " gate?";
                        vector<string> options{ get_all_element_letters((user_circuit.get_circuit_size())) };
                        string option = get_user_option(options);
                        return option;
                    };

                    if (element_type == "input") {
                        cout << "Set a value for the input.";
                        vector<std::string> input_value_options{ "0","1" };
                        std::string input_value_option{ get_user_option(input_value_options) };

                        bool input_value{ [&input_value_option]() {
                            return input_value_option == "1" ? true : false;
                        }() };

                        user_circuit.add_element(input_value);
                        cout << "Input added with input value " << input_value
                            << ". Refer to this as '"
                            << get_element_letter(user_circuit.get_circuit_size() - 1)
                            << "'.";
                    }
                    
                    else if (user_circuit.get_circuit_size() == 0) {
                        cout << "Add some inputs to the circuit first!";
                    }
                    
                    else if (element_type == "unary") {

                        cout << "\nThis gate can have 1 input";
                        int element_input = get_element_position(get_input_element(gate_library[gate_type_option]));

                        user_circuit.add_element(gate_library[gate_type_option], element_input);

                        cout << gate_library[gate_type_option]
                            << " gate added with an input from gate '"
                            << get_element_letter(element_input)
                            << "'. Refer to this as '"
                            << get_element_letter(user_circuit.get_circuit_size() - 1)
                            << "'.";
                    }
                    
                    else if (element_type == "binary") {

                        cout << "\nThis gate can have 2 inputs.\nFor the 1st input:";
                        int element_input1 = get_element_position(get_input_element(gate_library[gate_type_option]));
                        cout << "For the 2nd input:";
                        int element_input2 = get_element_position(get_input_element(gate_library[gate_type_option]));

                        user_circuit.add_element(gate_library[gate_type_option],
                            element_input1, element_input2);

                        cout << gate_library[gate_type_option]
                            << " gate added with inputs from gates '"
                            << get_element_letter(element_input1)
                            << "' and '"
                            << get_element_letter(element_input2)
                            << "'. Refer to this as '"
                            << get_element_letter(user_circuit.get_circuit_size() - 1)
                            << "'.";
                    }

                    cout << "\nWould you like to add another gate?";
                    new_element_option = get_user_option(yes_no_options);
                }
                break;
            }


            case 4: { // prints the value of every input and output and the logic formula of every output
                
                if (user_circuit.get_circuit_size() == 0) {
                    std::cout << "Please add some gates to a circuit first!\n\n";
                    break;
                }

                std::vector<int> input_positions = user_circuit.get_input_positions();
                for (int i{}; i < input_positions.size(); i++) {
                    std::cout << "Input " << get_element_letter(input_positions[i])
                        << " is " << user_circuit.get_element_output(input_positions[i]) << "\n";
                }
                std::cout << "\n";

                user_circuit.print_circuit_output();
                user_circuit.circuit_formula();

                break;
            }


            case 5: // prints truth table for whole circuit and the logic formula for all outputs

                if (user_circuit.get_circuit_size() == 0) {
                    std::cout << "Please add some gates to a circuit first!\n\n";
                    break;
                }
                std::cout << "\n";

                user_circuit.circuit_truth_table();

                std::cout << "\n";
                break;


            case 6: { // prints the truth table for a chosen circuit element
                      // also prints the element's logic formula and its gate type
                
                using namespace std;

                if (user_circuit.get_circuit_size() == 0) {
                    cout << "Please add some gates to a circuit first!\n\n";
                    break;
                }

                std::string print_truth_table_option{ "y" };

                while (print_truth_table_option != "n") {
                    cout << "Which gate in the circuit would you like to see a truth table for?";

                    vector<string> element_options{ get_all_element_letters(
                        (user_circuit.get_circuit_size())) };
                    string user_element_option{ get_user_option(element_options) };
                    cout << "\n";

                    user_circuit.element_truth_table(get_element_position(user_element_option));

                    cout << "\n\nWould you like to view the truth table for"
                        << " another gate in the circuit? ";
                    print_truth_table_option = get_user_option(yes_no_options);
                }
                break;
            }


            case 7: { // prints truth table of user-chosen gate from the gate library
                
                using namespace std;

                if (gate_library.size() == 0) {
                    cout << "Please create some gates first!\n\n";
                    break;
                }

                string print_truth_table_option{ "y" };

                while (print_truth_table_option != "n") {
                    cout << "Which gate would you like to see a truth table for?";
                    vector<string> gate_options{};

                    for (auto it = gate_library.begin(); it != gate_library.end(); it++) {
                        gate_options.push_back(it->first);
                    }
                    string user_gate_option{ get_user_option(gate_options) };
                    cout << "\n";

                    print_gate_truth_table(gate_library[user_gate_option]);

                    cout << "\n\nWould you like to view the truth table for"
                        << " another gate type in the library? ";
                    print_truth_table_option = get_user_option(yes_no_options);
                }
                break;
            }


            case 8: { // swaps the input value of user chosen input/s

                using namespace std;

                if (user_circuit.get_circuit_size() == 0) {
                    cout << "Please add some inputs to a circuit first!\n\n";
                    break;
                }

                string change_input_option{ "y" };

                while (change_input_option != "n") {
                    cout << "Which input would you like to change?";
                    vector<string> input_options{};
                    vector<int> input_positions{ user_circuit.get_input_positions() };

                    for (const int& position : input_positions) {
                        input_options.push_back(get_element_letter(position));
                    }
                    string user_input_option{ get_user_option(input_options) };
                    int input_position = get_element_position(user_input_option);
                    bool previous_value{ user_circuit.get_element_output(input_position) };

                    user_circuit.change_input(input_position); // also sequentially updates the rest of the circuit

                    cout << "input '" << user_input_option
                        << "' swapped from " << previous_value << " to "
                        << user_circuit.get_element_output(input_position)

                        << "\n\nWould you like to change another input?";
                    change_input_option = get_user_option(yes_no_options);
                }
                break;
            }


            case 9: { // option for exiting the program

                std::cout << "Are you sure you want to exit?";
                std::string option{ get_user_option(yes_no_options) };

                if (option == "y") {
                    end_program_condition = true;
                }
                break;
            }


            case 0: //  provides additional detail on using the program

                std::cout << "\n-To get started, create a circuit option 1, then create some gates with option 2.\n\n"
                    << "-You can then view the behaviour of these logic gates with option 7.\n\n"
                    << "-Next add some gates to the circuit with option 3.\n\n"
                    << "-You will need to have at least one input and select it's input value (1 or 0).\n\n"
                    << "-For any gates you make, you will need to choose its inputs.\n\n"
                    << "-Some gates require a single input and some require two\n\n"
                    << "-You then have several options to view circuit information (input/output values, truth tables, logic formulae).\n\n"
                    << "-You can also swap the value of an input from 1 to 0 or vice versa.\n\n"
                    << "-When you are finised, you can create a new circuit with option '1' or exit with option '9'.\n\n";

                break;
        }


        if (end_program_condition == true) {
            std::cout << "ending program...\n\n";

            break;
        }
    }
}



// definition of functions used in the interface
//
// gets an option from a user based on a vector of string or integer user options
template <class class_type> class_type get_user_option(const std::vector<class_type>& options)
{
    using namespace std;

    while (true) {
        cout << "\n\n";
        print_options(options);
        class_type user_option{};
        cin.clear();
        string user_input{};
        getline(cin, user_input);
        cout << "------------------------------------------------------------------------------\n";

        int invalid_input{ -1 };
        try {
            if (user_input.find(" ") != string::npos) {
                throw invalid_input;
            } 
            else if (is_integral<class_type>::value) {
                if (user_input.size() != 1 || user_input.find_first_of("0123456789") == string::npos) {
                    throw invalid_input;
                }
            } 
            else if (user_input.size() == 1) {
                if (user_input != "0" && user_input != "1" &&
                    user_input.find_first_of(alphabet) == string::npos) {
                    throw invalid_input;
                }
            }

            stringstream get_option_stream;
            get_option_stream.str(user_input);
            get_option_stream >> user_option;

            for (const class_type& option : options) {
                if (user_option == option) {
                    return user_option;
                }
            }
            throw invalid_input;
        }

        catch (int error) {
            if (error == invalid_input) {
                cout << "\n'" << user_input << "' is not an option.\n"
                    << "Please enter one of the following.";
            }
        }
    }
}


// lists all options the user can choose from
template <class class_type> void print_options(const std::vector<class_type>& options)
{
    std::cout << "Options:\n";

    for (const class_type& option : options) {
        if (option != options.back()) {
            std::cout << option << ", ";
        } else {
            std::cout << option;
        }
    }
    std::cout << "\n\n";
}


// adds a given gate_type to the gate_library, in upper-case letters
void create_gate(std::unordered_map<std::string, std::string>& gate_library,
    const std::string& gate_type)
{
    using namespace std;

    if (gate_type == "not") {
        gate_library[gate_type] = "NOT";
    } 
    else if (gate_type == "buffer") {
        gate_library[gate_type] = "BUFFER";
    } 
    else if (gate_type == "and") {
        gate_library[gate_type] = "AND";
    } 
    else if (gate_type == "or") {
        gate_library[gate_type] = "OR";
    } 
    else if (gate_type == "nand") {
        gate_library[gate_type] = "NAND";
    } 
    else if (gate_type == "nor") {
        gate_library[gate_type] = "NOR";
    } 
    else if (gate_type == "xor") {
        gate_library[gate_type] = "XOR";
    } 
    else if (gate_type == "xnor") {
        gate_library[gate_type] = "XNOR";
    }
}


// creates a vector of letter-values for all circuit elements
// used for listing element options to the user
std::vector<std::string> get_all_element_letters(const int& circuit_size)
{
    std::vector<std::string> letters;

    for (int i{}; i < circuit_size; i++) {
        letters.push_back(get_element_letter(i));
    }
    return letters;
}


// converts element letter into its position in the circuit
// (a,b,c --> 0,1,2 and so on)
int get_element_position(const std::string& element_letter)
{
    int element_position{};
    int value_of_letter{ 1 };

    for (auto it = element_letter.rbegin(); it != element_letter.rend(); it++) {
        element_position += value_of_letter * static_cast<int>(alphabet.find(*it));
        value_of_letter += 1;
    }
    return element_position;
}


// prints the truth table for a given gate type
void print_gate_truth_table(const std::string& gate_type)
{
    std::string element_type = get_element_type(gate_type);

    if (element_type == "unary") {

        std::cout << gate_type << " gate truth table:\n\n";
        std::vector<std::vector<bool>> inputs = truth_table_inputs_generator(1);
        std::vector<std::vector<bool>> outputs(1);

        for (int i{}; i < inputs[0].size(); i++) {
            std::vector<bool> input;
            input.push_back(inputs[0][i]);
            outputs[0].push_back(logic_operation(gate_type, input));
        }

        print_truth_table(inputs, outputs);
    } 
    
    else if (element_type == "binary") {

        std::cout << gate_type << " gate truth table:\n\n";
        std::vector<std::vector<bool>> inputs = truth_table_inputs_generator(2);
        std::vector<std::vector<bool>> outputs(1);

        for (int i{}; i < inputs[0].size(); i++) {
            std::vector<bool> input;
            input.push_back(inputs[0][i]);
            input.push_back(inputs[1][i]);
            outputs[0].push_back(logic_operation(gate_type, input));
        }

        print_truth_table(inputs, outputs);
    }
}