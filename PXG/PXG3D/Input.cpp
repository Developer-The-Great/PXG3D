#include "Input.h"
#include "KeyCode.h"

#include "Debug.h"

Input::~Input()
{
}

void Input::PollEvents()
{
	glfwPollEvents();
}

Input & Input::GetInstance()
{
	static Input inputInstance;

	return inputInstance;
}

int Input::GetTrackedKeyCount()
{
	return GetInstance().inputStates.size();
}

template<typename T>
inline void Input::AddKeysToTrack(T key)
{
	int keyGiven = static_cast<int>(key);
	Debug::Log(Verbosity::Info,"{0}",keyGiven);

	Input* input = &GetInstance();

	input->inputStates.insert(std::pair<int, InputState>(keyGiven, InputState()));
	}



void Input::key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	UpdateKey(key, action);
}

void Input::mouse_button_callback(GLFWwindow * window, int button, int action, int mods)
{
	UpdateKey(button, action);
}

void Input::mouse_position_callback(GLFWwindow * window, double xpos, double ypos)
{
	GetInstance().x = xpos;
	GetInstance().y = ypos;
}



bool Input::GetKey(KeyCode keyCode)
{
	Input* input = &GetInstance();
	
	std::map<int, InputState>::iterator iter;
	bool keyFound = input->findKey(keyCode, iter);
	
	if (keyFound)
	{
		//Debug::Log(Verbosity::Debug, "KeyFound");
		InputState KeyState = iter->second;
		
		return KeyState.CurrentButtonPressState && KeyState.PreviousButtonPressState;
		
	}
	return false;
}

bool Input::GetKeyDown(KeyCode keyCode)
{
	Input* input = &GetInstance();
	
	std::map<int, InputState>::iterator iter;
	bool keyFound = input->findKey(keyCode, iter);

	if (keyFound)
	{
		InputState KeyState = iter->second;
		//Debug::Log(Verbosity::Debug, "previous : {0} , current : {1}", KeyState.PreviousButtonPressState, KeyState.CurrentButtonPressState);
		return KeyState.CurrentButtonPressState && !KeyState.PreviousButtonPressState;

	}
	return false;
}

bool Input::GetKeyUp(KeyCode keyCode)
{
	Input* input = &GetInstance();

	std::map<int, InputState>::iterator iter;
	bool keyFound = input->findKey(keyCode, iter);

	if (keyFound)
	{
		InputState KeyState = iter->second;

		return !KeyState.CurrentButtonPressState && KeyState.PreviousButtonPressState;

	}
	return false;
}

void Input::LateUpdateTrackedKeyStates()
{
	Input * input = &GetInstance();
	std::map<int, InputState>::iterator iter;

	for (iter = input->inputStates.begin(); iter != input->inputStates.end(); ++iter)
	{
		iter->second.PreviousButtonPressState = iter->second.CurrentButtonPressState;
	}
}

int Input::GetMouseX()
{
	return GetInstance().x;
}

int Input::GetMouseY()
{
	return GetInstance().y;
}

Input::Input()
{
	
}

bool Input::findKey(int key, std::map<int, InputState>::iterator& OUT outIter)
{

	Input* input = &GetInstance();
	std::map<int, InputState>::iterator iter;

	for (iter = input->inputStates.begin(); iter != input->inputStates.end(); ++iter)
	{
		if (iter->first == key)
		{
			outIter = iter;
			return true;
		}
	}
	outIter = iter;
	return false;
}

bool Input::findKey(KeyCode keyCode, std::map<int, InputState>::iterator& OUT outIter)
{
	return findKey(static_cast<int>(keyCode), outIter);
}

void Input::UpdateKey(int key,int action)
{
	Input* input = &GetInstance();

	std::map<int, InputState>::iterator iter;

	bool isKeyFound = false;

	//attempt to find key

	for (iter = input->inputStates.begin(); iter != input->inputStates.end(); ++iter)
	{
		if (iter->first == key)
		{
			isKeyFound = true;
			break;
		}
	}

	//if found
	if (isKeyFound)
	{
		std::map<int, InputState>::iterator iter = input->inputStates.find(key);
		//update data
		if (action == GLFW_PRESS)
		{

			iter->second.CurrentButtonPressState = true;
		}
		else if (action == GLFW_RELEASE)
		{
			iter->second.CurrentButtonPressState = false;
		}

	}
}




template void Input::AddKeysToTrack<KeyCode>(KeyCode key);
template void Input::AddKeysToTrack<int>(int key);

