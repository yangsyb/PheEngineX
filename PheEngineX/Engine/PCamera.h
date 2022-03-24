#pragma once
#include "pch.h"
#include "Transform.h"

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
		void SetPosition(const glm::vec3& position);

		void SetRotation(const glm::vec3& rotation);

		void SetDirection(const glm::vec3& front);

		void SetTransform(Transform transform);

		glm::vec3& MoveForward(const glm::vec3& offset);


	public:
		const glm::vec3& GetPosition() const { return PPosistion; }
		const glm::vec3& GetRotation() const { return PRotation; }
		const Transform& GetTransform() const { return PTransform; }
		const glm::mat4& GetProjectionMatrix() const { return PProjection; }
		const glm::mat4& GetProjectionDitherMatrix() const { return PProjectionDither; }
		const glm::mat4& GetViewMatrix() const { return PView; }
		const glm::mat4& GetProjectionViewMatrix() const { return PProjectionView; }

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
		POrthographicCamera(float Width, float Height) : PCamera()
		{
			PWidth = Width;
			PHeight = Height;
			RecalculateProjectionMatrix();
			RecalculateViewMatrix();
		}
		virtual void UpdateZNearFar() override;

	protected:
		virtual glm::mat4 GetPreciseProjectionMatrix() override
		{
			return glm::orthoLH_ZO(-Width, Width, -Width, Width, -100.0f, 100.0f);
		}

		virtual void RecalculateProjectionMatrix() override
		{
			PProjection = glm::orthoLH_ZO(-Width, Width, -Width, Width, -Width, Width);

			PProjection[3].z = -PProjection[3].z;
			PProjection[3].b = -PProjection[3].b;
			PProjection[3].p = -PProjection[3].p;
			PProjectionView = PProjection * PView;
			UpdateProjectionConstant();
		}

		float Width = 30;

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
