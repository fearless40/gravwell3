#include "dxgi.h"
#include "VideoCard.h"

namespace DX::DXGI {
	
	
	class DXGI {
		comptr<IDXGIFactory> factory = nullptr;

	public:
		DXGI() {
			HRESULT result;
			result = CreateDXGIFactory(__uuidof(IDXGIFactory), factory.put_void());
		};

		std::vector<VideoCard> GetVideoCards() {
			std::vector<VideoCard> videocards;
			comptr<IDXGIAdapter> videocard;
			unsigned int index = 0;

			while (factory->EnumAdapters(index, videocard.put()) == S_OK) {
				videocards.emplace_back(videocard, index == 0 ? true : false);
				++index;
			}
			return videocards;
		}

	};
}