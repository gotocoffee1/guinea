#include "app.hpp"

class hello : public gn::app
{
    virtual void on_update(bool&) override
    {
        if (GN(Window, "Test"))
            gn::Text("Hello World");
    }
};


int main(int argc, char** argv)
{
    return hello().launch(argc, argv);
}
