#ifndef MENU_VIEW_AS_H
#define MENU_VIEW_AS_H

template <typename T>
constexpr auto view_as(const auto var)
{
    return (T)(var);
}

#endif

