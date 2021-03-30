
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
        }
    }
};

int main()
{
    return app{}.launch();
}
