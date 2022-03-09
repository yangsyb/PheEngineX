#include "pch.h"
#include "PShader.h"
#include "PShaderManager.h"
#include "DxException.h"
#include "GraphicsContext.h"

namespace Phe
{
	static ComPtr<ID3DBlob> CompileShader(const std::wstring& Filename, const D3D_SHADER_MACRO* Defines, const std::string& EntryPoint, const std::string& Target)
	{
		ComPtr<ID3DBlob> ByteCode;
		ComPtr<ID3DBlob> Errors;
		D3DCompileFromFile(Filename.c_str(), Defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
			EntryPoint.c_str(), Target.c_str(), 0, 0, &ByteCode, &Errors);
		return ByteCode;
	}

	PShader::PShader(const std::wstring FilePath, std::string VS, std::string PS) : PFilePath(FilePath), PVSEntry(VS), PPSEntry(PS)
	{
		ShaderParameter perObject("cbPerObject", ShaderParamType::CBVDescriptorHeap, 1, 0, 0);
		ParamMap[PShaderManager::GetSingleton().PropertyToID("PerObjectBuffer")] = UINT(Params.size());
		Params.push_back(perObject);

		ShaderParameter perPass("cbPerPass", ShaderParamType::CBVDescriptorHeap, 1, 1, 0);
		ParamMap[PShaderManager::GetSingleton().PropertyToID("PerCameraBuffer")] = UINT(Params.size());
		Params.push_back(perPass);

		PRasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		PBlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		PDepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	}

	PShader::~PShader()
	{

	}

	void PShader::Register()
	{
		PShaderManager::GetSingleton().AddShader(shared_from_this());
	}

	void PShader::Initialize()
	{
		PVS = CompileShader(PFilePath, nullptr, PVSEntry, "vs_5_0");
		PPS = CompileShader(PFilePath, nullptr, PPSEntry, "ps_5_0");

		std::vector<CD3DX12_ROOT_PARAMETER> rootParameters;
		std::vector<CD3DX12_DESCRIPTOR_RANGE> texTables;
		std::vector<CD3DX12_DESCRIPTOR_RANGE> constantTables;

		constantTables.reserve(Params.size());

		for (auto& shaderParam : Params)
		{
			CD3DX12_ROOT_PARAMETER rootParameter;
			switch (shaderParam.type)
			{
			case ShaderParamType::ConstantBuffer:
				rootParameter.InitAsConstantBufferView(shaderParam.baseRegister, shaderParam.registerSpace);
				break;
			case ShaderParamType::CBVDescriptorHeap:
				CD3DX12_DESCRIPTOR_RANGE constantTable;
				constantTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, shaderParam.descriptorNums, shaderParam.baseRegister, shaderParam.registerSpace);
				constantTables.push_back(constantTable);
				rootParameter.InitAsDescriptorTable(1, &constantTables.back());

				break;
			case ShaderParamType::SRVDescriptorHeap:
				CD3DX12_DESCRIPTOR_RANGE texTable;
				texTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, shaderParam.descriptorNums, shaderParam.baseRegister, shaderParam.registerSpace);
				texTables.push_back(texTable);
				rootParameter.InitAsDescriptorTable(1, &texTables.back());
				break;
			}
		}

		CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(UINT(rootParameters.size()), rootParameters.data(), 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		ComPtr<ID3DBlob> serializedRootSig = nullptr;
		ComPtr<ID3DBlob> errorBlob = nullptr;
		HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
			serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());

		if (errorBlob != nullptr)
		{
			::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
		}
		ThrowIfFailed(GraphicContext::GetSingleton().Device()->CreateRootSignature(
			0,
			serializedRootSig->GetBufferPointer(),
			serializedRootSig->GetBufferSize(),
			IID_PPV_ARGS(PRootSignature.GetAddressOf())));
	}



	void PShader::ReflectShader()
	{
		ComPtr<ID3D12ShaderReflection> Reflection;
		D3DReflect(PVS->GetBufferPointer(), PVS->GetBufferSize(), IID_ID3D12ShaderReflection, (void**)& Reflection);
		D3D12_SHADER_DESC desc;
		Reflection->GetDesc(&desc);
		for(size_t CBindex = 0; CBindex<desc.ConstantBuffers; ++CBindex)
		{
			ID3D12ShaderReflectionConstantBuffer* buffer = Reflection->GetConstantBufferByIndex(CBindex);
			D3D12_SHADER_BUFFER_DESC bufferDesc;
			buffer->GetDesc(&bufferDesc);
			for (UINT Varindex = 0; Varindex < bufferDesc.Variables; Varindex++)
			{
				ID3D12ShaderReflectionVariable* var = buffer->GetVariableByIndex(Varindex);
				D3D12_SHADER_VARIABLE_DESC varDesc;
				var->GetDesc(&varDesc);

//				memcpy(&mMappedData[elementIndex * mElementByteSize], &data, varDesc);
			}
		}
	}

	void PShader::BindRootSignature(ID3D12GraphicsCommandList* commandList)
	{
		commandList->SetGraphicsRootSignature(PRootSignature.Get());
	}

	void PShader::SetDescriptorTable(ID3D12GraphicsCommandList* commandList, UINT paramID, CD3DX12_GPU_DESCRIPTOR_HANDLE handle)
	{
		auto&& paramIterator = ParamMap.find(paramID);
		if (paramIterator == ParamMap.end())
		{
			return;
		}

		UINT rootParamIndex = paramIterator->second;
		ShaderParameter& parameter = Params[rootParamIndex];

		commandList->SetGraphicsRootDescriptorTable(rootParamIndex, handle);
	}

	void PShader::SetRootConstantBufferView(ID3D12GraphicsCommandList* commandList, UINT paramID, D3D12_GPU_VIRTUAL_ADDRESS address)
	{
		auto&& paramIterator = ParamMap.find(paramID);
		if (paramIterator == ParamMap.end())
		{
			return;
		}

		UINT rootParamIndex = paramIterator->second;
		ShaderParameter& parameter = Params[rootParamIndex];

		commandList->SetGraphicsRootConstantBufferView(rootParamIndex, address);
	}

	void PShader::SetPSODesc(D3D12_GRAPHICS_PIPELINE_STATE_DESC* psoDesc)
	{
		psoDesc->VS =
		{
			reinterpret_cast<BYTE*>(PVS->GetBufferPointer()),
			PVS->GetBufferSize()
		};
		psoDesc->PS =
		{
			reinterpret_cast<BYTE*>(PPS->GetBufferPointer()),
			PPS->GetBufferSize()
		};
		psoDesc->pRootSignature = PRootSignature.Get();
		psoDesc->RasterizerState = PRasterizerState;
		psoDesc->DepthStencilState = PDepthStencilState;
		psoDesc->BlendState = PBlendState;
	}
}