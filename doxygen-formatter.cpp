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
#include <cctype>


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

/** Replace the given file with the formatted version of the text inside.
* @param    : string of the input file's name
* @post     : the text inside the input file is changed completely to get rid of comment new lines
*/
void overwriteFile(std::string input_file)
{
    std::string formatted_string = "";

    std::ifstream ifile(input_file);
    if (ifile.is_open())
    {
        std::string line;
        while (ifile.good())
        {
            std::getline(ifile, line);
            formatLine(line);
            formatted_string += line;
        }
    }
    ifile.close();

    std::ofstream ofile(input_file);
    if (ofile.is_open())
    {
        // length-1 because c_str conversion add an extra null character at the end
        ofile.write(formatted_string.c_str(), formatted_string.length()-1);
    }
    ofile.close();
}

/** List all regular files in a given directory.
* @param    : string of directory
* @return   : vector of strings of all files in the directory
*/
std::vector<std::string> selectionMenu(std::string selected_path, bool listMode = false)
{
    std::vector<std::string> files;
    int i = 0;
    for (const auto& f : std::filesystem::directory_iterator(selected_path))
    {
        if (f.is_regular_file())
        {
            files.push_back(f.path().string());
            if (listMode)
            {
                std::cout << "[" << i << "] " << f.path().string() << std::endl;
            }
            i++;
        }
    }
    return files;
}

/** Take a variable number of input files selecting from given files and put them into a vector of selected file names.
 * @param   : vector of strings comprising of all files present in working directory
 * @return  : vector of strings comprising of selected files
*/
std::vector<std::string> select(std::vector<std::string> files)
{
    std::vector<std::string> inputs;
    int file_index;
    std::cout << "Please input the indexes of the file(s) you want to format (multiple files are separated by spaces) : ";
    while (std::cin >> file_index)
    {
        inputs.push_back(files.at(file_index));
        //break the infinite cin loop once enter is pressed, failbit prevents non int inputs
        if (std::iscntrl(std::cin.get()))
        {
            break;
        }
    }

    return inputs;
}

/** Take an input text file and format it to a doxygen style file.
* @pre      : text file to read
* @post     : specified file in this style of commented documentation is created
*/
void routineOne()
{
    std::vector<std::string> files_to_format = select(selectionMenu("./", true));
    for (const auto& s : files_to_format)
    {
        overwriteFile(s);
    }
}


int main()
{
    int routine_index;
    std::cout << "Would you like to read in specific file(s) [1] or all files in a folder [2] : ";
    std::cin >> routine_index;
    if (routine_index == 1) {
        routineOne();
    }
}