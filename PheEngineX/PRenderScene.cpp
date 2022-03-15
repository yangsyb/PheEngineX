#include "pch.h"
#include "PRenderScene.h"
#include "GraphicsContext.h"
#include "PRenderThread.h"
#include "PAssetManager.h"
#include "RHI/PShader.h"
#include "RHI/PShaderManager.h"


namespace Phe
{
	PRenderScene::PRenderScene() : ActorNum(0)
	{
		PCbvSrvUavDescriptorSize = GraphicContext::GetSingleton().Device()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		D3D12_DESCRIPTOR_HEAP_DESC cbHeapDesc;
		cbHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		cbHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		cbHeapDesc.NumDescriptors = 4096;
		cbHeapDesc.NodeMask = 0;
		GraphicContext::GetSingleton().Device()->CreateDescriptorHeap(&cbHeapDesc, IID_PPV_ARGS(&CbvHeap));

		PerObjectCBufferByteSize = (sizeof(PerObjectCBuffer) + 255) & ~255;
		PerCameraCBufferByteSize = (sizeof(PerCameraCBuffer) + 255) & ~255;
		PerFrameCBufferByteSize = (sizeof(PerFrameCBuffer) + 255) & ~255;
		PerMaterialCBufferByteSize = (sizeof(PerMaterialCBuffer) + 255) & ~255;

		PerCameraCB = std::make_unique<UploadBuffer<PerCameraCBuffer>>(GraphicContext::GetSingleton().Device().Get(), 1, true);
	}

	PRenderScene::~PRenderScene()
	{

	}



	void PRenderScene::AddActor(std::string StaticMeshName, Transform MeshTransform, std::string MaterialName)
	{
		GraphicContext::GetSingleton().ResetCommandList();
		PMeshDataStruct data = PAssetManager::GetSingleton().GetMeshData(StaticMeshName);
		std::shared_ptr<PRenderStaticMesh> RSM = std::make_shared<PRenderStaticMesh>(StaticMeshName, data);
		RenderActorData.insert({ StaticMeshName, RSM });
		std::shared_ptr<PMaterial> Mat = PAssetManager::GetSingleton().GetMaterialData(MaterialName);
		std::shared_ptr<PRenderMaterial> RM = std::make_shared<PRenderMaterial>(Mat);
		RenderMaterialData.insert({ MaterialName , RM });
		std::string TexName = Mat->GetTextureName();
		std::shared_ptr<PRenderTexture> RT = std::make_shared<PRenderTexture>(PAssetManager::GetSingleton().GetTextureData(TexName));
		RenderTextureData.insert({ RT ,  UINT(RenderTextureData.size()) });
		RM->SetHeapOffsetIndex(RenderTextureData[RT]);

		std::shared_ptr<PActor> Actor = std::make_shared<PActor>();
		Actor->SetStaticMesh(StaticMeshName);
		Actor->SetTransform(MeshTransform);
		Actor->SetMaterial(MaterialName);
		std::string ShaderName = RM->GetShader()->GetName();
		bool IsExist = false;
		for (auto& RenderActorPool : RenderActorPools)
		{
			if (RenderActorPool.first == ShaderName)
			{
				RenderActorPool.second->Push(Actor);
				IsExist = true;
				break;
			}
		}
		if (!IsExist)
		{
			std::shared_ptr<DrawPool> Pool = std::make_shared<DrawPool>(ShaderName);
			Pool->Push(Actor);
			RenderActorPools.insert({ ShaderName , Pool });
		}
		ActorNum++;
		GraphicContext::GetSingleton().ExecuteCommandList();
		PRenderThread::Get()->FlushCommandQueue();
		BuildConstantBuffer();
	}

	void PRenderScene::AddActors(std::vector<std::string> StaticMeshName, std::vector<Transform> MeshTransform, std::vector<std::string> MeshMaterialName)
	{
		for (size_t index = 0; index < StaticMeshName.size(); index++)
		{
			GraphicContext::GetSingleton().ResetCommandList();
			PMeshDataStruct data = PAssetManager::GetSingleton().GetMeshData(StaticMeshName[index]);
			std::shared_ptr<PRenderStaticMesh> RSM = std::make_shared<PRenderStaticMesh>(StaticMeshName[index], data);
			RenderActorData.insert({ StaticMeshName[index], RSM });

			std::shared_ptr<PActor> Actor = std::make_shared<PActor>();
			Actor->SetStaticMesh(StaticMeshName[index]);
			Actor->SetTransform(MeshTransform[index]);
			Actor->SetMaterial(MeshMaterialName[index]);
			std::string ShaderName = PAssetManager::GetSingleton().GetMaterialData(MeshMaterialName[index])->GetShader()->GetName();
			bool IsExist = false;
			for (auto& RenderActorPool : RenderActorPools)
			{
				if (RenderActorPool.first == ShaderName)
				{
					RenderActorPool.second->Push(Actor);
					IsExist = true;
					break;
				}
			}
			if (!IsExist)
			{
				std::shared_ptr<DrawPool> Pool = std::make_shared<DrawPool>(ShaderName);
				Pool->Push(Actor);
				RenderActorPools.insert({ ShaderName , Pool });
			}
			ActorNum++;
			GraphicContext::GetSingleton().ExecuteCommandList();
			PRenderThread::Get()->FlushCommandQueue();
		}
		BuildConstantBuffer();
	}

	void PRenderScene::ClearScene()
	{
		RenderActorPools.clear();
		ActorNum = 0;
	}

	void PRenderScene::UpdateCamera(PerCameraCBuffer CameraCBuffer)
	{
		PerCameraCB->CopyData(0, CameraCBuffer);
	}

	void PRenderScene::BuildConstantBuffer()
	{
		GraphicContext::GetSingleton().ResetCommandList();

		if (ActorNum > 0)
		{
			PerObjCB = std::make_unique<UploadBuffer<PerObjectCBuffer>>(GraphicContext::GetSingleton().Device().Get(), ActorNum, true);
			PerFrameCB = std::make_unique<UploadBuffer<PerFrameCBuffer>>(GraphicContext::GetSingleton().Device().Get(), ActorNum, true);
			PerMaterialCB = std::make_unique<UploadBuffer<PerMaterialCBuffer>>(GraphicContext::GetSingleton().Device().Get(), ActorNum, true);
		}
		auto handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(CbvHeap->GetCPUDescriptorHandleForHeapStart());

		//Texture
		for (auto TextureIdx : RenderTextureData)
		{
			handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(CbvHeap->GetCPUDescriptorHandleForHeapStart());
			size_t TextureHeapIndex = TextureIdx.second;
			handle.Offset(int(TextureHeapIndex), PCbvSrvUavDescriptorSize);
			TextureIdx.first->CreateSRV(handle);
		}
		//PerObj
		for (size_t index = 0; index < size_t(ActorNum); index++)
		{
			D3D12_GPU_VIRTUAL_ADDRESS PerObjCBAddress = PerObjCB->Resource()->GetGPUVirtualAddress();
			handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(CbvHeap->GetCPUDescriptorHandleForHeapStart());
			size_t ObjBufferIndex = index;
			PerObjCBAddress += ObjBufferIndex * PerObjectCBufferByteSize;
			size_t ObjHeapIndex = index + RenderTextureData.size();
			handle.Offset(int(ObjHeapIndex), PCbvSrvUavDescriptorSize);
			D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
			cbvDesc.BufferLocation = PerObjCBAddress;
			cbvDesc.SizeInBytes = PerObjectCBufferByteSize;
			GraphicContext::GetSingleton().Device()->CreateConstantBufferView(&cbvDesc, handle);
		}

		//PerCamera
		D3D12_GPU_VIRTUAL_ADDRESS PerCameraCBAddress = PerCameraCB->Resource()->GetGPUVirtualAddress();
		handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(CbvHeap->GetCPUDescriptorHandleForHeapStart());
		size_t CameraBufferIndex = 0;
		PerCameraCBAddress += CameraBufferIndex * PerCameraCBufferByteSize;
		size_t CameraHeapIndex = ActorNum + RenderTextureData.size();
		handle.Offset(int(CameraHeapIndex), PCbvSrvUavDescriptorSize);
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
		cbvDesc.BufferLocation = PerCameraCBAddress;
		cbvDesc.SizeInBytes = PerCameraCBufferByteSize;
		GraphicContext::GetSingleton().Device()->CreateConstantBufferView(&cbvDesc, handle);

		for (size_t index = 0; index < size_t(ActorNum); index++)
		{
			//PerFrame
			D3D12_GPU_VIRTUAL_ADDRESS PerFrameCBAddress = PerFrameCB->Resource()->GetGPUVirtualAddress();
			handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(CbvHeap->GetCPUDescriptorHandleForHeapStart());
			size_t FrameBufferIndex = index;
			PerFrameCBAddress += FrameBufferIndex * PerFrameCBufferByteSize;
			size_t FrameHeapIndex = ActorNum + index + 1 + RenderTextureData.size();
			handle.Offset(int(FrameHeapIndex), PCbvSrvUavDescriptorSize);
			D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
			cbvDesc.BufferLocation = PerFrameCBAddress;
			cbvDesc.SizeInBytes = PerFrameCBufferByteSize;
			GraphicContext::GetSingleton().Device()->CreateConstantBufferView(&cbvDesc, handle);

			//PerMaterial
			D3D12_GPU_VIRTUAL_ADDRESS PerMaterialCBAddress = PerMaterialCB->Resource()->GetGPUVirtualAddress();
			handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(CbvHeap->GetCPUDescriptorHandleForHeapStart());
			size_t MaterialBufferIndex = index;
			PerMaterialCBAddress += MaterialBufferIndex * PerMaterialCBufferByteSize;
			size_t MaterialHeapIndex = 2 * ActorNum + index + 1 + RenderTextureData.size();
			handle.Offset(int(MaterialHeapIndex), PCbvSrvUavDescriptorSize);
			D3D12_CONSTANT_BUFFER_VIEW_DESC MatcbvDesc;
			MatcbvDesc.BufferLocation = PerMaterialCBAddress;
			MatcbvDesc.SizeInBytes = PerMaterialCBufferByteSize;
			GraphicContext::GetSingleton().Device()->CreateConstantBufferView(&MatcbvDesc, handle);
		}


		GraphicContext::GetSingleton().ExecuteCommandList();
		PRenderThread::Get()->FlushCommandQueue();

	}

	void PRenderScene::Render()
	{
		PerObjectCBuffer PerObjConstant;
		PerFrameCBuffer PerFrameConstant;
		PerMaterialCBuffer PerMaterialConstant;
		auto commandList = GraphicContext::GetSingleton().CommandList();
		ID3D12DescriptorHeap* DescriptorHeaps[] = { CbvHeap.Get() };
		commandList->SetDescriptorHeaps(_countof(DescriptorHeaps), DescriptorHeaps);
		UINT32 ActorIndex = 0;

		ID3D12PipelineState* CurrentPSO = nullptr;
		for (auto RenderActorPool : RenderActorPools)
		{
			std::shared_ptr<PShader> RenderActorShader = PShaderManager::GetSingleton().GetShader(RenderActorPool.first);
			ID3D12PipelineState* ThisPSO = GraphicContext::GetSingleton().GetPSO(RenderActorShader);
			if (CurrentPSO != ThisPSO)
			{
				//				commandList->SetPipelineState(GraphicContext::GetSingleton().GetPSO(RenderActorShader));
				commandList->SetPipelineState(ThisPSO);
				CurrentPSO = ThisPSO;
				RenderActorShader->BindRootSignature(commandList);
			}

			auto RenderMeshMap = RenderActorPool.second->Request();
			for (auto Actors : RenderMeshMap)
			{
				auto vbv = RenderActorData[Actors.first]->VertexBufferView();
				commandList->IASetVertexBuffers(0, 1, &vbv);
				auto ibv = RenderActorData[Actors.first]->IndexBufferView();
				commandList->IASetIndexBuffer(&ibv);
				commandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				UINT DrawIndexCount = RenderActorData[Actors.first]->IndexCount();
				for (auto Actor : Actors.second)
				{
					std::shared_ptr<PRenderMaterial> ActorMaterial = RenderMaterialData[Actor->GetMaterialName()];
					PerObjConstant.Position = Actor->GetTransform().GetPositionMat();
					PerObjConstant.Rotation = Actor->GetTransform().GetRotaionMat();
					PerObjConstant.Scale = Actor->GetTransform().GetScaleMat();
					PerFrameConstant.Time = PRenderThread::Get()->GetCurrentTotalTime();
					PerMaterialConstant.DiffuseAlbedo = ActorMaterial->GetDiffuseAlbedo();
					PerMaterialConstant.FresnelR0 = ActorMaterial->GetPFresnelR0();
					PerMaterialConstant.Roughness = ActorMaterial->GetRoughness();
					PerObjCB->CopyData(ActorIndex, PerObjConstant);
					PerFrameCB->CopyData(ActorIndex, PerFrameConstant);
					PerMaterialCB->CopyData(ActorIndex, PerMaterialConstant);
					auto TShader = ActorMaterial->GetShader();
					UINT cbvIndex = ActorIndex;

					// 					auto cbvHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(CbvHeap->GetGPUDescriptorHandleForHeapStart());
					// 					cbvHandle.Offset(cbvIndex, PCbvSrvUavDescriptorSize);
					// 					TShader->SetDescriptorTable(commandList, PShaderManager::GetSingleton().PropertyToID("PerObjectBuffer"), cbvHandle);
					// 
					// 					cbvHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(CbvHeap->GetGPUDescriptorHandleForHeapStart());
					// 					cbvHandle.Offset(ActorNum, PCbvSrvUavDescriptorSize);
					// 					TShader->SetDescriptorTable(commandList, PShaderManager::GetSingleton().PropertyToID("PerCameraBuffer"), cbvHandle);
					// 
					// 					cbvHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(CbvHeap->GetGPUDescriptorHandleForHeapStart());
					// 					cbvHandle.Offset(ActorNum + cbvIndex + 1, PCbvSrvUavDescriptorSize);
					// 					TShader->SetDescriptorTable(commandList, PShaderManager::GetSingleton().PropertyToID("PerFrameBuffer"), cbvHandle);

					auto cbvHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(CbvHeap->GetGPUDescriptorHandleForHeapStart());
					cbvHandle.Offset(ActorMaterial->GetHeapOffsetIndex(), PCbvSrvUavDescriptorSize);
					TShader->SetDescriptorTable(commandList, PShaderManager::GetSingleton().PropertyToID("Texture"), cbvHandle);
					TShader->SetRootConstantBufferView(commandList, PShaderManager::GetSingleton().PropertyToID("PerObjectBuffer"), PerObjCB->Resource()->GetGPUVirtualAddress() + ActorIndex * PerObjectCBufferByteSize);
					TShader->SetRootConstantBufferView(commandList, PShaderManager::GetSingleton().PropertyToID("PerCameraBuffer"), PerCameraCB->Resource()->GetGPUVirtualAddress());
					TShader->SetRootConstantBufferView(commandList, PShaderManager::GetSingleton().PropertyToID("PerFrameBuffer"), PerFrameCB->Resource()->GetGPUVirtualAddress() + ActorIndex * PerFrameCBufferByteSize);
					TShader->SetRootConstantBufferView(commandList, PShaderManager::GetSingleton().PropertyToID("PerMaterialBuffer"), PerMaterialCB->Resource()->GetGPUVirtualAddress() + ActorIndex * PerMaterialCBufferByteSize);


					commandList->DrawIndexedInstanced(DrawIndexCount, 1, 0, 0, 0);
					ActorIndex++;
				}
			}
		}
	}

	DrawPool::DrawPool(std::string ShaderName) : PShaderName(ShaderName)
	{

	}

	DrawPool::~DrawPool()
	{
		for (auto Actors : PActorMap)
		{
			for (auto Actor : Actors.second)
			{

			}
		}
		PActorMap.clear();
	}

	void DrawPool::Push(std::shared_ptr<PActor> actor)
	{
		for (auto& ActorList : PActorMap)
		{
			if (ActorList.first == actor->GetName())
			{
				ActorList.second.push_back(actor);
				break;
			}
		}
		PActorMap.insert({ actor->GetName(), std::vector<std::shared_ptr<PActor>>{actor} });
	}

	std::unordered_map<std::string, std::vector<std::shared_ptr<PActor>>> DrawPool::Request()
	{
		return PActorMap;
	}
}

