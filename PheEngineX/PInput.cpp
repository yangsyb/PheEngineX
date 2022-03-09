#include "pch.h"
#include "PInput.h"

using namespace std;

std::array<KeyState, (size_t)KeyCode::COUNT> Input::m_KeyStates = {};
std::pair<float, float> Input::PMousePosition = std::pair<float, float>(0, 0);
void Input::Update()
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
	UpdateKeyState(KeyCode::Z, 'Z');
	UpdateKeyState(KeyCode::X, 'X');
	//UpdateKeyState(KeyCode::Mouse0, VK_LBUTTON);
	//UpdateKeyState(KeyCode::Mouse2, VK_RBUTTON);

}

bool Input::GetKey(KeyCode keyCode)
{
	return ((int)m_KeyStates[(int)keyCode] & (int)KeyState::KeyHold) != 0;
}

bool Input::GetKeyUp(KeyCode keyCode)
{
	return ((int)m_KeyStates[(int)keyCode] & (int)KeyState::KeyUp) != 0;
}

bool Input::GetKeyDown(KeyCode keyCode)
{
	return ((int)m_KeyStates[(int)keyCode] & (int)KeyState::KeyDown) != 0;
}

std::pair<float, float> Input::GetMousePosition()
{
	return PMousePosition;
}

void Input::UpdateKeyState(KeyCode keyCode, int windowsKeyCode)
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

void Input::OnMouseDown(KeyCode MouseBtnCode)
{
	m_KeyStates[(int)MouseBtnCode] = KeyState::KeyDown;
}

void Input::OnMouseUp(KeyCode MouseBtnCode)
{
	m_KeyStates[(int)MouseBtnCode] = KeyState::KeyUp;
}

void Input::UpdateMouseState()
{
	if (m_KeyStates[(int)KeyCode::Mouse0] == KeyState::KeyDown)
		m_KeyStates[(int)KeyCode::Mouse0] = KeyState::KeyHold;

	if (m_KeyStates[(int)KeyCode::Mouse2] == KeyState::KeyDown)
		m_KeyStates[(int)KeyCode::Mouse2] = KeyState::KeyHold;
}

void Input::OnMouseMove(int x, int y)
{
	PMousePosition = std::pair<float, float>(x, y);
}
