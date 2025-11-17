#pragma once
#include "module_type.hpp"
#include <filesystem>

struct SourceDef {
    ModuleType type;    //type of compiled module
    std::string name;   //name of compiled module 
    std::filesystem::path path;  //path to source file / interface file

    bool operator==(const SourceDef &other) const = default;
    std::size_t hash() const {return std::hash<std::string>()(name);}
};

