#pragma once
#include "pch.h"
#include "array"
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
	Add,
	Minus,
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
	static Input* Create();
	virtual void Update() = 0;
	virtual bool GetKey(KeyCode keyCode) = 0;
	virtual bool GetKeyUp(KeyCode keyCode) = 0;
	virtual bool GetKeyDown(KeyCode keyCode) = 0;
	virtual std::pair<float, float> GetMousePosition() = 0;
	virtual void OnMouseMove(int x, int y) = 0;
	virtual void OnMouseDown(KeyCode MouseBtnCode) = 0;
	virtual void OnMouseUp(KeyCode MouseBtnCode) = 0;


protected:
	virtual void UpdateKeyState(KeyCode keyCode, int windowsKeyCode) = 0;
	virtual void UpdateMouseState() = 0;
};


#ifdef PlatformWin32
class PInputWin32 : public Input
{
public:
	PInputWin32();
	~PInputWin32() = default;
	virtual void Update() override;
	virtual bool GetKey(KeyCode keyCode) override;
	virtual bool GetKeyUp(KeyCode keyCode) override;
	virtual bool GetKeyDown(KeyCode keyCode) override;
	virtual std::pair<float, float> GetMousePosition() override;
	virtual void OnMouseMove(int x, int y) override;
	virtual void OnMouseDown(KeyCode MouseBtnCode) override;
	virtual void OnMouseUp(KeyCode MouseBtnCode) override;

protected:
	virtual void UpdateKeyState(KeyCode keyCode, int windowsKeyCode) override;
	virtual void UpdateMouseState() override;

protected:
	std::array<KeyState, (size_t)KeyCode::COUNT> m_KeyStates;
	std::pair<float, float> PMousePosition;
};

#elif defined(PlatformIOS)
class PInputIOS : public Input
{
public:
	
};

#elif defined(PlatformAndroid)
class PInputAndroid : public Input
{
public:

};

#endif
