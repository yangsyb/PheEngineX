#pragma once
#include "pch.h"
#include "Engine/Editor/PShader.h"

namespace Phe
{
	class PPipeline
	{
	public:
		PPipeline(PShader* Shader);
		virtual ~PPipeline();

		virtual PShader* GetShader() { return PPipelineShader; }
		virtual std::string GetShaderName() { return PPipelineShader->GetShaderName(); }
	protected:
		PShader* PPipelineShader;
	};
}