
#include "guinea.hpp"

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
                    auto end = begin + payload->DataSize;
                    while(begin != end)
                    {
                        std::string_view sv = begin;
                        begin += std::size(sv) + 1;
                        drop += sv;
                    }
                }
            }
            ui::TextUnformatted(drop);
        }
    }
};

int main()
{
    return app{}.launch();
}
