export module cairn.utils.utf8;

import <iostream>;
import <string>;

export inline void output_u8string(std::ostream &out, std::u8string_view text) {
    for (auto &x: text) out.put(x);
}

export inline std::iostream &operator<<(std::iostream &out, std::u8string_view x) {output_u8string(out,x); return out;}
export inline std::ostream &operator<<(std::ostream &out, std::u8string_view x) {output_u8string(out,x); return out;}
export inline std::iostream &operator<<(std::iostream &out, const std::u8string &x) {output_u8string(out,x); return out;}
export inline std::ostream &operator<<(std::ostream &out, const std::u8string &x) {output_u8string(out,x); return out;}

export inline std::u8string_view u8_from_string(std::string_view s) {
    return std::u8string_view(reinterpret_cast<const char8_t *>(s.data()),s.size());
}
export inline std::string_view string_from_u8(std::u8string_view s) {
    return std::string_view(reinterpret_cast<const char *>(s.data()),s.size());
}
