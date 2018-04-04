#include <stdio.h>
#include <SDL.h>

// screen size
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Starts up SDL and creates a window
bool init();

// Loads media
bool loadMedia();

// Frees media and shuts down SDL
void close();

// Global variables:

// The window we will be drawing to
SDL_Window* gWindow = NULL;

// The surface we will be adding to the window to draw to
SDL_Surface* gScreenSurface = NULL;

// Image that will be shown on the screen
SDL_Surface* gHelloWorld = NULL;

bool init() {
	// Initialization flag; this will be returned as it is if everything is successful
	bool success = true;

	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else {
		// Create window
		gWindow = SDL_CreateWindow("SDL_Tutorial_2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL) {
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else {
			// Get the window surface
			gScreenSurface = SDL_GetWindowSurface(gWindow);
		}
	}

	return success;
}

bool loadMedia() {
	// Loading succes flag
	bool success = true;

	// Load splash image
	gHelloWorld = SDL_LoadBMP("Images/hello_world.bmp");
	if (gHelloWorld == NULL) {
		printf("Unable to load image! SDL Error: %s\n", SDL_GetError());
		success = false;
	}

	return success;
}


void close() {
	// Deallocate surface
	SDL_FreeSurface(gHelloWorld);
	gHelloWorld = NULL;

	// Destroy window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	// Quit SDL subsystems
	SDL_Quit();
}

int main(int argc, char* args[]) {
	if (!init()) {
		printf("Cannot initialize!");
	}
	else {
		if (!loadMedia()) {
			printf("Cannot load media!");
		}
		else {
			// apply the iamge through blitting
			// Blitting takes a source surface and stamps a copy of it onto the destination surface.
			// The first argument is the source image while the third is the destination.
			SDL_BlitSurface(gHelloWorld, NULL, gScreenSurface, NULL);

			// Always need to update surface to see the image on the screen
			// When we draw to the surface, we are rendering the frame to the back buffer.
			// What is shown to the user is the front buffer.
			// UdpateWindowSurface() swaps the back and front buffer so we can see the finished frame rather than an unfinished frame as we draw to it.
			// Essentially, we update the window surface after every blit.
			SDL_UpdateWindowSurface(gWindow);

			// Wait two seconds
			SDL_Delay(2000);
		}
	}

	// Free resources and close SDL
	close();

	return 0;
}