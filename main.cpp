#include "Game.h"

int main() 
{
	Game game("MainCraft", 1280, 720, 4, 4, true);


	while (!game.getWindowShouldClose()) {
		//UPDATE INPUT ---
		
		game.update();
		game.render();
		
	}

	//END OF PROGRAM

	//Delete program
	//core_program.unuse();
	
	glfwTerminate();
	return 0;
}