#pragma once
#include "pch.h"
#include "PShader.h"

namespace Phe
{
	enum class PipelineType
	{
		BasePipeline,
		ShadowPipeline,

		PipelineCount
	};

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