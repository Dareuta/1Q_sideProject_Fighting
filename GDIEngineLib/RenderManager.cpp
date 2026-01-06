//======================================================================
//RenderManager.cpp
//======================================================================
#include "pch.hpp"
#include "RenderManager.hpp"
#include "ResourceManager.hpp"
#include "WinGameApp.hpp" 

#undef min
#undef max
#include <algorithm>

void RenderManager::Startup(HWND hWnd, int Width, int Height) {
	this->m_hWnd = hWnd;
	this->m_width = Width;
	this->m_height = Height;

	m_FrontBufferDC = GetDC(hWnd); //윈도우 클라이언트 영역의 DeviceContext얻기
	m_BackBufferDC = CreateCompatibleDC(m_FrontBufferDC); // 호환되는 DeviceContext 생성
	m_BackBufferBitmap = CreateCompatibleBitmap(m_FrontBufferDC, m_width, m_height); // 메모리 영역생성
	SelectObject(m_BackBufferDC, m_BackBufferBitmap); // MemDC의 메모리영역 지정

	Gdiplus::GdiplusStartup(&m_GdiPlusToken, &gsi, nullptr);
	m_pBackBufferGraphics = Gdiplus::Graphics::FromHDC(m_BackBufferDC);


	//////////////
	/*
	m_pBackBufferGraphics->SetCompositingMode(Gdiplus::CompositingModeSourceCopy);
	m_pBackBufferGraphics->SetCompositingQuality(Gdiplus::CompositingQualityHighSpeed);
	m_pBackBufferGraphics->SetInterpolationMode(Gdiplus::InterpolationModeNearestNeighbor);
	m_pBackBufferGraphics->SetPixelOffsetMode(Gdiplus::PixelOffsetModeNone);
	m_pBackBufferGraphics->SetSmoothingMode(Gdiplus::SmoothingModeNone);
	*/
}

void RenderManager::Shutdown() {
	if (m_pBackBufferGraphics)
	{
		delete m_pBackBufferGraphics;
		m_pBackBufferGraphics = nullptr;
	}

	DeleteObject(m_BackBufferBitmap);
	DeleteDC(m_BackBufferDC);
	ReleaseDC(m_hWnd, m_FrontBufferDC);

	Gdiplus::GdiplusShutdown(m_GdiPlusToken);
	m_BitmapsGroups.clear();
}

int RenderManager::GetFrameMax(const std::wstring& key) {
	auto it = m_BitmapsGroups.find(key);
	if (it != m_BitmapsGroups.end() && !it->second.bitmaps.empty()) //찾는값이 없으면 end()가 됨, seconde > key-value 중 value값을 보겠다
		return static_cast<int>(it->second.bitmaps.size()); // value(vector).size()
	else
		return 0;
}

SIZE RenderManager::GetBitmapSize(const std::wstring& key, int frameIndex) {
	auto it = m_BitmapsGroups.find(key);
	if (it != m_BitmapsGroups.end()) {
		const std::vector<Gdiplus::Bitmap*>& bitmaps = it->second.bitmaps;
		if (frameIndex >= 0 && frameIndex < bitmaps.size()) {
			Gdiplus::Bitmap* bmp = bitmaps[frameIndex];
			if (bmp) {
				SIZE size;
				size.cx = bmp->GetWidth();
				size.cy = bmp->GetHeight();
				return size;
			}
		}
	}
	return SIZE{ 0, 0 };
}

void RenderManager::DrawImage(const std::wstring& key, int index, int posX, int posY, bool flipX) {

	Gdiplus::Bitmap* frameBmp = m_BitmapsGroups[key].bitmaps[index];

	int height = frameBmp->GetHeight();
	int width = frameBmp->GetWidth();

	int offsetX = m_BitmapsGroups[key].offsetX;
	int offsetY = m_BitmapsGroups[key].offsetY;

	int xFix = m_StandardOffsetX - offsetX; // 기준 피벗에서 나는 차이
	int drawX = posX + xFix;

	int yFix = m_StandardOffsetY - offsetY;
	int drawY = posY + yFix;

	if (flipX) {
		float pivotX = static_cast<float>(drawX + offsetX);
		Gdiplus::Matrix mirrorX;

		mirrorX.Translate(pivotX, 0);
		mirrorX.Scale(-1.0f, 1.0f); // 반대로 이동함
		mirrorX.Translate(-pivotX, 0);

		m_pBackBufferGraphics->SetTransform(&mirrorX);
		m_pBackBufferGraphics->DrawImage(frameBmp, drawX, drawY);
		m_pBackBufferGraphics->ResetTransform();
	}
	else {

		m_pBackBufferGraphics->DrawImage(frameBmp, drawX, drawY);
	}
}

bool RenderManager::LoopImage(const std::wstring& key, bool isLoop, int posX, int posY, bool flipX) {
	auto& info = m_FrameInfo[key];
	info.localFrameCounter++;

	if (info.isFirstTime) {
		info.maxFrame = GetFrameMax(key); //key가 유효하지 않으면 터짐 알아만 두자 
		info.isFirstTime = false;
	}

	DrawImage(key, info.currentFrame, posX, posY, flipX); // posX, Y는 입력에 따라 변하는 값임

	if (info.localFrameCounter >= frameRate) {
		info.localFrameCounter = 0;
		info.currentFrame++;
	}

	if (info.currentFrame >= info.maxFrame) { //maxFrame = size > -1값이 최대 index
		if (isLoop)
			info.currentFrame = 0;
		else {
			//마지막 프레임 반복		
			info.currentFrame = info.maxFrame - 1;
			return true; // 끝났다
		}
	}
	return false; // 아직이다
}

bool RenderManager::LoopImageInstance(const std::wstring& key, FrameInfo& info, int posX, int posY, bool flipX) {
	info.localFrameCounter++;

	if (info.isFirstTime) {
		info.maxFrame = GetFrameMax(key);
		info.isFirstTime = false;
	}

	DrawImage(key, info.currentFrame, posX, posY, flipX);

	if (info.localFrameCounter >= frameRate) { // 전역 속도 기준 사용
		info.localFrameCounter = 0;
		info.currentFrame++;
	}

	if (info.currentFrame >= info.maxFrame) {
		info.currentFrame = 0;
		return true;
		//	info.currentFrame = info.maxFrame - 1; 마지막프레임으로 고정하는 부분
	}
	return false;
}

void RenderManager::LoopImageReset(const std::wstring& key) {
	m_FrameInfo[key] = { true , 0, 0 };
	SIZE size = GetBitmapSize(key, 0);
}

void RenderManager::BeginDraw()
{
	PatBlt(m_BackBufferDC, 0, 0, m_width, m_height, BLACKNESS);
}

void RenderManager::EndDraw()
{
	BitBlt(m_FrontBufferDC, 0, 0, m_width, m_height, m_BackBufferDC, 0, 0, SRCCOPY);
}

Gdiplus::Bitmap* RenderManager::GetBitmap(const std::wstring& key, int frameIndex)
{
	auto it = m_BitmapsGroups.find(key);
	if (it != m_BitmapsGroups.end()) {
		const auto& bitmaps = it->second.bitmaps;
		if (frameIndex >= 0 && frameIndex < bitmaps.size()) {
			return bitmaps[frameIndex];
		}
	}
	return nullptr;
}

std::vector<Gdiplus::Bitmap*>& RenderManager::GetBitmaps(const std::wstring& key) {
	return m_BitmapsGroups[key].bitmaps;
}


Gdiplus::Bitmap* RenderManager::ResizeBitmap(Gdiplus::Bitmap* source, int width, int height) {
	if (!source)
		return nullptr;

	Gdiplus::Bitmap* resized = new Gdiplus::Bitmap(width, height, PixelFormat32bppARGB);

	Gdiplus::Graphics graphics(resized);
	graphics.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
	graphics.DrawImage(source, 0, 0, width, height);

	return resized;
}

void RenderManager::ResizeBitmapGroup(const std::wstring& key, int width, int height) {
	auto it = m_BitmapsGroups.find(key);
	if (it == m_BitmapsGroups.end())
		return;

	auto& bitmaps = it->second.bitmaps;
	std::vector<Gdiplus::Bitmap*> resizedBitmaps;

	for (auto bmp : bitmaps) {
		if (bmp) {
			Gdiplus::Bitmap* resized = ResizeBitmap(bmp, width, height);
			if (resized) {
				resizedBitmaps.push_back(resized);
			}
		}
	}


	bitmaps = std::move(resizedBitmaps); // 새로운 리사이즈된 비트맵으로 교체
}

void RenderManager::DrawTextSimple(const std::wstring& text, int x, int y, Gdiplus::Color color) {
	Gdiplus::FontFamily fontFamily(L"Arial");
	Gdiplus::Font font(&fontFamily, 16);
	Gdiplus::SolidBrush brush(color);

	Gdiplus::PointF point(static_cast<Gdiplus::REAL>(x), static_cast<Gdiplus::REAL>(y));
	m_pBackBufferGraphics->DrawString(text.c_str(), -1, &font, point, &brush);
}

void RenderManager::DrawColliderBox(const RECT& worldRect, Gdiplus::Color color) {
	int cameraX = GetCameraOffsetX();
	int cameraY = GetCameraOffsetY();

	int screenX = worldRect.left - cameraX;
	int screenY = worldRect.top - cameraY;
	int width = worldRect.right - worldRect.left;
	int height = worldRect.bottom - worldRect.top;

	Gdiplus::Pen pen(color, 2);
	m_pBackBufferGraphics->DrawRectangle(&pen, screenX, screenY, width, height);
}

void RenderManager::DrawHealthBar(int x, int y, int width, int height, int current, int max, Gdiplus::Color fillColor, bool reversed) {
	float ratio = static_cast<float>(current) / max;
	int filledWidth = static_cast<int>(width * ratio);

	Gdiplus::SolidBrush bgBrush(Gdiplus::Color(50, 50, 50)); // 배경 (회색)
	Gdiplus::SolidBrush hpBrush(fillColor);                  // 체력 (빨강/파랑)

	// 체력바 배경
	m_pBackBufferGraphics->FillRectangle(&bgBrush, x, y, width, height);
	// 체력 채운 부분
	if (reversed) {
		// 왼쪽 → 오른쪽 체력바
		m_pBackBufferGraphics->FillRectangle(&hpBrush, x, y, filledWidth, height);
	}
	else {
		// 오른쪽 → 왼쪽 체력바
		int startX = x + (width - filledWidth);
		m_pBackBufferGraphics->FillRectangle(&hpBrush, startX, y, filledWidth, height);
	}
}



