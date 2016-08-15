#pragma once
#include "RenderComponent.h"
class SkyboxRenderer :	public RenderComponent
{
	friend class Skybox;
public:
	
	SkyboxRenderer();
	~SkyboxRenderer();

	virtual void Render() override;

private:
	Skybox* m_skybox;
};

