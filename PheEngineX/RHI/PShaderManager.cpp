#include "pch.h"
#include "PShaderManager.h"

namespace Phe
{
	PShaderManager::PShaderManager() : CurID(0)
	{
		PropertyID.reserve(64);

		PerObjectBufferID = PropertyToID("PerObjectBuffer");
		PerCameraBufferID = PropertyToID("PerCameraBuffer");
		PerMaterialBufferID = PropertyToID("PerMaterialBuffer");
	}

	
	PShaderManager::~PShaderManager()
	{

	}



	UINT PShaderManager::PropertyToID(std::string property)
	{
		auto&& IDIterator = PropertyID.find(property);
		if (IDIterator == PropertyID.end())
		{
			unsigned int value = CurID;
			PropertyID[property] = CurID;
			++CurID;
			return value;
		}

		return IDIterator->second;
	}
}


