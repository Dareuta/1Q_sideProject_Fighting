//======================================================================
//Singleton.hpp
//======================================================================
#pragma once

template <typename T>
class Singleton {
public:
	Singleton()
	{
		assert(s_instance == nullptr && "Singleton instance already created!");
		s_instance = static_cast<T*>(this);
	}

	virtual ~Singleton() = default;

	// 복사 및 이동 금지
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	Singleton(Singleton&&) = delete;
	Singleton& operator=(Singleton&&) = delete;

	static T& Get()
	{
		assert(s_instance != nullptr && "Singleton instance not created!");
		return *s_instance;
	}
private:
	static T* s_instance;
};

template <typename T>
T* Singleton<T>::s_instance = nullptr;