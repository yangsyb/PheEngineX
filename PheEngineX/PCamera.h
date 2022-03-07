#pragma once
#include "pch.h"
#include "Transform.h"

namespace Phe
{
	class PCamera
	{
	public:
		PCamera(float fov, float width, float height);
		virtual ~PCamera() = default;

		void RecalculateViewMatrix();
		void RecalculateProjectionMatrix();

		void UpdateViewConstant();
		void UpdateProjectionConstant();

		PassConstants GetPassConstant() { return PMainPassCB; }
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
		float PFov;

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
		glm::mat4 PPreviousProjectionView;
		glm::mat4 PCurrentProjectionView;

		Transform PTransform;

		PassConstants PMainPassCB;
	};
}
