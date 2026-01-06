//======================================================================
//ResourceManager.cpp
//======================================================================
#include "pch.hpp"
#include "ResourceManager.hpp"
#include "RenderManager.hpp"

void ResourceManager::Startup() {

}
void ResourceManager::Shutdown() {
	for (auto& bitmap : m_LoadedBitmaps) {
		delete bitmap;
	}
	m_LoadedBitmaps.clear();

}

void ResourceManager::Update() {

}

void ResourceManager::Clear() {


}

int NumFilter(const std::wstring filename) { //그러니까, filename이 Standing(22)이면, 22를 반환해주는 함수임
	std::wregex numberRegex(L"(\\((\\d+)\\))"); // regex > w뭐시깽 정규식 찾기 >> 패턴 찾는거임
	std::wsmatch match; // wstring에 대한 매칭결과를 저장하는 뭐시깽
	if (std::regex_search(filename, match, numberRegex)) {
		try {
			return std::stoi(match[2]); // match[1]하면 오류남 개에바임
		}
		catch (...) { //에러나면 알아서 잡는다
			return 0;
		}
	}
	return 0;
}

void ResourceManager::LoadPng(const std::wstring& folder, const std::wstring& pattern) {
	std::vector<std::filesystem::directory_entry> fileVec; //directory_entry 이게뭐지 << 파일 하나를 의미하는 객체값

	std::wstring searchPath = L"..\\Resource\\" + folder + L"\\";

	for (const auto& entry : std::filesystem::directory_iterator(searchPath))  // 폴더를 순회하는 for문
		if (entry.is_regular_file()) { // 실체가 있는 파일인 경우에만
			std::wstring filename = entry.path().filename().wstring(); // \뒷부분을 따서 wstring으로 반환
			if (filename.find(pattern) != std::wstring::npos && entry.path().extension() == L".png") // png로 끝나는거 + 패턴있는거
				fileVec.push_back(entry);
		}

	std::sort(fileVec.begin(), fileVec.end(),
		[](const auto& a, const auto& b) { //거지같은 람다함수 보기 힘드네
			return NumFilter(a.path().filename().wstring()) < NumFilter(b.path().filename().wstring()); // 숫자만 뽑아서 대소비교
		});

	std::vector<Gdiplus::Bitmap*> pngs;

	for (const auto& entry : fileVec) {
		Gdiplus::Bitmap* bitmap = new Gdiplus::Bitmap(entry.path().c_str());
		if (bitmap->GetLastStatus() == Gdiplus::Ok)
			pngs.push_back(bitmap);
		else
			delete bitmap;
	}

	PendingBitmaps(pngs);
	RenderManager::Get().SetBitmaps(pngs, pattern);
}

void ResourceManager::PendingBitmaps(const std::vector<Gdiplus::Bitmap*>& bitmaps) {
	for (auto bitmap : bitmaps) {
		m_LoadedBitmaps.push_back(bitmap);
	}
}




