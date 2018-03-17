#include "include/GGEGame.h"

int main()
{

	GGE::Game *game = new GGE::Game();

	game->play();

	delete game;

	return 0;
}
