#include "dxgi.h"
#include "VideoCard.h"

namespace DX::DXGI {
	
	
	class DXGI {
		ComPtr<IDXGIFactory> factory = nullptr;

	public:
		DXGI() {
			HRESULT result;
			result = CreateDXGIFactory(__uuidof(IDXGIFactory), &factory);
		};

		std::vector<VideoCard> GetVideoCards() {
			std::vector<VideoCard> videocards;
			HRESULT result;
			ComPtr<IDXGIAdapter> videocard;
			unsigned int index = 0;

			while (factory->EnumAdapters(index, &videocard) == S_OK) {
				videocards.emplace_back(videocard, index == 0 ? true : false);
				++index;
			}
			return videocards;
		}

	};
}