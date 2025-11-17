#pragma once

#include "fd_streambuf.hpp"
#include <filesystem>
#include <optional>

#include <iostream>
#include <vector>
#include "arguments.hpp"
#include <ext/stdio_filebuf.h>

class Process {
public:
    const pid_t pid = -1;
    std::optional<fd_streambuf> child_stdin_buf;
    std::optional<fd_streambuf> child_stdout_buf;
    std::optional<fd_streambuf> child_stderr_buf;
    std::optional<std::ostream> stdin_stream;
    std::optional<std::istream> stdout_stream;
    std::optional<std::istream> stderr_stream;

    Process(const Process&) = delete;
    Process& operator=(const Process&) = delete;

    int waitpid_status();
    void kill_child(int sig);
    void kill_child();

    enum StreamFlags {
        no_streams = 0,
        input = 1,
        output = 2,
        input_output =3,
        error = 4,
        input_error = 5,
        output_error = 6,
        input_output_error =7,
    };

    friend StreamFlags operator|(StreamFlags a, StreamFlags b) {
        return static_cast<StreamFlags>(static_cast<int>(a) | static_cast<int>(b));
    }
    friend int operator&(StreamFlags a, StreamFlags b) {
        return static_cast<int>(a) & static_cast<int>(b);
    }

    static Process spawn(const std::filesystem::path & path,
                         const std::filesystem::path & workdir,
                         const std::span<const ArgumentString>& args,
                         StreamFlags flags);

protected:
    Process(pid_t pid, int fdstdin, int fdstdout, int fdstderr);

};

