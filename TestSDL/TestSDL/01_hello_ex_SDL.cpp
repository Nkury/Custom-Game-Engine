// Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>

//Screen dimension constraints
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// main must be instantiated in this way for SDL to be compatible with multiple platforms
int main(int argc, char* args[]) {
	// The window we will be rendering to
	SDL_Window* window = NULL;

	// The surface contained by the window
	SDL_Surface* screenSurface = NULL;

	// Initialize SDL using SDL's video subsystem since we don't care about anything else
	// hence the SDL_INIT_VIDEO parameter
	// If there is an error, SDL_Init will return -1.
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {

		// SDL_GetError() will let you know if any errors occur within an SDL function
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else {
		// Create window using SDL_CreateWindow
		// The first parameter is the title bar caption of the window
		// The second and third parameter tells the window where in the x and y position, respectively, to create the window
		// The fourth and fifth parameter dictate the window size
		// The last parameter tells the function what creation flags should be taken into consideration-- in this case, always show the window.
		window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL) {
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}
		else {
			// Get window surface so we can draw to it.
			screenSurface = SDL_GetWindowSurface(window);

			// Fill the surface white
			SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));

			// Update the surface. Just because we draw to a surface doesn't mean it will show up. We will need to update the window surface to show
			// everything that has been drawn.
			SDL_UpdateWindowSurface(window);

			// Wait two seconds (parameter in milliseconds)
			// When SDL_Delay is called, it will not be able to respond to input from keyboard or mouse
			SDL_Delay(2000);
		}
	}

	// Destroy window to free its memory (also taking care of the SDL_Surface we got from it.
	SDL_DestroyWindow(window);

	// Quit SDL subsystems (the libraries)
	SDL_Quit();

	// Terminate the program
	return 0;
}