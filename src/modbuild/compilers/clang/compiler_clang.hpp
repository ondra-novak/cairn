#pragma once
#include "../../abstract_compiler.hpp"
#include "../../utils/arguments.hpp"
#include "../../utils/version.hpp"
#include <filesystem>

class CompilerClang : public AbstractCompiler {
public:

    virtual std::string_view get_compiler_name() const override {
        return "clang";
    }

    virtual int compile(
        const OriginEnv &env,
        const SourceDef &src,
        std::span<const SourceDef> modules,
        CompileResult &result) const override;
    
    virtual int link(std::span<const std::filesystem::path> objects) const override;

        virtual std::string preproces(
                const OriginEnv &env,
                const std::filesystem::path &file) const override;

    CompilerClang(Config config);

    virtual bool initialize_build_system(BuildSystemConfig ) override {
        return false;
    }

    virtual bool commit_build_system() override {
        return false;
    }


    virtual bool generate_compile_command(
        const OriginEnv &env,
        const SourceDef &src,
        std::span<const SourceDef> modules,
        std::vector<ArgumentString> &result) const override;


    virtual void initialize_module_map(std::span<const SourceDef> ) override {}


    static constexpr auto preprocess_flag = ArgumentConstant("-E");
    static constexpr auto bmi_ext = ArgumentConstant("pcm");
    static constexpr auto fmodule_header_user = ArgumentConstant("-fmodule-header=user");
    static constexpr auto fmodule_header_system = ArgumentConstant("-fmodule-header=system");    
    static constexpr auto xcpp_system_header = ArgumentConstant("-xc++-system-header");
    static constexpr auto xcpp_header = ArgumentConstant("-xc++-header");
    static constexpr auto xcpp_module = ArgumentConstant("-xc++-module");
    static constexpr auto output_flag = ArgumentConstant("-o");
    static constexpr auto precompile_flag = ArgumentConstant("--precompile");
    static constexpr auto compile_flag = ArgumentConstant("-c");
    static constexpr auto fmodule_file = ArgumentConstant("-fmodule-file=");
    static constexpr auto version_flag = ArgumentConstant("--version");
    static constexpr auto fprebuild_module_path = ArgumentConstant("-fprebuilt-module-path=");
    static constexpr auto wno_pragma_system_header_outside_header = ArgumentConstant("-Wno-pragma-system-header-outside-header");
    static constexpr auto wno_experimental_header_units = ArgumentConstant("-Wno-experimental-header-units");
    static constexpr auto stdcpp=ArgumentConstant("-std=c++");
    static constexpr auto stdcpp17=ArgumentConstant("-std=c++17"); //need to disable modules during preprocess

    //preprocessor options
    static constexpr auto preproc_D = ArgumentConstant("-D");
    static constexpr auto preproc_U = ArgumentConstant("-U");
    static constexpr auto preproc_I = ArgumentConstant("-I");
    static constexpr auto preproc_define_macro = ArgumentConstant("--define-macro");
    static constexpr auto preproc_undefine_macro = ArgumentConstant("--undefine-macro");
    static constexpr auto preproc_include_directory = ArgumentConstant("--include-directory");
    
    static constexpr auto all_preproc = std::array<ArgumentStringView, 6>({
        preproc_D, preproc_I, preproc_U, preproc_define_macro, preproc_undefine_macro, preproc_include_directory
    });


protected:
    Config _config;
    std::filesystem::path _module_cache;
    std::filesystem::path _object_cache;
    Version _version;
    
  

    std::filesystem::path get_bmi_path(const SourceDef &src) const {
        auto n = src.name;
        for (auto &c: n) if (c == ':') c = '-';
        std::filesystem::path fname(n);
        fname.replace_extension(".pcm");
        return _config.working_directory/"pcm"/fname;
    };

    std::filesystem::path get_obj_path(const SourceDef &src) const {
        return _config.working_directory/"obj"/intermediate_file(src,".o");
    }
    std::filesystem::path get_hdr_bmi_path(const SourceDef &src) const {
        return _config.working_directory/"pcm"/intermediate_file(src,".pcm");
    }
    std::vector<ArgumentString> build_arguments(bool precompile_stage,  const OriginEnv &env,
        const SourceDef &src,
        std::span<const SourceDef> modules,
        CompileResult &result) const;
    
};