#include "pch.h"
#include "PCameraController.h"
#include "PInput.h"

namespace Phe
{
	PCameraController::PCameraController(std::shared_ptr<PCamera> camera) : PMainCamera(camera), PInterpolatingState()
	{
		PTargetState.SetFromTransform(PMainCamera->GetTransform());
	}

	float Lerp(float a, float b, float t)
	{
		return a * (1 - t) + b * t;
	}

	void PCameraController::OnUpdate()
	{
		static float prev_x = 0, prev_y = 0;
		static bool prev_IsMoving = false;
		bool isMoving = false;
		if (Input::GetKeyDown(KeyCode::Mouse0))
		{
			isMoving = true;
			if (prev_IsMoving == false)
			{
				prev_IsMoving = true;
				std::pair<float, float> pos = Input::GetMousePosition();
				prev_x = pos.first; prev_y = pos.second;
			}
			else
			{
				std::pair<float, float> pos = Input::GetMousePosition();
				float diffx = pos.first - prev_x;
				float diffy = pos.second - prev_y;
				glm::vec3 currRotation = PMainCamera->GetRotation();

				PTargetState.pitch += -PRotateSpeed * 400 * 0.0005f * diffy;
				PTargetState.yaw += +PRotateSpeed * 400 * 0.0005f * diffx;
				prev_x = pos.first; prev_y = pos.second;
			}
			if (Input::GetKey(KeyCode::W))
			{
				PTargetState.Translate(PMainCamera->MoveForward({ PSpeed * 2 * 0.01f, 0.0f, 0.0f }));
			}
			if (Input::GetKey(KeyCode::A))
			{
				PTargetState.Translate(PMainCamera->MoveForward({ 0.0f, -PSpeed * 2 * 0.01f, 0.0f }));
			}
			if (Input::GetKey(KeyCode::S))
			{
				PTargetState.Translate(PMainCamera->MoveForward({ -PSpeed * 2 * 0.01f, 0.0f, 0.0f }));
			}
			if (Input::GetKey(KeyCode::D))
			{
				PTargetState.Translate(PMainCamera->MoveForward({ 0.0f, PSpeed * 2 * 0.01f, 0.0f }));
			}
		}
		else
		{
			prev_IsMoving = false;
		}

		if (Input::GetKey(KeyCode::Z))
		{
			if (PSpeed < 10)
			{
				PSpeed = PSpeed + 1;
			}
		}
		if (Input::GetKey(KeyCode::X))
		{
			if (PSpeed > 1)
			{
				PSpeed = PSpeed - 1;
			}
		}


		float positionLerpPct = 1.f - exp((log(1.f - 0.99f) / PPositionLerpTime) * 0.01f);
		float rotationLerpPct = 1.f - exp((log(1.f - 0.99f) / PRotationLerpTime) * 0.01f);
		PInterpolatingState.LerpTowards(PTargetState, positionLerpPct, rotationLerpPct);
		PMainCamera->SetTransform(PInterpolatingState.AsTransform());
	}

	void PCameraController::CameraState::SetFromTransform(Transform t)
	{
		roll = t.EulerAngles.x;
		pitch = t.EulerAngles.y;
		yaw = t.EulerAngles.z;
		x = t.Translation.x;
		y = t.Translation.y;
		z = t.Translation.z;
	}

	void PCameraController::CameraState::Translate(glm::vec3 translation)
	{
		x += translation.x;
		y += translation.y;
		z += translation.z;
	}

	void PCameraController::CameraState::LerpTowards(CameraState target, float positionLerpPct, float rotationLerpPct)
	{
		yaw = Lerp(yaw, target.yaw, rotationLerpPct);
		pitch = Lerp(pitch, target.pitch, rotationLerpPct);
		roll = Lerp(roll, target.roll, rotationLerpPct);

		x = Lerp(x, target.x, positionLerpPct);
		y = Lerp(y, target.y, positionLerpPct);
		z = Lerp(z, target.z, positionLerpPct);
	}

	void PCameraController::CameraState::UpdateTransform(Transform t)
	{
		t.EulerAngles = glm::vec3(pitch, yaw, roll);
		t.Translation = glm::vec3(x, y, z);
	}

	Transform PCameraController::CameraState::AsTransform()
	{
		Transform t;
		t.EulerAngles = glm::vec3(roll, pitch, yaw);
		t.Translation = glm::vec3(x, y, z);
		return t;
	}
}




