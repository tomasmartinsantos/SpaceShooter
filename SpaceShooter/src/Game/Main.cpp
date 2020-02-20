#include "World.h"
#include "Scene.h"
#include "Components/HeadersComponents.h"
#include "../Engine/Audio/Audiosource.h"

int main(int argc, char* argv[])
{
    World::Instance()->Main();

    return 0;
}