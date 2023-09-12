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
#include <filesystem>
#include <vector>


/** Format a given line of the string to get rid of white spaces in comment blocks and leave the rest as is.
* This function has 2 static variables to keep track of whether or not we are currently in a comment block or not and to know
* whether or not the comment block has just ended. This is to know whether we should get rid of white spaces or leave them as
* is. If we are in a comment block, we look for if the line is just white space and then remove it if it has no meaningful content.
* If we are not in a comment block, we check for if the comment block has just ended, to get rid of the following empty line before
* the function name. Lastly, we leave the line as is if neither conditions are true. The last bit of this function simply sets our
* static flags true or false depending on if we find a "/*" or "*./" beginning/ending the comment block.
* @param    : reference to a string, line of the input file to format
* @post     : a modified string that is empty or contains the given line with an added new line for readability
*/
void formatLine(std::string& s)
{
    static bool in_comment_block = false;
    static bool comment_block_just_ended = false;

    if (in_comment_block)
    {
        if (s.find_first_not_of(' ') == std::string::npos)
        {
            s = "";
        }
        else
        {
            s += "\n";
        }
    }
    else if (comment_block_just_ended)
    {
        s = "";
        comment_block_just_ended = false;
    }
    else
    {
        s += "\n";
    }

    if (s.find("/**") != std::string::npos)
    {
        in_comment_block = true;
    }
    else if (s.find("*/") != std::string::npos)
    {
        in_comment_block = false;
        comment_block_just_ended = true;
    }
}

/** Take an input text file and format it to a doxygen style file.
* @pre      : text file to read
* @post     : specified file in this style of commented documentation is created
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
            formatLine(line);
            formatted << line;
        }
        
    }
    project_specs.close();
    formatted.close();
}

void selectionMenu(std::string selected_path)
{
    std::vector<std::string> files;
    int i = 0;
    for (const auto& f : std::filesystem::directory_iterator(selected_path))
    {
        if (f.is_regular_file())
        {
            files.push_back(f.path().string());
            std::cout << "[" << i << "] " << f.path().string() << std::endl;
            i++;
        }
    }
}

int main()
{
    int routine_index;
    std::cout << "Would you like to read in a specific file [1] or all files in a folder [2] : ";
    std::cin >> routine_index;
    if (routine_index == 1) {
        //routineOne();
        selectionMenu("./");
    }
}