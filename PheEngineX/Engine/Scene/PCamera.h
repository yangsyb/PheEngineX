#pragma once
#include "pch.h"
#include "Engine/Core/Transform.h"

namespace Phe
{
	class PCamera
	{
	public:
//		PCamera(float fov, float width, float height);
		PCamera();
		virtual ~PCamera() = default;

		virtual void RecalculateViewMatrix();
		virtual void RecalculateProjectionMatrix() = 0;
		virtual void UpdateZNearFar() = 0;
		virtual glm::mat4 GetPreciseProjectionMatrix() = 0;

		void UpdateViewConstant();
		void UpdateProjectionConstant();

		PerCameraCBuffer GetPassConstant() { return PMainPassCB; }
	public:
		virtual void SetPosition(const glm::vec3 position);

		virtual void SetRotation(const glm::vec3 rotation);

		virtual void SetDirection(const glm::vec3 front);

		virtual void SetTransform(Transform transform);

		virtual glm::vec3& MoveForward(const glm::vec3 offset);


	public:
		virtual const glm::vec3 GetPosition() const { return PPosistion; }
		virtual const glm::vec3 GetRotation() const { return PRotation; }
		virtual const Transform GetTransform() const { return PTransform; }
		virtual const glm::mat4 GetProjectionMatrix() const { return PProjection; }
		virtual const glm::mat4 GetProjectionDitherMatrix() const { return PProjectionDither; }
		virtual const glm::mat4 GetViewMatrix() const { return PView; }
		virtual const glm::mat4 GetProjectionViewMatrix() const { return PProjectionView; }

		virtual const glm::vec3 GetFront() const { return PFront; }

	protected:
		float PWidth;
		float PHeight;

		glm::vec3 PPosistion;
		glm::vec3 PDirection;
		glm::vec3 PRotation;

		glm::vec3 PFront;
		glm::vec3 PUp;
		glm::vec3 PWorldUp;
		glm::vec3 PRight;

		glm::mat4 PProjection;
		glm::mat4 PProjectionDither;
		glm::mat4 PView;
		glm::mat4 PProjectionView;
		glm::mat4 NormalizedPProjectionView;
		glm::mat4 PPreviousProjectionView;
		glm::mat4 PCurrentProjectionView;

		glm::mat4 PNormalizeMat;

		Transform PTransform;

		PerCameraCBuffer PMainPassCB;
	};


	class POrthographicCamera : public PCamera
	{
	public:
	public:
		POrthographicCamera(float Width, float Height) : PCamera()
		{
			PWidth = Width;
			PHeight = Height;
			RecalculateProjectionMatrix();
			RecalculateViewMatrix();
		}
		virtual void UpdateZNearFar() override;

		void ResetOrtho(float pleft, float pright, float pbottom, float ptop, float pnear, float pfar)
		{
			Left = pleft;
			Right = pright;
			Bottom = pbottom;
			Top = ptop;
			Near = pnear;
			Far = pfar;

			RecalculateProjectionMatrix();
			RecalculateViewMatrix();
		}

		void RecalculateOrtho(glm::vec3 SceneCenter, float SceneRadius)
		{
			glm::mat4 PDesiredView = glm::lookAtLH(PPosistion, SceneCenter, PUp);
			glm::vec4 PSceneSphere = VectorToMat(SceneCenter, PDesiredView);
// 			Near = PSceneSphere.x - SceneRadius;
// 			Far = PSceneSphere.x + SceneRadius;
			Left = PSceneSphere.x - SceneRadius;
			Right = PSceneSphere.x + SceneRadius;
			Bottom = PSceneSphere.y - SceneRadius;
			Top = PSceneSphere.y + SceneRadius;
			Near = PSceneSphere.z - SceneRadius;
			Far = PSceneSphere.z + SceneRadius;

			RecalculateProjectionMatrix();
			RecalculateViewMatrix();
		}

	protected:
		virtual glm::mat4 GetPreciseProjectionMatrix() override
		{
			return glm::orthoLH_ZO(-Width, Width, -Width, Width, -100.0f, 100.0f);
		}

		virtual void RecalculateProjectionMatrix() override
		{
			PProjection = glm::orthoLH_ZO(Left, Right, Bottom, Top, Near, Far);

			//PProjection[3].z = -PProjection[3].z;
			//PProjection[3].b = -PProjection[3].b;
			//PProjection[3].p = -PProjection[3].p;
			PProjectionView = PProjection * PView;
			UpdateProjectionConstant();
		}

		float Width = 30;
		float Left = -30;
		float Right = 30;
		float Top = 30;
		float Bottom = -30;
		float Near = -30;
		float Far = 30;
	};

	class PPerspectiveCamera : public PCamera
	{
	public:
		PPerspectiveCamera(float Fov, float Width, float Height) : PFov(Fov), PCamera()
		{
			PWidth = Width;
			PHeight = Height;
			RecalculateProjectionMatrix();
			RecalculateViewMatrix();
		}

		glm::mat4 AsOrthoProjection(glm::vec3 SceneCenter, float SceneRadius);
	protected:
		virtual void RecalculateProjectionMatrix() override
		{
			PProjection = glm::perspectiveLH_ZO(glm::radians(PFov), PWidth / PHeight, 1.f, 3000.0f);
			PProjectionView = PProjection * PView;
			UpdateProjectionConstant();
		}
		virtual void UpdateZNearFar() override;
		virtual glm::mat4 GetPreciseProjectionMatrix() override
		{
			return glm::perspectiveLH_ZO(glm::radians(PFov), PWidth / PHeight, 1.f, 3000.0f);
		}


	private:
		float PFov;
	};
}
