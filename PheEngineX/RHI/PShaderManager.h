#pragma once
#include "pch.h"

namespace Phe
{
	class PShaderManager : public Singleton<PShaderManager>
	{
	public:
		PShaderManager();
		~PShaderManager();

		UINT PropertyToID(std::string property);
		UINT GetPerObjectBufferID() { return PerObjectBufferID; }
		UINT GetPerCameraBufferID() { return PerCameraBufferID; }
		UINT GetPerMaterialBuffer() { return PerMaterialBufferID; }
	private:
		UINT CurID;
		std::unordered_map<std::string, UINT> PropertyID;

		UINT PerObjectBufferID;
		UINT PerCameraBufferID;
		UINT PerMaterialBufferID;
	};
}
