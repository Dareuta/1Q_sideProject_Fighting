//======================================================================
//Scene.cpp
//======================================================================
#include "pch.hpp"
#include "Scene.hpp"
#include "Object.hpp"

void Scene::Clear()
{
	for (auto & object : m_Objects)
	{
		delete object;
	}
	m_Objects.clear();
	for (auto& object : m_DeletePendingObjects)
	{
		delete object;
	}
	m_DeletePendingObjects.clear();
}

void Scene::Update()
{
	for (auto& object : m_Objects)
	{
		object->Update();
	}
}

void Scene::Render()
{
	std::sort(m_Objects.begin(), m_Objects.end(),
		[](Object* a, Object* b) {
			return a->GetLayer() < b->GetLayer();
		});



	for (auto& object : m_Objects)
	{
		object->Render();
	}
}
