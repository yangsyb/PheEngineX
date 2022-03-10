#include "pch.h"
#include "PShaderManager.h"

namespace Phe
{
	PShaderManager::PShaderManager() : CurID(0)
	{
		PropertyID.reserve(64);

		PerObjectBufferID = PropertyToID("PerObjectBuffer");
		PerCameraBufferID = PropertyToID("PerCameraBuffer");
		PerFrameBufferID = PropertyToID("PerFrameBuffer");
		PerMaterialBufferID = PropertyToID("PerMaterialBuffer");
	}


	PShaderManager::~PShaderManager()
	{

	}



	void PShaderManager::AddShader(std::shared_ptr<PShader> Shader)
	{
		ShaderList.push_back(Shader);
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

	void PShaderManager::CompileAllShader()
	{
		for (auto Shader : ShaderList)
		{
			Shader->Initialize();
			Shader->ReflectShader();
		}
	}

	void PShaderManager::CreateUploadBuffer()
	{
		// 		UINT PElementByteSize = (TotalSize + 255) & ~255;
		// 		auto HeapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		// 		auto ResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(size_t(PElementByteSize) * ElementCount);
		// 		GraphicContext::GetSingleton().Device()->CreateCommittedResource(
		// 			&HeapProperty,
		// 			D3D12_HEAP_FLAG_NONE,
		// 			&ResourceDesc,
		// 			D3D12_RESOURCE_STATE_GENERIC_READ,
		// 			nullptr,
		// 			IID_PPV_ARGS(&mUploadBuffer));
		// 
		// 		mUploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&mMappedData));
	}


	std::shared_ptr<PShader> PShaderManager::GetShader(std::string ShaderName)
	{
		for (auto Shader : ShaderList)
		{
			if (Shader->GetName() == ShaderName)
			{
				return Shader;
			}
		}
		return nullptr;
	}

	void PShaderManager::AddConstantSize(std::string ShaderName, std::vector<UINT32> ConstantSize)
	{
		ShaderConstantSizeMap.insert({ ShaderName,ConstantSize });
	}

}


