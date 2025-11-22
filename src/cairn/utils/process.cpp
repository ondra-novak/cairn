export module cairn.utils.process;
#ifdef _WIN32
export import :win;
#else
export import :posix;
#endif