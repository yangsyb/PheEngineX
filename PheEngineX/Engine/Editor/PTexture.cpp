#include "pch.h"
#include "PTexture.h"
#include "PMaterial.h"
#include "GPUResource/PGPUTexture.h"
#include "Render/PRenderThread.h"

namespace Phe
{

	PTexture::PTexture(std::string TextureName, std::wstring TexturePathName) : PTextureName(TextureName), PTexturePathName(TexturePathName), PLinkedGPUTexture(nullptr)
	{

	}

	PTexture::~PTexture()
	{
		std::string TextureName = PTextureName;
		PTask* Task = CreateTask(PTask, PRenderThread::Get()->GetRenderScene()->DestroySceneTexture(TextureName));
		PRenderThread::Get()->AddTask(Task);
		//TODO Warining At LinkedMaterial
	}

	void PTexture::BindMaterial(PMaterial* Material)
	{
		PLinkedMaterial.insert({ Material->GetName(), Material });
	}

	void PTexture::BindGPUTexture(PGPUTexture* LinkedGPUTexture)
	{
		PLinkedGPUTexture = LinkedGPUTexture;
	}

}