#include <algorithm>
#include <fstream>
#include <iostream>
#include <istream>
#include <vector>

#include "yaml-cpp/yaml.h"

std::string toLower(std::string str)
{
    std::transform(str.begin(),
                   str.end(),
                   str.begin(),
                   [](unsigned char c){ return std::tolower(c); });

    return str;
}

std::string toFunction(std::string str)
{
    return "lantern_" + toLower(str);
}

std::string buildArguments(std::string name, YAML::Node node)
{
    std::string arguments = "";

    for (size_t idx = 0; idx < node.size(); idx++)
    {
        if (idx > 0)
        {
            arguments += ", ";
        }

        arguments += "void* " + node[idx]["name"].as<std::string>();
        arguments += ", const char* " + node[idx]["name"].as<std::string>() + "Type";
    }

    return arguments;
}

void replaceFile(std::string path,
                 std::string start,
                 std::string end,
                 std::vector<std::string> replacement)
{
    // read input file
    std::string line;
    std::ifstream input(path);
    std::vector<std::string> content;
    while (std::getline(input, line))
    {
        content.push_back(line);
    }
    input.close();

    // make replacements
    auto iterStart = std::find(content.begin(), content.end(), start);
    if (iterStart != content.end())
    {
        auto iterEnd = std::find(iterStart, content.end(), end);
        if (iterStart != content.end())
        {
            std::cout << "Replacing " << path << std::endl;

            content.erase(iterStart + 1, iterEnd);
            content.insert(iterStart + 1, replacement.begin(), replacement.end());
        }
    }

    // write output file
    std::ofstream output(path);
    for (auto iter = content.begin(); iter != content.end(); iter++)
    {
        output << *iter << std::endl;
    }
    output.close();
}

int main(int argc, char *argv[])
{
    if (argc < 4) {
        std::cout << "Usage: lanterngen declarations.yaml lantern.cpp lantern.h" << std::endl;
        return 1;
    }

    char* pathDeclarations = argv[1];
    char* pathSource = argv[2];
    char* pathHeader = argv[3];

    YAML::Node config = YAML::LoadFile(pathDeclarations);

    std::cout << "Loaded " << pathDeclarations << " with " << config.size() << " nodes" << std::endl;

    // generate function headers
    std::vector<std::string> headers;
    headers.push_back("/*");
    for (size_t idx = 0; idx < config.size(); idx++)
    {
        std::string name = config[idx]["name"].as<std::string>();
        std::string arguments = buildArguments(name, config[idx]["arguments"]);

        headers.push_back("LANTERN_API void (LANTERN_PTR " + toFunction(name) + ")(" + arguments + ");");
    }
    headers.push_back("*/");

    // generate function bodies
    std::vector<std::string> bodies;
    bodies.push_back("/*");
    for (size_t idx = 0; idx < config.size(); idx++)
    {
        std::string name = config[idx]["name"].as<std::string>();
        std::string arguments = buildArguments(name, config[idx]["arguments"]);

        bodies.push_back("void " + toFunction(name) + "(" + arguments + ") {}");
    }
    bodies.push_back("*/");

    // generate symbol loaders
    std::vector<std::string> symbols;
    symbols.push_back("  /*");
    for (size_t idx = 0; idx < config.size(); idx ++)
    {
        std::string name = config[idx]["name"].as<std::string>();
        symbols.push_back("  LOAD_SYMBOL(" + toFunction(name) + ")");
    }
    symbols.push_back("  */");

    replaceFile(pathSource, "/* Autogen Body -- Start */", "/* Autogen Body -- End */", bodies);
    replaceFile(pathHeader, "/* Autogen Headers -- Start */", "/* Autogen Headers -- End */", headers);
    replaceFile(pathHeader, "  /* Autogen Symbols -- Start */", "  /* Autogen Symbols -- End */", symbols);

    return 0;
}