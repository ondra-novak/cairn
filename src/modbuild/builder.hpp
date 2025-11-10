#pragma once

#include "abstract_compiler.hpp"
#include "module_database.hpp"
#include "utils/thread_pool.hpp"
#include <condition_variable>
class Builder {
public:

    Builder(std::size_t threads, AbstractCompiler &compiler);


    bool build(std::vector<ModuleDatabase::CompilePlan> plan);


protected:

    ThreadPool _thrp;
    std::mutex _mx;
    std::condition_variable _cv;
    bool _event = false;
    AbstractCompiler &_compiler;



}