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

// Current displayed image
SDL_Surface* gStretchedSurface = NULL;

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
	gStretchedSurface = loadSurface("Images/stretch.bmp");
	if (gStretchedSurface == NULL)
	{
		printf("Failed to load image!\n");
		success = false;
	}

	return success;
}


void close() {
	// Deallocate surface
	SDL_FreeSurface(gStretchedSurface);
	gStretchedSurface = NULL;
		
	// Destroy window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	// Quit SDL subsystems
	SDL_Quit();
}

// Beforehand, we have been blitting the images raw, which causes needless slow down.
// This function will not convert the image to an optimized format to speed them up.
// Normally, when we load an image, it's loaded in a 24-bit format since bitmaps are
// 24-bit. However, if we blit that to a 32-bit image, SDL will convert it every single time the image is blitted.
// Using SDL_ConvertSurface, we can pass in the format of the surface we want the imaged surface to convert to.
SDL_Surface* loadSurface(string path) {

	// The final optimized image
	SDL_Surface* optimizedSurface = NULL;

	// Load image at specified path
	SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
	if (loadedSurface == NULL) {
		printf("Unable to load image %s! SDL Error: %s", path.c_str(), SDL_GetError());
	}
	else {
		// Convert surface to screen format
		optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, NULL);
		if (optimizedSurface == NULL) {
			printf("Unable to optimze image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		// Get rid of old loaded surface because it is still in memory
		SDL_FreeSurface(loadedSurface);
	}

	return optimizedSurface;
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
				}
				
				// Apply the image stretched rather than raw to optimize load time.
				SDL_Rect stretchRect;
				stretchRect.x = 0;
				stretchRect.y = 0;
				stretchRect.w = SCREEN_WIDTH;
				stretchRect.h = SCREEN_HEIGHT;
				// blitting the reformated surface using SDL_BlitScaled
				// The first parameter is the surface being blitted
				// The third parameter is the destination surface of the blit
				// The fourth parameter is the destination rectangle of being reformatted
				SDL_BlitScaled(gStretchedSurface, NULL, gScreenSurface, &stretchRect);

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