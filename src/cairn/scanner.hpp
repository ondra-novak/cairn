#pragma once
#include "module_type.hpp"
#include <string>
#include <vector>
#include <filesystem>

class SourceScanner {
public:

    struct Reference {
        ModuleType type;
        std::string name;
    };


    struct Info {
        std::string name;     //logical name of this module  (FQN for partition)
        ModuleType type = ModuleType::source;
        std::vector<Reference> required;  //list of logical names of required modules (partitions are FQN)
        std::vector<Reference> exported; //list of logical names of exported modules (must be also included as required)
    };


    static Info scan_string(std::string_view text);

protected:

    static Info scan_string_2(std::string_view text);


};