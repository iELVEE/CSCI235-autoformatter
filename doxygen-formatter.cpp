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



int main()
{
    int routine_index;
    std::cout << "Would you like to read in a specific file [1] or all files in a folder [2] : ";
    std::cin >> routine_index;
    if (routine_index == 1) {
        std::cout << "worked";
    }
}