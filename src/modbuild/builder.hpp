#pragma once

#include "abstract_compiler.hpp"
#include "compile_commands_supp.hpp"
#include "module_database.hpp"
#include "utils/thread_pool.hpp"
#include <future>


class Builder {
public:

    Builder(std::size_t threads, AbstractCompiler &compiler, OriginEnv default_env);

    std::future<bool> build(std::vector<ModuleDatabase::CompilePlan> plan, bool stop_on_error);
    void generate_compile_commands(CompileCommandsTable &cctable, std::vector<ModuleDatabase::CompilePlan> plan);
    static std::vector<AbstractCompiler::SourceDef> create_module_mapping(const std::span<const ModuleDatabase::CompilePlan> &plan);

    


protected:

    ThreadPool _thrp;
    AbstractCompiler &_compiler;
    OriginEnv _default_env;

};