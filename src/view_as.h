#ifndef SHELLCODE_VIEW_AS_H
#define SHELLCODE_VIEW_AS_H

template <typename T>
inline constexpr auto view_as(const auto var)
{
    return (T)(var);
}

#endif

