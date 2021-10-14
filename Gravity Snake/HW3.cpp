#include "game.h"

/*
The above and beyond feature for my game is a main menu and end menu
in addition to some images that I added. The main menu features a picture
of a snake as a backdrop and a button that can be clicked to start the game.
The game screen has controls in the top right corner that are highlighted
when the respective key is pressed. In the top left corner, there is a button
that can be clicked to end the game preemptively. The end screen features text
displaying stats if you won the game, or game over text if you ended the game
early. There are also two buttons that allow the player to play again or return
to the main menu.
*/

int main()
{
	RenderWindow window(VideoMode(800, 600), "Gravity Snake");

	createWorld(window);
	update(window);
}