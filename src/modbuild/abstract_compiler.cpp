#include "abstract_compiler.hpp"
#include "utils/process.hpp"

int AbstractCompiler::invoke(const Config &cfg, 
    const std::filesystem::path &workdir, 
    std::span<const ArgumentString> arguments)
{
    Process p = Process::spawn(cfg.program_path, workdir, arguments, true);
    return p.waitpid_status();

}



std::vector<ArgumentString> AbstractCompiler::prepare_args(const OriginEnv &env) {
    std::vector<ArgumentString> out;
    ArgumentString a;
    for (const auto &i: env.includes) {
        auto s = path_arg(i);
        a.push_back('-');
        a.push_back('I');
        a.append(s);
        out.push_back(a);
    }
    return out;        
}
