#pragma once
#include "pch.h"
#include <xlocinfo>
#include <DirectXCollision.h>
#include "glm/ext/matrix_float4x4.hpp"
using namespace Microsoft::WRL;
static glm::mat4 Identity4x4()
{
	static glm::mat4 I(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	return I;
}

static glm::mat4 Test4x4()
{
	static glm::mat4 I(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		5.0f, 2.0f, 5.0f, 1.0f);

	return I;
}


struct ObjectConstants
{
	glm::mat4 Position;
	glm::mat4 Rotation;
	glm::mat4 Scale;
};

struct PassConstants
{
	glm::mat4 View;
	glm::mat4 Proj;
};


struct PVertex
{
	glm::vec3 Pos;
	glm::vec4 Normal;
};

struct PMeshDataStruct
{
	std::vector<float> Vertices;
	std::vector<float> Normal;
	std::vector<UINT32> Indices;
};

struct PTreeConstants
{
	float Time;
	glm::vec3 Center;
};


static UINT CalcConstantBufferByteSize(UINT byteSize)
{
	return (byteSize + 255) & ~255;//..
}