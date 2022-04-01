#pragma once
#include "PNode.h"
#include "PCamera.h"
namespace Phe
{
	class PRenderLight;
	class PLight;
	class PNodeLight : public PNode
	{
		DECLARE_NODE_WITH_CONSTRUCTOR(Light);

	public:
		void SetAsLight(PLight* Light);
		void SetLightName(std::string LightName);
		void SetLightRadius(float LightRadius);
		void SetLightStrength(float LightStrength);
		void SetIsDynamic(bool IsDynamic);
		void SetLinkedLight(PRenderLight* LinkedLight);
		void SetPosition(glm::vec3 Position);
		void SetRotation(glm::vec3 Rotation);
		void BindLinkedLight(PLight* Light);
		void BindRenderLight(PRenderLight* RenderLight);
		PRenderLight* GetLinkedRenderLight() { return PLinkedRenderLight; }
		virtual ~PNodeLight();

		bool GetIsDynamic() { return PIsDynamic; }
		PerCameraCBuffer GetPassCBuffer(){ return LightView->GetPassConstant(); }
		glm::mat4 GetVP() { return LightView->GetProjectionViewMatrix();}
		POrthographicCamera* GetLightView() { return LightView; }
	private:
		std::string PLightName;
		float PLightRaidus;
		float PLightStrength;
		bool PIsDynamic;

		POrthographicCamera* LightView;

		PLight* PLinkedLight;
		PRenderLight* PLinkedRenderLight;
	};
}