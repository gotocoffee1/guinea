
#include "guinea.hpp"

#include <thread>

struct app final : ui::guinea
{
    void render() noexcept override
    {
        if (GUI(Window, "hello world"))
        {
            {
                static int64_t num = 50;
                ui::DragNum("Value", num, 1.f);
            }
            {
                static int64_t num = 50;
                ui::SliderNum("asdad", num, 1, 4);
            }
            static std::string drop = "[none]";

            ui::Button("target");

            if (GUI(DragDropTarget))
            {
                if (const ImGuiPayload* payload = ui::AcceptDragDropPayload(UI_EXTERN_FILE))
                {
                    drop.clear();
                    auto begin = static_cast<const char*>(payload->Data);
                    auto end   = begin + payload->DataSize;
                    while (begin != end)
                    {
                        std::string_view sv = begin;
                        begin += std::size(sv) + 1;
                        drop += sv;
                    }
                }
            }
            ui::TextUnformatted(drop);
        }
        ui::ShowAboutWindow();
        ui::ShowMetricsWindow();
    }
};

int main(int, char**)
{
    app a{};
    while (a.loop())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
    return 0;
}
