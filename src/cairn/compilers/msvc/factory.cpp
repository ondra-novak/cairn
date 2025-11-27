export module cairn.compiler.msvc;

import cairn.abstract_compiler;
import <memory>;

export std::unique_ptr<AbstractCompiler> create_compiler_msvc( AbstractCompiler::Config ) {
    throw std::runtime_error("unsupported");
}
