#pragma once

class RenderingManager;
class PhysicsManager;
class SoundManager;
class SceneGraph;
class Game;

class Scene
{
public:
	Scene(Game* newGame);
	virtual ~Scene();

	virtual void Init();
	virtual void Render();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Shutdown();


	RenderingManager* GetRenderManager();
	PhysicsManager* GetPhysicsManager();
	SoundManager* GetSoundManager();

	SceneGraph* sceneGraph;

	Game* GetGame();

protected:

	RenderingManager *renderingManager;
	PhysicsManager *physicsManager;
	SoundManager *soundManager;
	Game* game;

private:
	class Camera* camera;
	bool m_isInitialized;
};

