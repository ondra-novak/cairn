export module cairn.compiler.gcc;

import cairn.abstract_compiler;
import <memory>;

export std::unique_ptr<AbstractCompiler> create_compiler_gcc(AbstractCompiler::Config );
