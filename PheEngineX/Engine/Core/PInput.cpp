#include "pch.h"
#include "PInput.h"

using namespace std;


Input* Input::Create()
{
	Input* input = nullptr;
#ifdef PlatformWin32
	input = new PInputWin32();
#elif defined (PlatformIOS)
	input = new PInputIOS();
#elif defined (PlatformAndroid)
	input = new PInputAndroid();
#endif
	return input;
}

#ifdef PlatformWin32
PInputWin32::PInputWin32() : m_KeyStates{}, PMousePosition(std::pair<float, float>(0, 0))
{

}

void PInputWin32::Update()
{
	UpdateKeyState(KeyCode::W, 'W');
	UpdateKeyState(KeyCode::A, 'A');
	UpdateKeyState(KeyCode::S, 'S');
	UpdateKeyState(KeyCode::D, 'D');
	UpdateKeyState(KeyCode::Q, 'Q');
	UpdateKeyState(KeyCode::E, 'E');
	UpdateKeyState(KeyCode::P, 'P');
	UpdateKeyState(KeyCode::Escape, VK_ESCAPE);
	UpdateKeyState(KeyCode::UpArrow, VK_UP);
	UpdateKeyState(KeyCode::DownArrow, VK_DOWN);
	UpdateKeyState(KeyCode::LeftArrow, VK_LEFT);
	UpdateKeyState(KeyCode::RightArrow, VK_RIGHT);
	UpdateKeyState(KeyCode::Keypad1, '1');
	UpdateKeyState(KeyCode::Keypad2, '2');
	UpdateKeyState(KeyCode::Keypad3, '3');
	UpdateKeyState(KeyCode::Keypad4, '4');
	UpdateKeyState(KeyCode::Keypad5, '5');
	UpdateKeyState(KeyCode::Z, 'Z');
	UpdateKeyState(KeyCode::X, 'X');
	UpdateKeyState(KeyCode::J, 'J');
}

bool PInputWin32::GetKey(KeyCode keyCode)
{
	return ((int)m_KeyStates[(int)keyCode] & (int)KeyState::KeyHold) != 0;
}

bool PInputWin32::GetKeyUp(KeyCode keyCode)
{
	return ((int)m_KeyStates[(int)keyCode] & (int)KeyState::KeyUp) != 0;
}

bool PInputWin32::GetKeyDown(KeyCode keyCode)
{
	return ((int)m_KeyStates[(int)keyCode] & (int)KeyState::KeyDown) != 0;
}

std::pair<float, float> PInputWin32::GetMousePosition()
{
	return PMousePosition;
}

void PInputWin32::UpdateKeyState(KeyCode keyCode, int windowsKeyCode)
{
	bool isKeyDown = GetAsyncKeyState(windowsKeyCode) & 0x0001;
	bool isKeyHold = GetAsyncKeyState(windowsKeyCode) & 0x8000;

	if (isKeyDown)
	{
		m_KeyStates[(int)keyCode] = KeyState::KeyDown;
	}
	else if (isKeyHold)
	{
		m_KeyStates[(int)keyCode] = KeyState::KeyHold;
	}
	else
	{
		m_KeyStates[(int)keyCode] = KeyState::KeyUp;
	}
}

void PInputWin32::OnMouseDown(KeyCode MouseBtnCode)
{
	m_KeyStates[(int)MouseBtnCode] = KeyState::KeyDown;
}

void PInputWin32::OnMouseUp(KeyCode MouseBtnCode)
{
	m_KeyStates[(int)MouseBtnCode] = KeyState::KeyUp;
}

void PInputWin32::UpdateMouseState()
{
	if (m_KeyStates[(int)KeyCode::Mouse0] == KeyState::KeyDown)
		m_KeyStates[(int)KeyCode::Mouse0] = KeyState::KeyHold;

	if (m_KeyStates[(int)KeyCode::Mouse2] == KeyState::KeyDown)
		m_KeyStates[(int)KeyCode::Mouse2] = KeyState::KeyHold;
}

void PInputWin32::OnMouseMove(int x, int y)
{
	PMousePosition = std::pair<float, float>(float(x), float(y));
}
#endif