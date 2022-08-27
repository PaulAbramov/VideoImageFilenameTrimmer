// VideoImageFilenameTrimmer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <filesystem>
#include <locale>

void EraseSubStr(std::string& _mainString, std::string& _toErase)
{
    std::locale::global(std::locale("de_DE.utf8"));
    std::wcout.imbue(std::locale());

    auto& f = std::use_facet<std::ctype<char>>(std::locale());
    f.tolower(&_mainString[0], &_mainString[0] + _mainString.size());
    f.tolower(&_toErase[0], &_toErase[0] + _toErase.size());

    if (const size_t position = _mainString.find(_toErase); position != std::string::npos)
    {
        _mainString.erase(position, _toErase.length());
    }
}

int main()
{
    std::string directoryToCheck;
    std::string prefixToTrim;

    std::cout << "Which directory and all subdirectories should be checked?" << std::endl;
    std::cin >> directoryToCheck;

    std::cout << "Which prefix should be deleted? (case insensitive)" << std::endl;
    std::cin >> prefixToTrim;

    using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;
    {
        for (const auto& dirEntry : recursive_directory_iterator(directoryToCheck))
        {
            if(dirEntry.is_regular_file())
            {
                auto directory = dirEntry.path().parent_path().generic_string();
                auto filename = dirEntry.path().filename().generic_string();

                std::cout << "filename: " << filename;

                if (filename.find(prefixToTrim) == std::string::npos)
                {
                    continue;
                }
                
                EraseSubStr(filename, prefixToTrim);

                auto file = directory + "\\" + filename;

                if (rename(dirEntry.path().generic_string().c_str(), file.c_str()) != 0)
                {
                    perror("Error renaming file");
                    return 0;
                }
                else
                {
                    std::cout << "before the change: '" << dirEntry.path() << "'" << std::endl;
                    std::cout << "after the change: '" << file << "'" << std::endl;
	                std::cout << "File renamed successfully" << std::endl;
                }
            }
        }
    }
}

