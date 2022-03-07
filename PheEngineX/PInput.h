#pragma once
#include <array>

enum class KeyCode
{
	None,
	Backspace,
	Tab,
	Clear,
	Return,
	Pause,
	Escape,
	Space,
	A,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z,
	Keypad0,
	Keypad1,
	Keypad2,
	Keypad3,
	Keypad4,
	Keypad5,
	Keypad6,
	Keypad7,
	Keypad8,
	Keypad9,
	UpArrow,
	DownArrow,
	RightArrow,
	LeftArrow,
	Mouse0,		// Left Mouse Button
	Mouse1,		// Middle Mouse Button
	Mouse2,		// Right Mouse Button
	COUNT
};

enum class KeyState
{
	None = 0,
	KeyDown = 1,
	KeyHold = 2,
	KeyUp = 4
};

class Input
{
public:
	static void Update();
	static bool GetKey(KeyCode keyCode);
	static bool GetKeyUp(KeyCode keyCode);
	static bool GetKeyDown(KeyCode keyCode);
	static std::pair<float, float> GetMousePosition();
	static void OnMouseMove(int x, int y);
	static void OnMouseDown(KeyCode MouseBtnCode);
	static void OnMouseUp(KeyCode MouseBtnCode);


private:
	static void UpdateKeyState(KeyCode keyCode, int windowsKeyCode);
	static void UpdateMouseState();

private:
	static std::array<KeyState, (size_t)KeyCode::COUNT> m_KeyStates;
	static std::pair<float, float> PMousePosition;
};