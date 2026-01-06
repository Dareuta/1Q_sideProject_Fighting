//======================================================================
//Scene.hpp
//======================================================================
#pragma once

class Object; // 전 . 방 . 선 . 언

class Scene
{
private:
	std::vector<Object*> m_Objects;
	std::vector<Object*> m_DeletePendingObjects; // 삭제 예정
public:
	Scene() {


	}
	virtual ~Scene()
	{
		Clear();
	}
	/*
	template<typename T>
	T* CreateObject()
	{
		T* pObject = new T();
		m_Objects.push_back(pObject);
		return pObject;
	}
	*/

	template<typename T, typename... Args>
	T* CreateObject(Args&&... args)
	{
		T* pObject = new T(std::forward<Args>(args)...);
		m_Objects.push_back(pObject);
		return pObject;
	}

	void Clear();
	std::vector<Object*>& GetObjects(){
		return m_Objects;
	}
	virtual void Update();
	virtual void Render();
	virtual void Enter() {};
	virtual void Exit() { Clear(); }
};

