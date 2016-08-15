#include "ModelRenderer.h"
#include "SceneNode.h"

ModelRenderer::ModelRenderer(AssimpModel* model)
{
	this->model = model;
	if (model != NULL)
	{
		m_VAO_ID = model->GetVAO();
		radius = model->farthestPoint;
	}
}

ModelRenderer::ModelRenderer(AssimpModel * model, ShaderProgram * shader) : RenderComponent(shader)
{
	this->model = model;
	if (model != NULL)
	{
		m_VAO_ID = model->GetVAO();
		radius = model->farthestPoint;
	}
}

ModelRenderer::~ModelRenderer()
{
}

void ModelRenderer::SetModel(AssimpModel *model)
{
	this->model = model;
	if (model != NULL)
	{
		m_VAO_ID = model->GetVAO();
		radius = model->farthestPoint;
	}
}

void ModelRenderer::Render()
{
	if (model != NULL)
	{
		m_shader->SetUniform("model", sceneNode->transform.model);
		model->RenderModel();
	}
}
