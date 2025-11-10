#include "builder.hpp"

Builder::Builder(std::size_t threads, AbstractCompiler &compiler)
:_compiler(compiler) 
{
    _thrp.start(threads);
}
    
bool Builder::build(std::vector<ModuleDatabase::CompilePlan> plan) {
    
}