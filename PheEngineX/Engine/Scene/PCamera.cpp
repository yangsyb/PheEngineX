#include "pch.h"
#include "PCamera.h"

namespace Phe
{
	PCamera::PCamera()
	{
		PTransform = Transform();
		PWorldUp = PUp = glm::vec3(0.0f, 0.0f, 1.0f);
		SetPosition(glm::vec3(-70.0f, 0.0f, 0.0f));
		SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
        PNormalizeMat = 
        {
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f
        };

	}

 	void PCamera::RecalculateViewMatrix()
 	{
 		glm::vec3 center = PPosistion + PFront;
 		PView = glm::lookAtLH(PPosistion, center, PUp);
 		PProjectionView = PProjection * PView;
 
 		UpdateViewConstant();
 	}

 	void PCamera::UpdateViewConstant()
 	{
 		PMainPassCB.View = PView;
 		PMainPassCB.CameraLocation = PTransform.GetPosition();
//        PMainPassCB.CameraLocationMat = PTransform.GetPositionMat();

        NormalizedPProjectionView = PNormalizeMat * PProjectionView;
        PMainPassCB.ShadowTransform = NormalizedPProjectionView;
 	}
 
 	void PCamera::UpdateProjectionConstant()
 	{
 		PMainPassCB.Proj = PProjection;
 		PMainPassCB.CameraLocation = PTransform.GetPosition();
//        PMainPassCB.CameraLocationMat = PTransform.GetPositionMat();

        NormalizedPProjectionView = PNormalizeMat * PProjectionView;
        PMainPassCB.ShadowTransform = NormalizedPProjectionView;
 	}
 
 	void PCamera::SetPosition(const glm::vec3& position)
 	{
 		PTransform.Translation = position;
 		PPosistion = position;
 		RecalculateViewMatrix();
 	}
 
 	void PCamera::SetRotation(const glm::vec3& rotation)
 	{
 		PRotation = rotation;
 		PTransform.EulerAngles = rotation;
 		PFront.x = cos(glm::radians(rotation.z)) * cos(glm::radians(rotation.y));
 		PFront.y = sin(glm::radians(rotation.z)) * cos(glm::radians(rotation.y));
 		PFront.z = sin(glm::radians(rotation.y));
 		PFront = glm::normalize(PFront);
 
 		if (PRotation.y < -89) PRotation.y = -89;
 		else if (PRotation.y > 89) PRotation.y = 89;
 		PRight = glm::normalize(glm::cross(PWorldUp, PFront));
 		PUp = glm::normalize(glm::cross(PFront, PRight));
 		RecalculateViewMatrix();
 	}
 
 	void PCamera::SetDirection(const glm::vec3& front)
 	{
 		PFront = front;
 		PFront = glm::normalize(PFront);
 		PRight = glm::normalize(glm::cross(PWorldUp, PFront));
 		PUp = glm::normalize(glm::cross(PFront, PRight));
 		RecalculateViewMatrix();
 	}
 
 	void PCamera::SetTransform(Transform transform)
 	{
 		SetPosition(transform.Translation);
 		SetRotation(transform.EulerAngles);
 
 	}
 
 	glm::vec3& PCamera::MoveForward(const glm::vec3& offset)
 	{
 		glm::vec3 res = { 0,0,0 };
 		res += PRight * offset.y;
 		res += PFront * offset.x;
 		res += PUp * offset.z;
 
 		return res;
 	}

	void POrthographicCamera::UpdateZNearFar()
	{

	}

	void PPerspectiveCamera::UpdateZNearFar()
	{

	}

}