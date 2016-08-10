#pragma once
#include "RenderComponent.h"
#include "AssimpModel.h"

class ModelRenderer : public RenderComponent
{
public:
	
	AssimpModel* model;

	ModelRenderer(AssimpModel* model);
	ModelRenderer(AssimpModel* model, ShaderProgram* shader);
	~ModelRenderer();

	void SetModel(AssimpModel* model);
	virtual void Render() override;
};

