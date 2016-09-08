#include "ListenerComponent.h"
#include "SoundManager.h"
#include "SceneNode.h"
#include "Game.h"


ListenerComponent::ListenerComponent()
{
}


ListenerComponent::~ListenerComponent()
{
	sceneNode->GetGame()->GetSoundManager()->RemoveListener(sceneNode);
}

void ListenerComponent::Init()
{
	sceneNode->GetGame()->GetSoundManager()->AddListener(sceneNode);
}
