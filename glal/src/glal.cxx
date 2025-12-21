#include <glal/glal.hxx>

void glal::DestroyInstance(Instance instance)
{
    delete instance;
}
