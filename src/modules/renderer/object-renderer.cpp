#include "./object-renderer.hpp"
#include "./shader-renderer.hpp"
#include "either.hpp"
#include "exceptions/base-exception.hpp"
#include "./object.hpp"

#include "stb_image/stb_image.h"
#include "filesystem"

void ObjectRenderer::spawnMany(Object *obj, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        this->spawn(obj[i]);
    }
}

void ObjectRenderer::spawn(Object obj)
{
    this->objects.push_back(obj);
}

void ObjectRenderer::start_all()
{

    for (int i = 0; i < this->objects.size(); i++)
    {
        this->objects[i].start();
    }
}

void ObjectRenderer::render_all()
{
    for (int i = 0; i < this->objects.size(); i++)
    {
        this->objects[i].render();
    }
}
