/*
Author		: Alvi Aziz
File		: doxygen-formatter.cpp
    Created		:   MM/DD/YYYY 09/11/2023
    Description :   Take a text file filled with project specifications copied from the appropriate CSCI 235 page
                    and format it into a very simple and light doxygen format

Credits     : N/A
*/

#include <iostream>
#include <fstream>
#include <string>


/** Take an input text file and format it to a doxygen style file
* @pre  : text file to read
* @post : specified file in this style of commented documentation is created
*/
void routineOne()
{
    std::string input_file_name, output_file_name;
    std::cout << "Please input the file you would like to read : ";
    std::cin >> input_file_name;
    std::cout << "Please input the name of the file you would like to create : ";
    std::cin >> output_file_name;

    std::ifstream project_specs(input_file_name);
    std::ofstream formatted(output_file_name);
    if (project_specs.is_open()) 
    {
        std::string line;
        while (project_specs.good())
        {
            std::getline(project_specs, line);
            formatted << line << std::endl;
        }
        
    }
    project_specs.close();
    formatted.close();
}

int main()
{
    int routine_index;
    std::cout << "Would you like to read in a specific file [1] or all files in a folder [2] : ";
    std::cin >> routine_index;
    if (routine_index == 1) {
        routineOne();
    }
}