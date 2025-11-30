#include <fxng/engine.hxx>

int main()
{
    fxng::Engine engine(
        {
            .Application = {
                .Name = "Game",
                .Vendor = "Default",
                .Version = 0,
            },
            .Windows = {
                { .Main = true },
                { .Decorated = false, .Width = 800, .Height = 600 },
                { .Decorated = false, .Width = 400, .Height = 300 },
            },
        });
}
