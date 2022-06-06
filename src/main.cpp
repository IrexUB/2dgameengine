#include "Game/Game.h"
#include "Logger/Logger.h"

int main(void)
{
    try
    {
        Game game;
        game.Run();
    }
    catch (const std::exception &e)
    {
        Logger::LogError(e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}