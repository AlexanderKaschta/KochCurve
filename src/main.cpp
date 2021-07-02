/**
 * @date 26.12.2020
 * @author Alexander Kaschta
 */
#include <iostream>
#include <string>
#include <sstream>

#include "Game.h"

/**
 * Entry point of the application
 * @return int with a status code. It's zero, if everything executed fine, and it won't be something different, if an
 *         error occurred.
 */
int main(int argc, char *argv[]) {

    // Define a default size
    int n = 0;

    // Parse command line arguments
    if (argc > 2) {
        std::cout << "Ignoring invalid arguments." << std::endl;
    } else if (argc == 2) {
        std::stringstream input(argv[1]);
        if (!(input >> n)) {
            std::cout << "Argument can't be converted to an integer" << std::endl;
        } else if (n < 0) {
            n = 0;
        } else if (n > 10) {
            n = 10;
        }
    }

    // Create a new window instance
    Game game;

    game.Init("Blatt 11, Aufgabe 29 | Alexander Kaschta", 640, 480, n);

    // Game-Loop
    while (game.isRunning()) {
        game.CalculateTime();
        game.Update();
        game.Draw();
        game.HandleEvents();
    }

    game.Clear();

    return 0;
}
