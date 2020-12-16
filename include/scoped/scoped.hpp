#pragma once
#ifndef _GUINEA_SCOPED_HPP_
#define _GUINEA_SCOPED_HPP_

#define CONCAT_(x, y) x##y
#define CONCAT(x, y)  CONCAT_(x, y)
#define GUI(x, ...)                              \
    scoped::ui::x CONCAT(_gui_element, __LINE__) \
    {                                            \
        __VA_ARGS__                              \
    }

#endif
