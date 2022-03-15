#pragma once
#include "pch.h"
#include <xlocinfo>
#include <DirectXCollision.h>
#include "glm/ext/matrix_float4x4.hpp"
using namespace Microsoft::WRL;

struct PerObjectCBuffer
{
	glm::mat4 Position;
	glm::mat4 Rotation;
	glm::mat4 Scale;
};

struct PerCameraCBuffer
{
	glm::mat4 View;
	glm::mat4 Proj;
};

struct PerFrameCBuffer
{
	float Time;
};

struct PerMaterialCBuffer
{
	glm::vec4 DiffuseAlbedo;
	glm::vec3 FresnelR0;
	float Roughness;
};

struct PVertex
{
	glm::vec3 Pos;
	glm::vec4 Normal;
	glm::vec2 TextCoord;
};

struct PMeshDataStruct
{
	std::vector<float> Vertices;
	std::vector<float> Normal;
	std::vector<UINT32> Indices;
	std::vector<float> UVs;
};