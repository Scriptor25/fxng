#include <fxng/glal.hxx>

void fxng::glal::DestroyInstance(const Instance *instance)
{
    delete instance;
}
