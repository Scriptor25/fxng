#include <glal/glal.hxx>

void glal::DestroyInstance(const Instance instance)
{
    delete instance;
}
