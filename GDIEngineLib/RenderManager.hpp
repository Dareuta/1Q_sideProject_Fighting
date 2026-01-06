//======================================================================
//RenderManager.hpp
//======================================================================
#pragma once
#include "Singleton.hpp"
#include "framework.hpp" //무야호~
#pragma comment(lib, "gdiplus.lib")

struct FrameInfo {
	bool isFirstTime = true;
	int maxFrame = 0;
	int currentFrame = 0;
	int localFrameCounter = 0;
	float elapsedTime = 0.0f;
	float frameDuration = 0.1f;
};


class RenderManager : public Singleton<RenderManager>
{
private:
	int m_width;
	int m_height;
	HWND m_hWnd;
	HDC m_FrontBufferDC;    // 앞면 DC
	HDC m_BackBufferDC;    // 뒷면 DC
	HBITMAP m_BackBufferBitmap;
	int frameRate = 5; // 애니메이션 재생 빈도	

	int m_CameraOffsetX = 0;
	int m_CameraOffsetY = 0;
	float m_CenterX = 0.0f;

	std::unordered_map<std::wstring, FrameInfo> m_FrameInfo;

	ULONG_PTR m_GdiPlusToken;
	Gdiplus::GdiplusStartupInput gsi;
	Gdiplus::Graphics* m_pBackBufferGraphics;


	struct SpriteData {
		std::vector<Gdiplus::Bitmap*> bitmaps;
		int offsetX = 0;
		int offsetY = 0;
	};

	int m_StandardOffsetX = 130; //87, 234
	int m_StandardOffsetY = 351; // 327, 274

	std::unordered_map<std::wstring, SpriteData> m_BitmapsGroups;

public:
	RenderManager() = default;
	~RenderManager() = default;

	void SetCameraOffsetX(int x) { m_CameraOffsetX = x; }
	int GetCameraOffsetX() const { return m_CameraOffsetX; }

	void SetCenterX(float x) { m_CenterX = x; }
	float GetCenterX() { return m_CenterX; }

	Vector2 WorldToScreen(const Vector2& worldPos) const {
		return worldPos - Vector2(m_CameraOffsetX, m_CameraOffsetY);
	}


	
	int GetFrameRate() const { return frameRate; }	

	void SetCameraOffsetY(int y) { m_CameraOffsetY = y; }
	int GetCameraOffsetY() const { return m_CameraOffsetY; }

	void Startup(HWND hWnd, int Width, int Height);
	void Shutdown();
	void DrawImage(const std::wstring& Key, int Index, int posX, int posY, bool flipX = false);

	HDC GetBackBufferDC() { //디버그용
		return m_BackBufferDC;
	}

	bool LoopImage(const std::wstring& key, bool isLoop, int posX, int posY, bool flipX = false);
	void LoopImageReset(const std::wstring& key);
	bool LoopImageInstance(const std::wstring& key, FrameInfo& info, int posX, int posY, bool flipX = false);

	void SetStandardOffset(int offsetX, int offsetY) {
		m_StandardOffsetX = offsetX;
		m_StandardOffsetY = offsetY;
	}

	int GetStandardOffsetX() const {
		return m_StandardOffsetX;
	}
	int GetStandardOffsetY() const {
		return m_StandardOffsetY;
	}
	void DrawColliderBox(const RECT& rect, Gdiplus::Color color = Gdiplus::Color(255, 255, 0, 0));
	void DrawHealthBar(int x, int y, int width, int height, int current, int max, Gdiplus::Color fillColor, bool reversed = false);

	float GetCurrentX(const std::wstring& key, int x) {
		return m_BitmapsGroups[key].offsetX + x;
	}

	float GetCurrentY(const std::wstring& key, int y) {
		return m_BitmapsGroups[key].offsetY + y;
	}

	void BeginDraw();
	void EndDraw();
	void SetFrame(int rate) {
		frameRate = rate;
	}
	int GetFrameMax(const std::wstring& Key);
	SIZE GetBitmapSize(const std::wstring& key, int frameIndex);
	Gdiplus::Bitmap* GetBitmap(const std::wstring& key, int frameIndex);
	std::vector<Gdiplus::Bitmap*>& GetBitmaps(const std::wstring& key);

	void SetCurrentFrame(std::wstring& key, int n) {
		m_FrameInfo[key].currentFrame = n;
	}

	void SetBitmaps(const std::vector<Gdiplus::Bitmap*>& bitmaps, const std::wstring& key)
	{
		m_BitmapsGroups[key].bitmaps = bitmaps;
	}

	void SetOffset(const std::wstring& key, int x, int y) {
		m_BitmapsGroups[key].offsetX = x;
		m_BitmapsGroups[key].offsetY = y;
	}

	int GetOffsetX(const std::wstring& key) {
		return m_BitmapsGroups[key].offsetX;
	}

	Gdiplus::Bitmap* ResizeBitmap(Gdiplus::Bitmap* source, int width, int height);
	void ResizeBitmapGroup(const std::wstring& key, int width, int height);

	void DrawTextSimple(const std::wstring& text, int x, int y, Gdiplus::Color color = Gdiplus::Color::White);


};