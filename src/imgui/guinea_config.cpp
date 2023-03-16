#include "../../include/guinea_base.hpp"

#ifdef USE_GUINEA_ASSERT
#include <cassert>

namespace ImGui
{
void guinea_assert(const char* msg)
{
    if (auto* ctx = ui::ctx::get_current())
        ctx->failure(msg);
    else
        assert(false && msg);
}
} // namespace ImGui
#endif