#include "pch.h"
#include "PRenderScene.h"
#include "GraphicsContext.h"
#include "PRenderThread.h"
#include "PAssetManager.h"
#include "RHI/PShader.h"

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

	void PRenderScene::BuildMeshData(std::string MeshName, Transform MeshTransform)
	{
		PMeshDataStruct data = PAssetManager::GetSingleton().GetMeshData(MeshName);

		std::shared_ptr<PRenderStaticMesh> RSM = std::make_shared<PRenderStaticMesh>(MeshName, data);
		RenderMeshData.insert({ MeshName, RSM });
		RenderSceneMeshList.insert({ MeshName, std::vector<Transform>{MeshTransform} });
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

	void PRenderScene::BuildWPOMeshData(std::string MeshName, Transform MeshTransform)
	{
		PMeshDataStruct data = PAssetManager::GetSingleton().GetMeshData(MeshName);
		std::shared_ptr<PRenderStaticMesh> RSM = std::make_shared<PRenderStaticMesh>(MeshName, data);
		WPORenderMeshData.insert({ MeshName, RSM });
		WPORenderSceneMeshList.insert({ MeshName, std::vector<Transform>{MeshTransform} });
		WPORenderMeshNum++;
		BuildConstantBuffer();
	}

	void PRenderScene::AddWPOExistedMesh(std::string MeshName, Transform MeshTransform)
	{
		auto& MeshTransformVector = WPORenderSceneMeshList[MeshName];
		MeshTransformVector.push_back(MeshTransform);
		WPORenderMeshNum++;
		BuildConstantBuffer();
	}

	void PRenderScene::ClearScene()
	{
		RenderMeshData.clear();
		RenderSceneMeshList.clear();
		WPORenderMeshData.clear();
		WPORenderSceneMeshList.clear();
		RenderMeshNum = 0;
		WPORenderMeshNum = 0;
	}

	void PRenderScene::UpdateCamera(PassConstants passcb)
	{
		mPassCB->CopyData(0, passcb);
	}

	void PRenderScene::BuildConstantBuffer()
	{
		GraphicContext::GetSingleton().ResetCommandList();
		UINT objCBByteSize = (sizeof(ObjectConstants) + 255) & ~255;
		UINT passCBByteSize = (sizeof(PassConstants) + 255) & ~255;
		UINT treeCBByteSize = (sizeof(PTreeConstants) + 255) & ~255;

		if (RenderMeshNum > 0)
		{
			mObjectCB = std::make_unique<UploadBuffer<ObjectConstants>>(GraphicContext::GetSingleton().Device().Get(), RenderMeshNum + WPORenderMeshNum, true);

		}
		if (WPORenderMeshNum > 0)
		{
			mObjectCB = std::make_unique<UploadBuffer<ObjectConstants>>(GraphicContext::GetSingleton().Device().Get(), RenderMeshNum + WPORenderMeshNum, true);
			mTreeCB = std::make_unique<UploadBuffer<PTreeConstants>>(GraphicContext::GetSingleton().Device().Get(), 1, true);

		}

		D3D12_GPU_VIRTUAL_ADDRESS objcbAddress = mObjectCB->Resource()->GetGPUVirtualAddress();
		D3D12_GPU_VIRTUAL_ADDRESS passcbAddress = mPassCB->Resource()->GetGPUVirtualAddress();
		D3D12_GPU_VIRTUAL_ADDRESS treecbAddress = D3D12_GPU_VIRTUAL_ADDRESS();
		if (mTreeCB)
		{
			treecbAddress = mTreeCB->Resource()->GetGPUVirtualAddress();
		}

		auto handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(CbvHeap->GetCPUDescriptorHandleForHeapStart());
		for (size_t index = 0; index < size_t(RenderMeshNum) + size_t(WPORenderMeshNum); index++)
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
		size_t passCBufIndex = 0;
		passcbAddress += size_t(passCBufIndex) * UINT(passCBByteSize);

		int passheapIndex = RenderMeshNum + WPORenderMeshNum;

		handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(CbvHeap->GetCPUDescriptorHandleForHeapStart());
		handle.Offset(passheapIndex, PCbvSrvUavDescriptorSize);
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc1;
		cbvDesc1.BufferLocation = passcbAddress;
		cbvDesc1.SizeInBytes = passCBByteSize;

		GraphicContext::GetSingleton().Device()->CreateConstantBufferView(&cbvDesc1, handle);

		if (mTreeCB)
		{
			size_t treeBufIndex = 0;
			treecbAddress += size_t(treeBufIndex) * treeCBByteSize;
			int treepassheapIndex = RenderMeshNum + WPORenderMeshNum + 1;
			handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(CbvHeap->GetCPUDescriptorHandleForHeapStart());
			handle.Offset(treepassheapIndex, PCbvSrvUavDescriptorSize);
			D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc2;
			cbvDesc2.BufferLocation = treecbAddress;
			cbvDesc2.SizeInBytes = treeCBByteSize;

			GraphicContext::GetSingleton().Device()->CreateConstantBufferView(&cbvDesc2, handle);
		}


		GraphicContext::GetSingleton().ExecuteCommandList();
		PRenderThread::Get()->FlushCommandQueue();
	}

	void PRenderScene::BuildPSO()
	{
		GraphicContext::GetSingleton().ResetCommandList();
		CD3DX12_ROOT_PARAMETER SlotRootParameter[2]{ CD3DX12_ROOT_PARAMETER(), CD3DX12_ROOT_PARAMETER() };
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


		CD3DX12_ROOT_PARAMETER WPOSlotRootParameter[3]{ CD3DX12_ROOT_PARAMETER(),CD3DX12_ROOT_PARAMETER(),CD3DX12_ROOT_PARAMETER() };
		CD3DX12_DESCRIPTOR_RANGE WPOCbvTable;
		CD3DX12_DESCRIPTOR_RANGE WPOCbvTable1;
		CD3DX12_DESCRIPTOR_RANGE WPOCbvTable2;
		WPOCbvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
		WPOCbvTable1.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);
		WPOCbvTable2.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 2);
		WPOSlotRootParameter[0].InitAsDescriptorTable(1, &WPOCbvTable);
		WPOSlotRootParameter[1].InitAsDescriptorTable(1, &WPOCbvTable1);
		WPOSlotRootParameter[2].InitAsDescriptorTable(1, &WPOCbvTable2);
		CD3DX12_ROOT_SIGNATURE_DESC WPORootSigDesc(3, WPOSlotRootParameter, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
		ComPtr<ID3DBlob> WPOSerializedRootSig = nullptr;
		ComPtr<ID3DBlob> WPOErrorBlob = nullptr;
		D3D12SerializeRootSignature(&WPORootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1, WPOSerializedRootSig.GetAddressOf(), WPOErrorBlob.GetAddressOf());
		GraphicContext::GetSingleton().Device()->CreateRootSignature(0, WPOSerializedRootSig->GetBufferPointer(), WPOSerializedRootSig->GetBufferSize(), IID_PPV_ARGS(&WPORootSignature));

		WPOMvsByteCode = CompileShader(L"Shaders\\tree.hlsl", nullptr, "VS", "vs_5_0");
		WPOMpsByteCode = CompileShader(L"Shaders\\tree.hlsl", nullptr, "PS", "ps_5_0");

		WPOInputLayout =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
		};

		D3D12_GRAPHICS_PIPELINE_STATE_DESC WPOPsoDesc;
		ZeroMemory(&WPOPsoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
		WPOPsoDesc.InputLayout = { WPOInputLayout.data(), (UINT)WPOInputLayout.size() };
		WPOPsoDesc.pRootSignature = WPORootSignature.Get();
		WPOPsoDesc.VS = { reinterpret_cast<BYTE*>(WPOMvsByteCode->GetBufferPointer()),WPOMvsByteCode->GetBufferSize() };
		WPOPsoDesc.PS = { reinterpret_cast<BYTE*>(WPOMpsByteCode->GetBufferPointer()),WPOMpsByteCode->GetBufferSize() };

		WPOPsoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		WPOPsoDesc.RasterizerState.FrontCounterClockwise = TRUE;
		WPOPsoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		WPOPsoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		WPOPsoDesc.SampleMask = UINT_MAX;
		WPOPsoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		WPOPsoDesc.NumRenderTargets = 1;
		WPOPsoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;//DXGI_FORMAT mBackBufferFormat
		WPOPsoDesc.SampleDesc.Count = 1;//m4xMsaaState ? 4 : 1;
		WPOPsoDesc.SampleDesc.Quality = 0;//m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
		WPOPsoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;//DXGI_FORMAT mDepthStencilFormat
		GraphicContext::GetSingleton().Device()->CreateGraphicsPipelineState(&WPOPsoDesc, IID_PPV_ARGS(&WPOPSO));

		GraphicContext::GetSingleton().ExecuteCommandList();
		PRenderThread::Get()->FlushCommandQueue();
	}

	void PRenderScene::Render()
	{
		ObjectConstants objConstants;
		PTreeConstants treeConstants;
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
				cbvHandle.Offset(RenderMeshNum + WPORenderMeshNum, PCbvSrvUavDescriptorSize);
				commandList->SetGraphicsRootDescriptorTable(1, cbvHandle);
				commandList->DrawIndexedInstanced(DrawIndexCount, 1, 0, 0, 0);
				MeshIndex++;
			}
		}

		commandList->SetPipelineState(WPOPSO.Get());
		commandList->SetGraphicsRootSignature(WPORootSignature.Get());
		for (auto MeshGenre : WPORenderSceneMeshList)
		{
			auto vbv = WPORenderMeshData[MeshGenre.first]->VertexBufferView();
			commandList->IASetVertexBuffers(0, 1, &vbv);
			auto ibv = WPORenderMeshData[MeshGenre.first]->IndexBufferView();
			commandList->IASetIndexBuffer(&ibv);
			commandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			UINT DrawIndexCount = WPORenderMeshData[MeshGenre.first]->IndexCount();
			for (auto MeshTransform : MeshGenre.second)
			{
				objConstants.Position = MeshTransform.GetPositionMat();
				objConstants.Rotation = MeshTransform.GetRotaionMat();
				objConstants.Scale = MeshTransform.GetScaleMat();
				treeConstants.Center = glm::vec3(0, 0, 0);
				treeConstants.Time = PRenderThread::Get()->GetCurrentTotalTime();
				mObjectCB->CopyData(MeshIndex, objConstants);
				mTreeCB->CopyData(0, treeConstants);
				auto cbvHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(CbvHeap->GetGPUDescriptorHandleForHeapStart());
				cbvHandle.Offset(MeshIndex, PCbvSrvUavDescriptorSize);
				commandList->SetGraphicsRootDescriptorTable(0, cbvHandle);
				cbvHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(CbvHeap->GetGPUDescriptorHandleForHeapStart());
				cbvHandle.Offset(RenderMeshNum + WPORenderMeshNum, PCbvSrvUavDescriptorSize);
				commandList->SetGraphicsRootDescriptorTable(1, cbvHandle);
				cbvHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(CbvHeap->GetGPUDescriptorHandleForHeapStart());
				cbvHandle.Offset(RenderMeshNum + WPORenderMeshNum + 1, PCbvSrvUavDescriptorSize);
				commandList->SetGraphicsRootDescriptorTable(2, cbvHandle);
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

