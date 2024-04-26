#include <iostream>
#include "Vanessa_game.h"

int main()
{
    Vanessa_game game;

    // init
    game.init();
    //game.init_window(1024, 768);
    game.init_window(1200, 1000);

    // game loop
    while(game.running())
    {
        // check events & update calculations
        game.update();

        // render
        game.render();

    }

}

