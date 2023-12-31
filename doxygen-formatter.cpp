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
#include <unordered_map>

enum Doxs { AS_IS, JAVADOC, QT, BANNER };
std::unordered_map<Doxs, std::string> comment_beginner_ = { {AS_IS, "/**"}, {JAVADOC, "/**"},   {QT, "/*!"},    {BANNER,  "/************************************************"} };
std::unordered_map<Doxs, std::string> comment_middle_   = { {AS_IS, ""},   {JAVADOC, "*"},     {QT, "*"},      {BANNER,  " *"} };
std::unordered_map<Doxs, std::string> comment_ender_    = { {AS_IS, "*/"},  {JAVADOC, "*/"},    {QT, "*/"},     {BANNER,  " ***********************************************/"} };

std::vector<std::string> filter_types = { ".cpp", ".hpp", ".txt" };

/***************************************************************************************************************************************
 * SETTINGS
 **************************************************************************************************************************************/

bool filter_ = true;
// Display only filtered file types.

Doxs doxygen_style_ = AS_IS;
// Use the specified doxygen comment style, AS_IS will leave the comments as is with no modification.

bool hpp_to_cpp_ = true;
// If there are hpp files, copy the contents into a corresponding cpp file of the same name, even if the cpp file does not exist.

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/**Check to see if the line has any characters aside from a white space.
 * @param   : string to check through
 * @return  : bool whether the string has something besides white space
*/
bool notWhiteSpace(std::string s)
{
    for (const unsigned char c : s)
    {
        if (!std::isspace(c)){
            return false;
        }
    }
    return true;
}

/** Format a given line of the string to get rid of white spaces in comment blocks and leave the rest as is.
* This function has 2 static variables to keep track of whether or not we are currently in a comment block or not and to know
* whether or not the comment block has just ended. This is to know whether we should get rid of white spaces or leave them as
* is. If we are in a comment block, we look for if the line is just white space and then remove it if it has no meaningful content.
* If we are not in a comment block, we check for if the comment block has just ended, to get rid of the following empty line before
* the function name. Lastly, we leave the line as is if neither conditions are true. The last bit of this function simply sets our
* static flags true or false depending on if we find beginning/ending comment block characters.
* An addition to this algorithm is changing the lines and characters to proper format, the checks are all the same.
* Note      : order of setting the flags matters because if comment_block just ended is set before we check for, then we get rid of
*             the comment block ending characters instead of the following empty line.
* @param    : reference to a string, line of the input file to format
* @post     : a modified string that is empty or contains the given line with an added new line for readability
*/
void formatLine(std::string& s)
{
    static bool in_comment_block = false;
    static bool comment_block_just_ended = false;

    //in_comment_block flag isn't needed for /** since either way, it remains unmodified (unless changing to a different style)
    if (in_comment_block)
    {
        if (notWhiteSpace(s))
        {
            s = "";
        }
        else
        {
            if (s.find("*/") != std::string::npos)
            {
                s = comment_ender_[doxygen_style_];
            }
            else
            {
                s = comment_middle_[doxygen_style_] + s;
            }
            s += "\n";
        }
    }
    //get rid of following empty line
    else if (comment_block_just_ended)
    {
        s = "";
        comment_block_just_ended = false;
    }
    //leave as is (or convert)
    else
    {
        if (s.find("/**") != std::string::npos)
        {
            s = comment_beginner_[doxygen_style_];
        }
        s += "\n";
    }

    //searching for new style's comment style since they have been converted by now
    if (s.find(comment_beginner_[doxygen_style_]) != std::string::npos)
    {
        in_comment_block = true;
    }
    else if (s.find(comment_ender_[doxygen_style_]) != std::string::npos)
    {
        in_comment_block = false;
        comment_block_just_ended = true;
    }
}


/** Take an input file, and change a referenced string to be the formatted content of the file.
* @param    : string of the input file's name
* @param    : reference to the formatted string
* @post     : the formatted string is changed to reflect the input file's content
*/
void turnContentIntoFormattedString(std::string input_file, std::string& formatted_string)
{
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
}

/** Write the formatted string to be the content of an output file.
* @param    : string of the output file's name
* @param    : the text that will replace the output file's content
* @post     : the output file is created or overwritten to be formatted
*/
void writeContentToFile(std::string output_file, std::string formatted_string)
{
    std::ofstream ofile(output_file);
    if (ofile.is_open())
    {
        // length-1 because c_str conversion add an extra null character at the end
        ofile.write(formatted_string.c_str(), formatted_string.length() - 1);
    }
    ofile.close();
}

/** Replace the given file with the formatted version of the text inside.
* @param    : string of the input file's name
* @post     : the text inside the input file is changed completely to get rid of comment new lines
*/
void overwriteFile(std::string input_file)
{
    std::string formatted_string = "";

    turnContentIntoFormattedString(input_file, formatted_string);

    writeContentToFile(input_file, formatted_string);

    if (hpp_to_cpp_)
    {
        if (input_file.find(".hpp") != std::string::npos)
        {
            std::string cpp_correspondent = input_file.substr(0, input_file.find(".hpp")) + ".cpp";
            writeContentToFile(cpp_correspondent, formatted_string);
        }
    }
}


/** Check if the file is included among the filtered filetypes.
* @param    : string of the file's name
* @return   : bool of if the file type passes the filter
*/
bool checkExtension(std::string file_name)
{
    for (const auto& extension : filter_types)
    {
        if (file_name.find(extension) != std::string::npos)
        {
            return true;
        }
    }
    return false;
}

/** Add the file name to the vector of files through a reference and iterate i for printing the list
* @param    : reference to a vector of strings, file names
* @param    : file name to add to the vector
* @param    : reference to i, an index of where the file name lies
* @param    : boolean for if the list of the files should be printed out to cli
* @post     : file name is added to vector, iterator is increased
*/
void addFile(std::vector<std::string>& files, std::string file_name, int& i, bool listMode)
{
    files.push_back(file_name);
    if (listMode)
    {
        std::cout << "[" << i << "] " << file_name << std::endl;
    }
    i++;
}


/** List all regular files in a given directory.
* @param    : string of directory
* @param    : boolean for if the list of the files should be printed out to cli
* @return   : vector of strings of all files in the directory
*/
std::vector<std::string> selectionMenuFiles(std::string selected_path, bool listMode = false)
{
    std::vector<std::string> files;
    int i = 0;
    for (const auto& f : std::filesystem::directory_iterator(selected_path))
    {
        if (f.is_regular_file())
        {
            std::string file_name = f.path().string();
            if (filter_)
            {
                if (checkExtension(file_name))
                {
                    addFile(files, file_name, i, listMode);
                }
            }
            else
            {
                addFile(files, file_name, i, listMode);
            }
        }
    }

    if (listMode)
    {
        if (!files.empty()) {
            std::cout << "[" << i << "] " << "File not listed" << std::endl;
        }
        files.push_back("File not listed");
    }

    return files;
}

/** Get the directories of the selected_path and list them out
 *  @param  : string, path of directory to iterate through list them out
 *  @return : vector of strings of all the directories present in the selected directory
 */
std::vector<std::string> selectionMenuDirectories(std::string selected_path)
{
    std::vector<std::string> directories;
    int i = 0;
    for (const auto& f : std::filesystem::directory_iterator(selected_path))
    {
        if (f.is_directory())
        {
            std::string dir_name = f.path().string();
            //add file works for the given type of object, even if the name is misleading
            addFile(directories, dir_name, i, true);
        }
    }

    if (!directories.empty())
    {
        std::cout << "[" << i << "] " << "Directory not listed" << std::endl;
    }
    directories.push_back("Directory not listed");
    
    return directories;
}

/** Take a variable number of input files selecting from given files and put them into a vector of selected file names.
 * @param   : vector of strings comprising of all files present in working directory
 * @return  : vector of strings comprising of selected files
*/
std::vector<std::string> select(std::vector<std::string> files, std::string target)
{
    std::vector<std::string> inputs;
    int file_index;
    if (files.size() == 1)
    {
        std::cout << "What " << target << " would you like to format : ";
        std::cin >> files.at(0);
        inputs.push_back(files.at(0));
    }
    else
    {
        std::cout << "Please input the indexes of the " << target << "(s) you want to format (multiple objects are separated by spaces) : ";
        while (std::cin >> file_index)
        {
            //file not listed conditional
            if (file_index == (int) files.size() - 1) {
                std::cout << "What " << target << " would you like to format : ";
                std::cin >> files.at(file_index);
            }

            inputs.push_back(files.at(file_index));
            //break the infinite cin loop once enter is pressed, failbit prevents non int inputs
            if (std::iscntrl(std::cin.get()))
            {
                break;
            }
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
    std::vector<std::string> files_to_format = select(selectionMenuFiles("./", true), "file");
    for (const auto& s : files_to_format)
    {
        overwriteFile(s);
    }
}

/** Take all files in a directory and format them.
* @post     : all files in the directory formatted
*/
void routineTwo()
{
    std::cout << "absolute path or relative (e.g. ./sub_directory/target_directory)" << std::endl;

    std::vector<std::string> directories = select(selectionMenuDirectories("./"), "directory");
  
    for (const auto& d : directories)
    {
        std::vector<std::string> directory_files = selectionMenuFiles(d);
        for (const auto& s : directory_files)
        {
            overwriteFile(s);
        }
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
    else if (routine_index == 2) {
        routineTwo();
    }
}