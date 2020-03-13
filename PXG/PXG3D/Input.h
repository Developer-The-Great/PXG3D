#pragma once


#include <memory>
#include <vector>
#include <map>
#include "Utils.h"



struct InputState
{
	bool PreviousButtonPressState;
	bool CurrentButtonPressState;
};

struct GLFWwindow;
enum class KeyCode;

class Input
{

public:
	~Input();

	static void PollEvents();

	static Input& GetInstance();

	static int GetTrackedKeyCount();

	//static void AddKeysToTrack();
	template<typename T> static void AddKeysToTrack(T key);

	template<typename T, typename ...Args> static void AddKeysToTrack(T key, Args ...args)
	{
		AddKeysToTrack(key);
		AddKeysToTrack(args...);
	}

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

	static void mouse_position_callback(GLFWwindow* window, double xpos, double ypos);

	static bool GetKey(KeyCode keyCode);

	static bool GetKeyDown(KeyCode keyCode);

	static bool GetKeyUp(KeyCode keyCode);

	static void LateUpdateTrackedKeyStates();

	static int GetMouseX();

	static int GetMouseY();

private:

	Input();

	std::map<int, InputState> inputStates;

	bool findKey(int key, std::map<int, InputState>::iterator& OUT outIter);

	bool findKey(KeyCode keyCode, std::map<int, InputState>::iterator& OUT outIter);

	static void UpdateKey(int key, int action);

	float x, y;
};




