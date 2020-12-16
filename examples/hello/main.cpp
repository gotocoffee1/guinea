
#include "guinea.hpp"


struct app final : ui::guinea
{
    void loop(bool&) noexcept override
    {
        if (GUI(Window, "hello world"))
        {

        }
    }
};

int main()
{
    return app{}.launch();
}