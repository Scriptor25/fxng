#include <../../glal/include/glal/glal.hxx>

void glal::DestroyInstance(Ptr<Instance> instance)
{
    delete instance;
}
