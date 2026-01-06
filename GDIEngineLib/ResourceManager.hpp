//======================================================================
//ResourceManager.hpp
//======================================================================
#pragma once
#include "Singleton.hpp"
#include "framework.hpp"


class ResourceManager : public Singleton<ResourceManager>
{
private:
	std::vector<Gdiplus::Bitmap*> m_LoadedBitmaps;
public:
	ResourceManager() = default;
	~ResourceManager() {
		Clear();
	}
	void Startup();
	void Shutdown();
	void Update();
	void Clear();
	void PendingBitmaps(const std::vector<Gdiplus::Bitmap*>& bitmaps);
	void LoadPng(const std::wstring& folder, const std::wstring& pattern);		
};

