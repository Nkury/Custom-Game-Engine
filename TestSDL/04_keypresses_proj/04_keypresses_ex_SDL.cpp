#include <stdio.h>
#include <SDL.h>
#include <string>

using namespace std;

// screen size
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// key press surface constants
enum KeyPressSurfaces {
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL
};

// Starts up SDL and creates a window
bool init();

// Loads media
bool loadMedia();

// Frees media and shuts down SDL
void close();

// Global variables:

// Loads individual image
SDL_Surface* loadSurface(string path);

// The window we will be drawing to
SDL_Window* gWindow = NULL;

// The surface we will be adding to the window to draw to
SDL_Surface* gScreenSurface = NULL;

// The images that correspond to a keypress
SDL_Surface* gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL]; 

// Current displayed image
SDL_Surface* gCurrentSurface = NULL;

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

	//Load default surface
	gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = loadSurface("Images/press.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] == NULL)
	{
		printf("Failed to load default image!\n");
		success = false;
	}

	//Load up surface
	gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] = loadSurface("Images/up.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] == NULL)
	{
		printf("Failed to load up image!\n");
		success = false;
	}

	//Load down surface
	gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] = loadSurface("Images/down.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] == NULL)
	{
		printf("Failed to load down image!\n");
		success = false;
	}

	//Load left surface
	gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] = loadSurface("Images/left.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] == NULL)
	{
		printf("Failed to load left image!\n");
		success = false;
	}

	//Load right surface
	gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] = loadSurface("Images/right.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] == NULL)
	{
		printf("Failed to load right image!\n");
		success = false;
	}

	return success;
}


void close() {
	// Deallocate surfaces
	for (int i = 0; i < KEY_PRESS_SURFACE_TOTAL; i++) {
		SDL_FreeSurface(gKeyPressSurfaces[i]);
		gKeyPressSurfaces[i] = NULL;
	}
	
	// Destroy window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	// Quit SDL subsystems
	SDL_Quit();
}

// generalized function to load a surface in the future
SDL_Surface* loadSurface(string path) {
	// Load image at specified path
	SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
	if (loadedSurface == NULL) {
		printf("Unable to load image %s! SDL Error: %s", path.c_str(), SDL_GetError());
	}

	return loadedSurface;
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
			// Main loop flag for quitting the application
			bool quit = false;

			// Event handler- it handles events like key presses, mouse motion, joy button presses, etc. 
			SDL_Event e;

			// Set default current surface
			gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];

			// While the application runs; initiating the game loop
			while (!quit) {

				// handles events on the *event queue*. Whenever a button is pressed or a mouse is clicked, the
				// input is added to the queue. This loop will poll the event queue until it is empty and handles
				// all input requests.
				// When we SDL_PollEvent(param), SDL takes the most recent event from the queue (by dequeuing the front)
				// and places it into the passed in param (in this case, at the address of e).
				// When the queue is empty, SDL_PollEvent will return a 0.
				while (SDL_PollEvent(&e) != 0) {
					// User requests to quit by pressing the X button outside the window.
					if (e.type == SDL_QUIT) {
						quit = true;
					}
					// User presses a keyboard key
					else if (e.type == SDL_KEYDOWN) {
						// select surface based on the keypress
						// SDL_Keysym contains the information about the key that was pressed.
						// That SDL_Keysym contains the SDL_Keycode which identifies the key being pressed.
						switch (e.key.keysym.sym) {
							case SDLK_UP:
								gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_UP];
								break;
							case SDLK_DOWN:
								gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
								break;
							case SDLK_LEFT:
								gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
								break;
							case SDLK_RIGHT:
								gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
								break;
							default:
								gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
								break;
						}
					}
				}

				// apply the iamge through blitting
				// Blitting takes a source surface and stamps a copy of it onto the destination surface.
				// The first argument is the source image while the third is the destination.
				SDL_BlitSurface(gCurrentSurface, NULL, gScreenSurface, NULL);

				// Always need to update surface to see the image on the screen
				// When we draw to the surface, we are rendering the frame to the back buffer.
				// What is shown to the user is the front buffer.
				// UdpateWindowSurface() swaps the back and front buffer so we can see the finished frame rather than an unfinished frame as we draw to it.
				// Essentially, we update the window surface after every blit.
				SDL_UpdateWindowSurface(gWindow);
			}
		}
	}

	// Free resources and close SDL
	close();

	return 0;
}