#pragma once
#include "Component.h"
class ListenerComponent :
	public Component
{
public:
	ListenerComponent();
	virtual ~ListenerComponent();

	void Init() override;
};

