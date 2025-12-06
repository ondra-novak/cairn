# Cairn

An experimental tool for building programs written using C++20 modules.  

This tool performs the following steps:
1. **Discovery** – finds all modules and their required files.
2. **Scanning** – inspects all discovered files and generates metadata for them, determining their type and dependencies.
3. **Planning** – creates a build plan.
4. **Build execution** – invokes the compiler and compiles all modules in the correct order, ensuring proper references to imports.
5. **Automatic BMI generation** – automatically builds BMI for header modules.

# Installation

The project is written in C++20 modules, so it requires itself for a full compilation. To allow building without prior installation, two scripts are provided, one for each platform.

## Linux

**Requirements:**
- clang++ version 18 or higher
- **make** command

**Steps:**

```bash
$ make all
$ sudo make install
```

Note: if you need to specify the compiler version, you can use the CLANG variable:

```
$ make all CLANG=clang++-18
```

## Windows

**Requirements**

Microsoft Visual Studio Community 2022/2026

1) Open the Development Command Prompt for the target platform (e.g., **x64 Native Tools Developer Command Prompt for Visual Studio Community**).
2) Navigate to the directory where the project is unpacked.
3) Run `compile.bat`

# Usage

Check the help with the -h flag:

```
cairn -h
```

## modules.yaml

The `modules.yaml` file should be created in the folder containing your module files. It has the following entries:

- **files** - list of files to be included in the compilation.
- **prefixes** - an associative array of prefixes. Each prefix can contain one or more paths where additional modules will be searched. A prefix defines the namespace of modules located on that path.
- **includes** - adds paths to the include list.
- **options** - adds compiler flags.

## překlad

```
cairn <flagy> target=pathname.cpp compiler <compiler flags>
```

Example:

```
cairn -Bbuild -cbuild/compile_commands.json build/cairn=src/cairn/main clang++ -std=c++20 -O3
```

# CLI help

```
cairn [...switches...] <output1=file1.cpp> [<output2=file2.cpp>... ] <compiler> [...compiler/linker..flags...]
```

### Switches
| Switch | Description |
|--------|-------------|
| `-jN` | Specify number of threads to use. |
| `-p<type>` | Select compiler type: `gcc`, `clang`, or `msvc`. Use when compiler profile cannot be detected automatically |
| `-c<path>` | Generate `compile_commands.json` at the given path. |
| `-f<file>` | Specify environment file (`modules.yaml`) for this build. Can contains targets |
| `-B<dir>` | Specify build directory (build cache) |
| `-C` | Compile only (do not run linker). |
| `-L` | Link only (requires compiled files in build directory). |
| `-k` | Keep going on errors (continue build). |
| `-r` | Recompile all related files. |
| `-R` | Drop database, rescan, and recompile everything. |
| `-s` | Silent output — show only errors. |
| `-d` | Debug mode — output detailed information. |
| `-l`, `--list` | Do not compile; output list of all referenced modules and headers. |
| `-M<file>` | Do not compile; create a Makefile with all build steps (works well with `clang++` v18+). |
| `-S<file>` | Do not compile; create a BAT script with all build steps (works well with `cl.exe`). |
| outputN=fileN.cpp | specifies target and source cpp file. There can be multiple targets in this section. If you don't specify targets, targets specified in -f<file> will be compiled |
| compiler/linker flags | specify all required flags for the compiler. It is recommended at least to include `-std=c++20` (`/std:c++20` in Windows). 
| `--compile:` | following flags are used only during compile phase
| `--link:` | following flags are used only during link phase

### Special usage

```
cairn -fproject.yaml ...
```

if project.yaml contains targets, compiles all these targets


```
cairn --scan <file> <compiler> <flags>
```

Run scanner and output its results

## Sample output

```
$ ./.install/cairn -f cairn.yaml -j0 clang++ -std=c++20 -O3 -DNDEBUG 
Settings has been changed, rebuilding
[  1%] Compiled: cerrno
[  2%] Compiled: cstddef
[  3%] Compiled: cstring
[  5%] Compiled: exception
[  7%] Compiled: optional4 warnings generated.
[  8%] Compiled: type_traits
[  9%] Compiled: numeric
[ 10%] Compiled: cstdint
[ 11%] Compiled: span
[ 13%] Compiled: utility
[ 14%] Compiled: variant
[ 15%] Compiled: concepts
[ 16%] Compiled: vector
[ 17%] Compiled: array
[ 18%] Compiled: unordered_map
[ 20%] Compiled: cctype
[ 21%] Compiled: map
[ 22%] Compiled: set
[ 23%] Compiled: stdexcept
[ 24%] Compiled: system_error
[ 25%] Compiled: string
[ 26%] Compiled: atomic
[ 28%] Compiled: algorithm
[ 29%] Compiled: string_view
[ 30%] Compiled: iostream
[ 31%] Compiled: mutex
[ 32%] Compiled: memory
[ 33%] Compiled: cwctype
[ 34%] Compiled: /home/ondra/vscode/cairn/src/cairn/utils/hash.cpp
[ 36%] Compiled: functional
[ 37%] Compiled: unordered_set
[ 38%] Compiled: /home/ondra/vscode/cairn/src/cairn/utils/function_view.cpp
[ 39%] Compiled: /home/ondra/vscode/cairn/src/cairn/module_type.cpp
[ 40%] Compiled: fstream
[ 41%] Compiled: /home/ondra/vscode/cairn/src/cairn/version.cpp
[ 43%] Compiled: queue
[ 44%] Compiled: iterator
[ 45%] Compiled: /home/ondra/vscode/cairn/src/cairn/utils/utf_8.cpp1 warning generated.
[ 46%] Compiled: sstream
[ 47%] Compiled: filesystem
[ 48%] Compiled: /home/ondra/vscode/cairn/src/cairn/compile_target.cpp
[ 49%] Compiled: /home/ondra/vscode/cairn/src/cairn/utils/serialization_rules.cpp
[ 51%] Compiled: streambuf
[ 52%] Compiled: /home/ondra/vscode/cairn/src/cairn/utils/serializer.cpp
[ 53%] Compiled: ranges
[ 54%] Compiled: ostream
[ 55%] Compiled: /home/ondra/vscode/cairn/src/cairn/source_def.cpp
[ 56%] Compiled: /home/ondra/vscode/cairn/src/cairn/origin_env.cpp
[ 57%] Compiled: /home/ondra/vscode/cairn/src/cairn/build_plan.cpp
[ 59%] Compiled: regex
[ 60%] Compiled: format
[ 61%] Compiled: /home/ondra/vscode/cairn/src/cairn/module_resolver.ifc.cpp
[ 62%] Compiled: /home/ondra/vscode/cairn/src/cairn/utils/fd_streambuf.cpp
[ 63%] Compiled: /home/ondra/vscode/cairn/src/cairn/utils/arguments.cpp
[ 64%] Compiled: /home/ondra/vscode/cairn/src/cairn/utils/version.cpp
[ 66%] Compiled: /home/ondra/vscode/cairn/src/cairn/utils/log.ifc.cpp
[ 67%] Compiled: thread
[ 68%] Compiled: condition_variable
[ 69%] Compiled: /home/ondra/vscode/cairn/src/cairn/preprocess.ifc.cpp
[ 70%] Compiled: /home/ondra/vscode/cairn/src/cairn/scanner.cpp
[ 71%] Compiled: /home/ondra/vscode/cairn/src/cairn/utils/env.cpp
[ 72%] Compiled: /home/ondra/vscode/cairn/src/cairn/utils/log.cpp
[ 74%] Compiled: /home/ondra/vscode/cairn/src/cairn/utils/thread_pool.cpp
[ 75%] Compiled: chrono
[ 76%] Compiled: /home/ondra/vscode/cairn/src/cairn/cli.cpp
[ 77%] Compiled: /home/ondra/vscode/cairn/src/cairn/utils/simple_json.cpp
[ 78%] Compiled: /home/ondra/vscode/cairn/src/cairn/compile_commands_supp.ifc.cpp
[ 79%] Compiled: /home/ondra/vscode/cairn/src/cairn/utils/process_posix.cpp
[ 80%] Compiled: /home/ondra/vscode/cairn/src/cairn/utils/process.cpp
[ 82%] Compiled: /home/ondra/vscode/cairn/src/cairn/preprocess.cpp
[ 83%] Compiled: /home/ondra/vscode/cairn/src/cairn/gnu_compiler_setup.cpp
[ 84%] Compiled: /home/ondra/vscode/cairn/src/cairn/compile_commands_supp.cpp
[ 85%] Compiled: /home/ondra/vscode/cairn/src/cairn/abstract_compiler.cpp
[ 86%] Compiled: /home/ondra/vscode/cairn/src/cairn/compilers/clang/factory.cpp
[ 87%] Compiled: /home/ondra/vscode/cairn/src/cairn/compilers/gcc/factory.cpp
[ 89%] Compiled: /home/ondra/vscode/cairn/src/cairn/compilers/msvc/factory.cpp
[ 90%] Compiled: /home/ondra/vscode/cairn/src/cairn/builder.cpp
[ 91%] Compiled: /home/ondra/vscode/cairn/src/cairn/module_database.ifc.cpp
[ 92%] Compiled: /home/ondra/vscode/cairn/src/cairn/module_resolver.cpp
[ 93%] Compiled: /home/ondra/vscode/cairn/src/cairn/script_build.cpp
[ 94%] Compiled: /home/ondra/vscode/cairn/src/cairn/compilers/msvc/compiler_msvc.cpp
[ 95%] Compiled: /home/ondra/vscode/cairn/src/cairn/compilers/clang/compiler_clang.cpp
[ 97%] Compiled: /home/ondra/vscode/cairn/src/cairn/compilers/gcc/compiler_gcc.cpp
[ 98%] Compiled: /home/ondra/vscode/cairn/src/cairn/main.cpp
[ 99%] Compiled: /home/ondra/vscode/cairn/src/cairn/module_database.cpp
[100%] Linked: /home/ondra/vscode/cairn/build/cairn
Done
```

## exmaple of scanning
```
$ ./.install/cairn --scan ./src/cairn/module_database.ifc.cpp clang++ -std=c++20 -O3 -DNDEBUG 
---
module_name: cairn.module_database
type: interface
exports: []
imports:
 - type: interface
   name: cairn.module_type
 - type: interface
   name: cairn.source_scanner
 - type: interface
   name: cairn.module_resolver
 - type: interface
   name: cairn.utils.arguments
 - type: interface
   name: cairn.utils.hash
 - type: interface
   name: cairn.origin_env
 - type: interface
   name: cairn.build_plan
 - type: interface
   name: cairn.compile_target
 - type: interface
   name: cairn.abstract_compiler
 - type: interface
   name: cairn.compile_commands
 - type: system_header
   name: atomic
 - type: system_header
   name: chrono
 - type: system_header
   name: functional
 - type: system_header
   name: iterator
 - type: system_header
   name: filesystem
 - type: system_header
   name: type_traits
 - type: system_header
   name: unordered_map
 - type: system_header
   name: vector
 - type: system_header
   name: map
 - type: system_header
   name: variant
 - type: system_header
   name: span
```