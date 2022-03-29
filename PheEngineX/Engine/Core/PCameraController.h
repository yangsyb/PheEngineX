#pragma once
#include "Engine/Scene/PCamera.h"

namespace Phe
{
	class PCameraController
	{
	public:
		PCameraController() = default;
		PCameraController(std::shared_ptr<PCamera> camera);

		void OnUpdate();

	private:
		struct CameraState
		{
			float roll;
			float pitch;
			float yaw;
			float x;
			float y;
			float z;

			void SetFromTransform(Transform t);
			void Translate(glm::vec3 translation);
			void LerpTowards(CameraState target, float positionLerpPct, float rotationLerpPct);
			void UpdateTransform(Transform t);
			Transform AsTransform();
		};

	private:
	private:
		std::shared_ptr<PCamera> PMainCamera = nullptr;
		float PBoost = 3.5f;
		float PSpeed = 10.f;
		float PRotateSpeed = 0.4f;
		float PPositionLerpTime = 0.2f;
		float PRotationLerpTime = 0.01f;

		CameraState PTargetState;
		CameraState PInterpolatingState;
	};
}