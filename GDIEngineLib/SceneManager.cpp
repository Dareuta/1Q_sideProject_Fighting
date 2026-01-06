//======================================================================
//SceneManager.cpp
//======================================================================
#include "pch.hpp"
#include "SceneManager.hpp"
#include "Scene.hpp"


void SceneManager::Startup()
{
	
}

void SceneManager::Shutdown()
{
	for (auto& scene : m_vScenes)
	{
		delete scene;
	}
	m_vScenes.clear();
}

void SceneManager::Update()
{
	if (m_pNextScene)
	{
		if (m_pCurrentScene)
			m_pCurrentScene->Exit();

		m_pCurrentScene = m_pNextScene;
		m_pNextScene = nullptr;

		m_pCurrentScene->Enter();
	}

	if (m_pCurrentScene)
		m_pCurrentScene->Update(); //  scene에 Update 호출하는거임
}

void SceneManager::Render()
{
	if (m_pCurrentScene)
		m_pCurrentScene->Render(); // scene에 Render 호출하는거임
}

void SceneManager::ChangeScene(size_t index)
{
	if (index >= m_vScenes.size())
		return;

	m_pNextScene = m_vScenes[index];
}

void SceneManager::SetAllOffsetY(float offsetY) {
	if (!m_pCurrentScene) 
		return;

	auto& objects = m_pCurrentScene->GetObjects();
	for (auto& obj : objects) {
		float x = obj->GetMoveX();
		float y = obj->GetMoveY();
		obj->SetPos(x, y + offsetY);
	}
}