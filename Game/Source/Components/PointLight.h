#include "../BufferDataStructures.h"
#include "Component.h"

class PointLight :
	public Component
{
public:
	PointLight(class RenderObj* pObj);
	~PointLight();
	BufferData::PointLightData* mLight;
	void LoadProperties(const rapidjson::Value& properties) override;
};
