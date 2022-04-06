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
	PerObjectCBuffer() : Position(glm::mat4()), Rotation(glm::mat4()), Scale(glm::mat4())
	{}
	PerObjectCBuffer(glm::mat4 P, glm::mat4 R, glm::mat4 S) :
		Position(P), Rotation(R), Scale(S)
	{}
};

struct PerCameraCBuffer
{
	glm::mat4 View;
	glm::mat4 Proj;
	glm::mat4 ShadowTransform;
	glm::vec3 CameraLocation;
	float Time;
};

struct PerMaterialCBuffer
{
	glm::vec4 BaseColor;
	glm::vec3 FresnelR0;
	float Roughness;
};

struct PerLightCBuffer
{
	glm::vec3 LightDirection;
	float LightIntensity;
	float LightRadius;
	PerLightCBuffer() : 
		LightDirection(glm::vec3()), LightIntensity(0), LightRadius(0)
	{}
	PerLightCBuffer(glm::vec3 LD, float LI, float LR) : 
		LightDirection(LD), LightIntensity(LI), LightRadius(LR)
	{}
};

struct PVertex
{
	glm::vec3 Pos;
	glm::vec4 Normal;
	glm::vec4 Tangent;
	glm::vec2 TextCoord;
};

struct PMeshDataStruct
{
	std::vector<float> Vertices;
	std::vector<UINT16> Indices;
	std::vector<float> Normal;
	std::vector<float> Tangent;
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