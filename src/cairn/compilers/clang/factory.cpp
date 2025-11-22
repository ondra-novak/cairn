export module cairn.compiler.clang;

import cairn.abstract_compiler;
import <memory>;

export std::unique_ptr<AbstractCompiler> create_compiler_clang(AbstractCompiler::Config);


