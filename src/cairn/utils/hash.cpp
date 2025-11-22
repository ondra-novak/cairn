export module cairn.utils.hash;

import <cstddef>;
export inline std::size_t hash_combine(std::size_t lhs, std::size_t rhs) noexcept
{
    lhs ^= rhs + 0x9e3779b9 + (lhs << 6) + (lhs >> 2);
    return lhs;
}


export struct MethodHash {
    constexpr std::size_t operator()(const auto &key) const {
        return key.hash();
    }
};
