#pragma once
#include "pch.h"

struct Transform
{
public:
	glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
	glm::vec3 EulerAngles = { 0.0f, 0.0f, 0.0f };
	glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

	Transform(const Transform&) = default;
	Transform(const glm::vec3& translation = { 0,0,0 }, const glm::vec3& rotate = { 0,0,0 }, const glm::vec3& scale = { 1,1,1 }) :
		Translation(translation), EulerAngles(rotate), Scale(scale) {}

	glm::mat4 GetPositionMat()
	{
		return glm::translate(glm::mat4(1.0f), Translation);
	}

	glm::mat4 GetRotaionMat()
	{
		glm::qua<float> qrotation = glm::qua<float>(glm::radians(-EulerAngles));
		glm::mat4 rotation = glm::toMat4(qrotation);
		return rotation;
	}

	glm::mat4 GetScaleMat()
	{
		return glm::scale(glm::mat4(1.0f), Scale);
	}

	glm::mat4 GetTransform() const
	{
		//		glm::mat4 rotation = glm::toMat4(glm::quat(EulerAngles));
		glm::qua<float> qrotation = glm::qua<float>(glm::radians(-EulerAngles));
		glm::mat4 rotation = glm::toMat4(qrotation);
		return glm::translate(glm::mat4(1.0f), Translation) * rotation * glm::scale(glm::mat4(1.0f), Scale);
	}

	PerObjectCBuffer GetBufferData()
	{
		PerObjectCBuffer ret;
		ret.Position = GetPositionMat();
		ret.Rotation = GetRotaionMat();
		ret.Scale = GetScaleMat();
		return ret;
	}
};