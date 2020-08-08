#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include "Generic.h"

#ifdef ERROR_LOGGING
#include <string>
#include <iostream>
#endif

#ifndef INPUT_HANDLED
#include <map>
#include <vector>
#endif

#ifndef SOUND_FREQUENCY
#define SOUND_FREQUENCY (44100.0)
#endif

#ifndef SOUND_BUFFER_SIZE
#define SOUND_BUFFER_SIZE 1000
#endif // !SOUND_BUFFER_SIZE


// SDL-GAME
namespace SDLG
{
	float soundBuffer[SOUND_BUFFER_SIZE];

	typedef Uint32 millitime;

	// Programmer has own game loop
#ifndef LOOP_HANDLED

// Called every frame
	void OnFrame();

#endif //!LOOP_HANDLED

	// Called upon initialisation
	void OnStart();
	// Called upon exit
	void OnQuit();

	static bool gameRunning = true;

	static int windowWidth = SCALING_FACTOR * 20;
	static int windowHeight = SCALING_FACTOR * 19;

	static int windowX = 0;
	static int windowY = 0;

	static SDL_Window* gameWindow = nullptr;
	static SDL_Renderer* gameRenderer = nullptr;
	static Uint32 gameWindowID = 0;
	const static char* windowTitle = "";

	static Uint32 windowFlags = 0
		//| SDL_WINDOW_FULLSCREEN         /**< fullscreen window */
		//| SDL_WINDOW_OPENGL             /**< window usable with OpenGL context */
		//| SDL_WINDOW_SHOWN              /**< window is visible */
		//| SDL_WINDOW_HIDDEN             /**< window is not visible */
		| SDL_WINDOW_BORDERLESS         /**< no window decoration */
		//| SDL_WINDOW_RESIZABLE          /**< window can be resized */
		//| SDL_WINDOW_MINIMIZED          /**< window is minimized */
		//| SDL_WINDOW_MAXIMIZED          /**< window is maximized */
		//| SDL_WINDOW_INPUT_GRABBED      /**< window has grabbed input focus */
		//| SDL_WINDOW_INPUT_FOCUS        /**< window has input focus */
		//| SDL_WINDOW_MOUSE_FOCUS        /**< window has mouse focus */
		//| SDL_WINDOW_FULLSCREEN_DESKTOP
		//| SDL_WINDOW_FOREIGN            /**< window not created by SDL */
		//| SDL_WINDOW_ALLOW_HIGHDPI      /**< window should be created in high-DPI mode if supported.
		//												 On macOS NSHighResolutionCapable must be set true in the
		//												 application's Info.plist for this to have any effect. */
		//| SDL_WINDOW_MOUSE_CAPTURE      /**< window has mouse captured (unrelated to INPUT_GRABBED) */
		| SDL_WINDOW_ALWAYS_ON_TOP      /**< window should always be above others */
		//| SDL_WINDOW_SKIP_TASKBAR      /**< window should not be added to the taskbar */
		//| SDL_WINDOW_UTILITY      /**< window should be treated as a utility window */
		//| SDL_WINDOW_TOOLTIP      /**< window should be treated as a tooltip */
		//| SDL_WINDOW_POPUP_MENU      /**< window should be treated as a popup menu */
		//| SDL_WINDOW_VULKAN
		;
		
	/*
		:SDL_WINDOW_OPENGL,     ::SDL_WINDOW_INPUT_GRABBED,
		::SDL_WINDOW_HIDDEN,     ::SDL_WINDOW_RESIZABLE,
		::SDL_WINDOW_MAXIMIZED,  ::SDL_WINDOW_MINIMIZED,
	*/

	// Programmer handles time stuff
#ifndef TIME_HANDLED
	static millitime currentTime = 0;
	static millitime previousTime = 0;
	static millitime deltaTime = 0;

	static millitime minFrameDelta = 1000 / 60;

	static void HandleTime() {
		previousTime = currentTime;
		currentTime = SDL_GetTicks();
		deltaTime = currentTime - previousTime;

		// FPS is capped when a min delta time is given
		if (minFrameDelta > 0 && deltaTime < minFrameDelta) {
			millitime delayTime = minFrameDelta - deltaTime;

			deltaTime = minFrameDelta;
			currentTime = previousTime + deltaTime;

			SDL_Delay(delayTime);
		}
	}
#endif // !TIME_HANDLED

#ifdef ERROR_LOGGING

	static void MakeLog(std::string message) {
		std::cout << message;
		SDL_Log(message.c_str());
	}

#endif // ERROR_LOGGING

#ifndef INPUT_HANDLED

	static int mouseXPrev;
	static int mouseX;
	static int mouseXDelta;

	static int mouseYPrev;
	static int mouseY;
	static int mouseYDelta;

	static int mouseWheelYPrev;
	static int mouseWheelY;
	static int mouseWheelYDelta;

	static int mouseWheelXPrev;
	static int mouseWheelX;
	static int mouseWheelXDelta;

	struct keystate {
		Uint32 down = 0;
		Uint32 up = 0;
	};

	struct buttonState {
		Uint32 down = 0;
		Sint32 downX = 0, downY = 0;
		Uint32 up = 0;
		Sint32 upX = 0, upY = 0;
	};

	struct keyboardData {
		Uint32 lastUpdated = 0;
		std::map<SDL_Scancode, keystate> last_keys_scancode;
		std::map<SDL_Keycode, keystate> last_keys_keycode;
		std::map<SDL_Scancode, keystate> keys_scancode;
		std::map<SDL_Keycode, keystate> keys_keycode;
	};

	struct mouseData {
		std::map <Uint8, buttonState> last_mouse_buttons;
		std::map <Uint8, buttonState> mouse_buttons;
	};

	static keyboardData globalKeyboard;
	static mouseData globalMouseData;

	typedef Uint32 eventType;

	class EventCallback {
	public:
		bool active = true;
		virtual void Callback(SDL_Event&) = 0;
	};

	static std::map<eventType, std::vector<EventCallback*>> callbacks;

	static void TriggerEventCallbacks(SDL_Event& e) {
		auto it = callbacks.find(e.type);
		if (it == callbacks.end()) return;

		for (auto callback : it->second)
			if (callback->active) callback->Callback(e);
	}

	static bool keyPressed(SDL_Keycode key, keyboardData* pipe = &globalKeyboard) {
		return pipe->keys_keycode[key].down > pipe->last_keys_keycode[key].down;
	}
	static bool keyReleased(SDL_Keycode key, keyboardData* pipe = &globalKeyboard) {
		return pipe->keys_keycode[key].up > pipe->last_keys_keycode[key].up;
	}
	static bool keyDown(SDL_Keycode key, keyboardData* pipe = &globalKeyboard) {
		return pipe->keys_keycode[key].down > pipe->keys_keycode[key].up;
	}
	static bool keyUp(SDL_Keycode key, keyboardData* pipe = &globalKeyboard) {
		return pipe->keys_keycode[key].up > pipe->keys_keycode[key].down;
	}

	static bool scancodePressed(SDL_Scancode key, keyboardData* pipe = &globalKeyboard) {
		return pipe->keys_scancode[key].down > pipe->last_keys_scancode[key].down;
	}
	static bool scancodeReleased(SDL_Scancode key, keyboardData* pipe = &globalKeyboard) {
		return pipe->keys_scancode[key].up > pipe->last_keys_scancode[key].up;
	}
	static bool scancodeDown(SDL_Scancode key, keyboardData* pipe = &globalKeyboard) {
		return pipe->keys_scancode[key].down > pipe->keys_scancode[key].up;
	}
	static bool scancodeUp(SDL_Scancode key, keyboardData* pipe = &globalKeyboard) {
		return pipe->keys_scancode[key].up > pipe->keys_scancode[key].down;
	}

	static bool buttonPressed(Uint8 button) {
		return globalMouseData.mouse_buttons[button].down > globalMouseData.last_mouse_buttons[button].down;
	}
	static bool buttonReleased(Uint8 button) {
		return globalMouseData.mouse_buttons[button].up > globalMouseData.last_mouse_buttons[button].up;
	}
	static bool buttonDown(Uint8 button) {
		return globalMouseData.mouse_buttons[button].down > globalMouseData.mouse_buttons[button].up;
	}
	static bool buttonUp(Uint8 button) {
		return globalMouseData.mouse_buttons[button].up > globalMouseData.mouse_buttons[button].down;
	}

	static void HandleInput() {
		mouseXPrev = mouseX;
		mouseYPrev = mouseY;

		mouseWheelXPrev = mouseWheelX;
		mouseWheelYPrev = mouseWheelY;

		for (const auto& pair : globalMouseData.mouse_buttons)
			globalMouseData.last_mouse_buttons[pair.first] = globalMouseData.mouse_buttons[pair.first];

		for (const auto& pair : globalKeyboard.keys_scancode)
			globalKeyboard.last_keys_scancode[pair.first] = globalKeyboard.keys_scancode[pair.first];

		for (const auto& pair : globalKeyboard.keys_keycode)
			globalKeyboard.last_keys_keycode[pair.first] = globalKeyboard.keys_keycode[pair.first];

		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				gameRunning = false;
				return; // Exit immediately

			case SDL_KEYDOWN:
				if (e.key.windowID != gameWindowID) break;
				globalKeyboard.keys_scancode[e.key.keysym.scancode].down = e.key.timestamp;
				globalKeyboard.keys_keycode[e.key.keysym.sym].down = e.key.timestamp;
				globalKeyboard.lastUpdated = e.key.timestamp;
				break;

			case SDL_KEYUP:
				if (e.key.windowID != gameWindowID) break;
				globalKeyboard.keys_scancode[e.key.keysym.scancode].up = e.key.timestamp;
				globalKeyboard.keys_keycode[e.key.keysym.sym].up = e.key.timestamp;
				globalKeyboard.lastUpdated = e.key.timestamp;
				break;

			case SDL_MOUSEMOTION:
				if (e.motion.windowID != gameWindowID) break;
				mouseX = e.motion.x;
				mouseY = e.motion.y;
				break;

			case SDL_MOUSEBUTTONDOWN:
				if (e.button.windowID != gameWindowID) break;
				globalMouseData.mouse_buttons[e.button.button].down = e.button.timestamp;
				globalMouseData.mouse_buttons[e.button.button].downX = e.button.x;
				globalMouseData.mouse_buttons[e.button.button].downY = e.button.y;
				break;

			case SDL_MOUSEBUTTONUP:
				if (e.button.windowID != gameWindowID) break;
				globalMouseData.mouse_buttons[e.button.button].up = e.button.timestamp;
				globalMouseData.mouse_buttons[e.button.button].upX = e.button.x;
				globalMouseData.mouse_buttons[e.button.button].upY = e.button.y;
				break;

			case SDL_MOUSEWHEEL:
				if (e.wheel.windowID != gameWindowID) break;
				if (e.wheel.direction == SDL_MOUSEWHEEL_FLIPPED) {
					mouseWheelX -= e.wheel.x;
					mouseWheelY -= e.wheel.y;
				}
				else {
					mouseWheelX += e.wheel.x;
					mouseWheelY += e.wheel.y;
				}
				break;

			case SDL_WINDOWEVENT:
				if (e.window.windowID != gameWindowID) break;
				switch (e.window.event)
				{
				case SDL_WINDOWEVENT_SIZE_CHANGED:
				case SDL_WINDOWEVENT_RESIZED:
					windowWidth = e.window.data1;
					windowHeight = e.window.data2;
					break;

				case SDL_WINDOWEVENT_CLOSE:
					e.type = SDL_QUIT;
					SDL_PushEvent(&e);
					break;
				}
			}

			TriggerEventCallbacks(e);
		}

		SDL_GetGlobalMouseState(&mouseX, &mouseY);

		SDL_GetWindowPosition(gameWindow, &windowX, &windowY);

		mouseX -= windowX;
		mouseY -= windowY;

		mouseXDelta = mouseX - mouseXPrev;
		mouseYDelta = mouseY - mouseYPrev;
		mouseWheelXDelta = mouseWheelX - mouseWheelXPrev;
		mouseWheelYDelta = mouseWheelY - mouseWheelYPrev;
	}
#endif // !INPUT_HANDLED

	void AudioCallback(void* userdata, Uint8* stream, int len);

	static void CleanupSDL() {
		if (gameWindow != nullptr) SDL_DestroyWindow(gameWindow);
		if (gameRenderer != nullptr) SDL_DestroyRenderer(gameRenderer);
		gameWindow = nullptr;
		gameRenderer = nullptr;

		if (SDL_WasInit(NULL)) SDL_Quit();
	}

	static int StartSDL() {

		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
#ifdef ERROR_LOGGING
			MakeLog("Unable to initialise SDL: " + std::string(SDL_GetError()));
#endif // !ERROR_LOGGING
			CleanupSDL();
			return 1;
		}
		
		if ((gameWindow = SDL_CreateShapedWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, windowFlags)) == NULL) {
		//if ((gameWindow = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, windowFlags)) == NULL) {
#ifdef ERROR_LOGGING
			MakeLog("Unable to create window: " + std::string(SDL_GetError()));
#endif // !ERROR_LOGGING
			CleanupSDL();
			return 2;
		}

		if ((gameRenderer = SDL_CreateRenderer(gameWindow, -1, 0)) == NULL) {
#ifdef ERROR_LOGGING
			MakeLog("Unable to create renderer: " + std::string(SDL_GetError()));
#endif // !ERROR_LOGGING
			CleanupSDL();
			return 3;
		}

		SDL_AudioSpec config;
		config.freq = (int)SOUND_FREQUENCY;
		config.format = AUDIO_F32;
		config.channels = 1;
		config.callback = AudioCallback;
		config.samples = SOUND_BUFFER_SIZE;

		if (SDL_OpenAudio(&config, NULL) < 0) {
			fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
			exit(-1);
		}

		gameWindowID = SDL_GetWindowID(gameWindow);

		OnStart();

		currentTime = SDL_GetTicks();
		deltaTime = currentTime;

#ifndef LOOP_HANDLED
		while (gameRunning) {

			HandleInput();

			OnFrame();

#ifndef TIME_HANDLED
			HandleTime();
#endif // !TIME_HANDLED

		}
#endif // !LOOP_HANDLED

		OnQuit();

		CleanupSDL();

		return 0;
	}

}