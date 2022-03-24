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
	glm::mat4 CameraLocation;
	glm::mat4 ShadowTransform;
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
	std::vector<UINT16> Indices;
	std::vector<float> UVs;
};

struct PTextureDataStruct
{
	std::wstring TFileName;
};

struct PMaterialDataStruct
{
	std::string ShaderName;
	std::vector<std::string> TextureNames;
	glm::vec4 DiffuseAlbedo;
	glm::vec3 FresnelR0;
	float Metallic;
	float Specular;
	float Roughness;
};

struct PLightDataStruct
{
	glm::vec3 Position;
	glm::vec3 Rotation;
	glm::vec3 Scale;
	float LightStrength;
	float LightRadius;
};