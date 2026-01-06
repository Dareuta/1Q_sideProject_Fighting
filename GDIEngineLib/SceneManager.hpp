//======================================================================
//SceneManager.hpp
//======================================================================
#pragma once

#include "Singleton.hpp"
#include "Object.hpp"

class Scene; // 전. 방. 선. 언
class SceneManager : public Singleton<SceneManager>
{
public:
	SceneManager() = default;			// 다른 생성을 통한 생성자 호출은 금지
	~SceneManager() = default;

	std::vector<Scene*> m_vScenes; // 씬 리스트

	Scene* m_pCurrentScene = nullptr; // 현재 씬
	Scene* m_pNextScene = nullptr; // 다음 씬


	void SetAllOffsetY(float offsetY);

public:
	void Startup();
	void Shutdown();
	void Update();
	void Render();
	void ChangeScene(size_t index);
	Scene* GetCurrentScene() { return m_pCurrentScene; }

	void SetParam(int value) { param = value; }
	int GetParam() const { return param; }

	// CreateScene<TestScene>();
	template<typename T>
	void CreateScene()
	{
		Scene* pScene = new T();
		m_vScenes.push_back(pScene);
	}
	
private:
	int param = 0;
	
};

