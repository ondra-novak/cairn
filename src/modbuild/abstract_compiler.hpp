#pragma once
#include <string>
#include <filesystem>
#include <vector>
#include <span>
#include "module_type.hpp"
#include "utils/arguments.hpp"
#include "utils/which.hpp"

class AbstractCompiler {
public:

    struct Depends {
        std::string name;     //logical name of this module  (FQN for partition)
        bool is_interface;    //true, if module is interface (generates BMI), false if it is implementation (must be also in required)
        std::vector<std::string> required;  //list of logical names of required modules (partitions are FQN)
        std::vector<std::string> exported; //list of logical names of exported modules (must be also included as required)
    };

    struct CompileResult {
        std::filesystem::path interface;
        std::filesystem::path object;
    };

    struct ModuleMapping {
        std::string logical_name;
        std::filesystem::path interface;
    };

    struct Config {
        std::filesystem::path program_path;
        std::vector<ArgumentString> compile_options;
        std::vector<ArgumentString> link_options;
        std::filesystem::path working_directory;
    };

    virtual ~AbstractCompiler() = default;

    ///Changes working directory - should be used as first
    virtual void set_working_directory(std::filesystem::path) = 0;

    ///Initialize module map
    /** Use this before build. Some compilers can benefit of this, other not
        @param module_interface_cpp_list list of pair: module name and path to cpp file contains interface. The compiler
        use name to generate BMI name. 

        You must initialize map before you can compile any module. 
        This feature is used by gcc module-mapper. Gcc fails to compile module if it was not
        properly anounced. Clang and msvc ignores this feature.
    */
    virtual void initialize_module_map(std::vector<std::pair<std::string, std::filesystem::path> > module_interface_cpp_list) = 0;

    virtual int compile(const std::filesystem::path &source_ref, 
        ModuleType type,
        std::span<const ModuleMapping> modules,
        CompileResult &result) const = 0;
    
    virtual int link(std::filesystem::path binary, 
        std::span<const std::filesystem::path> objects) const = 0;

    virtual std::string preproces(const std::filesystem::path &file) const = 0;
    

    static constexpr auto compile_flag = ArgumentConstant("--compile:");
    static constexpr auto link_flag = ArgumentConstant("--link:");

    enum class State {
        common, compile, link
    };

    static Config parse_commandline(const std::span<const ArgumentString> &args, std::filesystem::path working_dir) {
        Config out;
        State st = State::common;
        if (args.empty()) return out;

        auto found = find_in_path(args[0]);
        if (found.has_value()) out.program_path = std::move(found.value());
        else out.program_path = args[0];

        out.working_directory = std::move(working_dir);

        auto params = args.subspan(1);

        for (auto &a: params) {
            if (a == compile_flag) st = State::compile;
            else if (a == link_flag) st = State::link;
            else { 
                switch (st) {
                    case State::common: out.compile_options.push_back(a);
                                        out.link_options.push_back(a);break;
                    case State::compile: out.compile_options.push_back(a);break;
                    case State::link: out.link_options.push_back(a);break;
                }
            }            
        }
        return out;

    }

};
