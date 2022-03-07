#include "pch.h"
#include "PRenderScene.h"
#include "GraphicsContext.h"
#include "PRenderThread.h"

namespace Phe
{
	//TODO 
	//Refactor To Dynamic staffs
	PRenderScene::PRenderScene() : RenderMeshNum(0)
	{
		PCbvSrvUavDescriptorSize = GraphicContext::GetSingleton().Device()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		D3D12_DESCRIPTOR_HEAP_DESC cbHeapDesc;
		cbHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		cbHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		cbHeapDesc.NumDescriptors = 1024;
		cbHeapDesc.NodeMask = 0;
		GraphicContext::GetSingleton().Device()->CreateDescriptorHeap(&cbHeapDesc, IID_PPV_ARGS(&CbvHeap));
//		BuildConstantBuffer();
		mPassCB = std::make_unique<UploadBuffer<PassConstants>>(GraphicContext::GetSingleton().Device().Get(), 1, true);
		BuildPSO();
	}

	PRenderScene::~PRenderScene()
	{

	}

	void PRenderScene::BuildMeshData(std::shared_ptr<PStaticMesh> StaticMesh, Transform MeshTransform)
	{
		PMeshDataStruct data;
		data.Vertices = StaticMesh->GetVertices();
		data.Indices = StaticMesh->GetIndices();
		data.Normal = StaticMesh->GetTangents();
		std::shared_ptr<PRenderStaticMesh> RSM = std::make_shared<PRenderStaticMesh>(StaticMesh->GetName(), data);
		RenderMeshData.insert({ StaticMesh->GetName(), RSM });
		RenderSceneMeshList.insert({ StaticMesh->GetName(), std::vector<Transform>{MeshTransform} });
		RenderMeshNum++;
		BuildConstantBuffer();
	}

	void PRenderScene::AddExistedMesh(std::string MeshName, Transform MeshTransform)
	{
		auto& MeshTransformVector = RenderSceneMeshList[MeshName];
		MeshTransformVector.push_back(MeshTransform);
		RenderMeshNum++;
		BuildConstantBuffer();
	}

	void PRenderScene::ClearScene()
	{
		RenderMeshData.clear();
		RenderSceneMeshList.clear();
		RenderMeshNum = 0;
	}

	void PRenderScene::UpdateCamera(PassConstants passcb)
	{
		mPassCB->CopyData(0, passcb);
	}

	void PRenderScene::BuildConstantBuffer()
	{
		GraphicContext::GetSingleton().ResetCommandList();

		mObjectCB = std::make_unique<UploadBuffer<ObjectConstants>>(GraphicContext::GetSingleton().Device().Get(), RenderMeshNum, true);
		UINT objCBByteSize = (sizeof(ObjectConstants) + 255) & ~255;
		UINT passCBByteSize = (sizeof(PassConstants) + 255) & ~255;

		D3D12_GPU_VIRTUAL_ADDRESS objcbAddress = mObjectCB->Resource()->GetGPUVirtualAddress();
		D3D12_GPU_VIRTUAL_ADDRESS passcbAddress = mPassCB->Resource()->GetGPUVirtualAddress();
		auto handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(CbvHeap->GetCPUDescriptorHandleForHeapStart());
		for (size_t index = 0; index < RenderMeshNum; index++)
		{
			objcbAddress = mObjectCB->Resource()->GetGPUVirtualAddress();
			handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(CbvHeap->GetCPUDescriptorHandleForHeapStart());
			size_t objCBufIndex = index;
			objcbAddress += objCBufIndex * objCBByteSize;
			size_t objheapIndex = index;
			handle.Offset(int(objheapIndex), PCbvSrvUavDescriptorSize);
			D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
			cbvDesc.BufferLocation = objcbAddress;
			cbvDesc.SizeInBytes = objCBByteSize;
			GraphicContext::GetSingleton().Device()->CreateConstantBufferView(&cbvDesc, handle);
		}
		int passCBufIndex = 0;
		passcbAddress += passCBufIndex * passCBByteSize;

		int passheapIndex = RenderMeshNum;

		handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(CbvHeap->GetCPUDescriptorHandleForHeapStart());
		handle.Offset(passheapIndex, PCbvSrvUavDescriptorSize);
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc1;
		cbvDesc1.BufferLocation = passcbAddress;
		cbvDesc1.SizeInBytes = passCBByteSize;

		GraphicContext::GetSingleton().Device()->CreateConstantBufferView(&cbvDesc1, handle);
		GraphicContext::GetSingleton().ExecuteCommandList();
		PRenderThread::Get()->FlushCommandQueue();
	}

	void PRenderScene::BuildPSO()
	{
		GraphicContext::GetSingleton().ResetCommandList();
		CD3DX12_ROOT_PARAMETER SlotRootParameter[2];
		CD3DX12_DESCRIPTOR_RANGE CbvTable;
		CD3DX12_DESCRIPTOR_RANGE CbvTable1;
		CbvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
		CbvTable1.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);
		SlotRootParameter[0].InitAsDescriptorTable(1, &CbvTable);
		SlotRootParameter[1].InitAsDescriptorTable(1, &CbvTable1);
		CD3DX12_ROOT_SIGNATURE_DESC RootSigDesc(2, SlotRootParameter, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
		ComPtr<ID3DBlob> SerializedRootSig = nullptr;
		ComPtr<ID3DBlob> ErrorBlob = nullptr;
		D3D12SerializeRootSignature(&RootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1, SerializedRootSig.GetAddressOf(), ErrorBlob.GetAddressOf());
		GraphicContext::GetSingleton().Device()->CreateRootSignature(0, SerializedRootSig->GetBufferPointer(), SerializedRootSig->GetBufferSize(), IID_PPV_ARGS(&RootSignature));

		MvsByteCode = CompileShader(L"Shaders\\color.hlsl", nullptr, "VS", "vs_5_0");
		MpsByteCode = CompileShader(L"Shaders\\color.hlsl", nullptr, "PS", "ps_5_0");

		InputLayout =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
		};

		D3D12_GRAPHICS_PIPELINE_STATE_DESC PsoDesc;
		ZeroMemory(&PsoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
		PsoDesc.InputLayout = { InputLayout.data(), (UINT)InputLayout.size() };
		PsoDesc.pRootSignature = RootSignature.Get();
		PsoDesc.VS = { reinterpret_cast<BYTE*>(MvsByteCode->GetBufferPointer()),MvsByteCode->GetBufferSize() };
		PsoDesc.PS = { reinterpret_cast<BYTE*>(MpsByteCode->GetBufferPointer()),MpsByteCode->GetBufferSize() };

		PsoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		PsoDesc.RasterizerState.FrontCounterClockwise = TRUE;
		PsoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		PsoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		PsoDesc.SampleMask = UINT_MAX;
		PsoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		PsoDesc.NumRenderTargets = 1;
		PsoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;//DXGI_FORMAT mBackBufferFormat
		PsoDesc.SampleDesc.Count = 1;//m4xMsaaState ? 4 : 1;
		PsoDesc.SampleDesc.Quality = 0;//m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
		PsoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;//DXGI_FORMAT mDepthStencilFormat
		GraphicContext::GetSingleton().Device()->CreateGraphicsPipelineState(&PsoDesc, IID_PPV_ARGS(&PSO));

		GraphicContext::GetSingleton().ExecuteCommandList();
		PRenderThread::Get()->FlushCommandQueue();
	}

	void PRenderScene::Render()
	{
		ObjectConstants objConstants;
		UINT32 MeshIndex = 0;

		auto commandList = GraphicContext::GetSingleton().CommandList();
		ID3D12DescriptorHeap* DescriptorHeaps[] = { CbvHeap.Get() };
		commandList->SetDescriptorHeaps(_countof(DescriptorHeaps), DescriptorHeaps);
		commandList->SetPipelineState(PSO.Get());
		commandList->SetGraphicsRootSignature(RootSignature.Get());

		for (auto MeshGenre : RenderSceneMeshList)
		{
			auto vbv = RenderMeshData[MeshGenre.first]->VertexBufferView();
			commandList->IASetVertexBuffers(0, 1, &vbv);
			auto ibv = RenderMeshData[MeshGenre.first]->IndexBufferView();
			commandList->IASetIndexBuffer(&ibv);
			commandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			UINT DrawIndexCount = RenderMeshData[MeshGenre.first]->IndexCount();
			for (auto MeshTransform : MeshGenre.second)
			{
				objConstants.Position = MeshTransform.GetPositionMat();
				objConstants.Rotation = MeshTransform.GetRotaionMat();
				objConstants.Scale = MeshTransform.GetScaleMat();
				mObjectCB->CopyData(MeshIndex, objConstants);
				auto cbvHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(CbvHeap->GetGPUDescriptorHandleForHeapStart());
				cbvHandle.Offset(MeshIndex, PCbvSrvUavDescriptorSize);
				commandList->SetGraphicsRootDescriptorTable(0, cbvHandle);
				cbvHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(CbvHeap->GetGPUDescriptorHandleForHeapStart());
				cbvHandle.Offset(RenderMeshNum, PCbvSrvUavDescriptorSize);
				commandList->SetGraphicsRootDescriptorTable(1, cbvHandle);
				commandList->DrawIndexedInstanced(DrawIndexCount, 1, 0, 0, 0);
				MeshIndex++;
			}
		}
	}


	ComPtr<ID3DBlob> PRenderScene::CompileShader(const std::wstring& Filename, const D3D_SHADER_MACRO* Defines, const std::string& EntryPoint, const std::string& Target)
	{
		ComPtr<ID3DBlob> ByteCode;
		ComPtr<ID3DBlob> Errors;
		D3DCompileFromFile(Filename.c_str(), Defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
			EntryPoint.c_str(), Target.c_str(), 0, 0, &ByteCode, &Errors);
		return ByteCode;
	}

}

