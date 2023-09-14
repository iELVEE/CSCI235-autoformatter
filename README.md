# CSCI235-autoformatter

### If you decide to make hpp AND cpp files of the same name, turn hpp_to_cpp to false, so that it does not change your cpp file's contents to the formatted version of the hpp file
	Meant to be used when copying and pasting project specifications from the CSCI 235 page.

To modify your preferences of the autoformatter, simply edit the following code as needed and then compile:

```
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
// Use the specified doxygen comment style, AS_IS weill leave the comments as is with no modification.

bool hpp_to_cpp_ = true;
// If there are hpp files, copy the contents into a corresponding cpp file of the same name, even if the cpp file does not exist.

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
```

Documentation style is completely dynamic. Simply add your style to the corresponding maps and the autoformatter will format in that style. Doubling up (formatting the same file twice) will yield undesired results. 

GIFs showing the use cases of the autoformatter will be added in the near future.
