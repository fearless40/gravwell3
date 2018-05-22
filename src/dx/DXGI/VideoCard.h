#include "dxgi.h"
#include <vector>
#include <memory>


namespace DX::DXGI {
	class VideoCard {
		ComPtr<IDXGIAdapter>	mAdapter;
		DXGI_ADAPTER_DESC		mDesc;
		bool					mIsPrimary = false;

		struct Displays {
			ComPtr<IDXGIOutput1> screen;
			std::unique_ptr<DXGI_MODE_DESC> modes[];
		};

		std::vector<Displays> mDisplays;

	public:
		VideoCard() = delete;
		VideoCard(ComPtr<IDXGIAdapter> adapter, bool PrimaryAdapter = false) : mAdapter(adapter) {
			adapter->GetDesc(&mDesc);
			GetScreensAttachedToAdapter();
		}

		bool isPrimary() const noexcept { return mIsPrimary; }

		std::size_t getNumberOfScreens() const noexcept { return mDisplays.size(); }

		void findDisplayMode(unsigned width, unsigned height, std::size_t screen = 0) {
			DXGI_MODE_DESC1 desc;
			DXGI_MODE_DESC1 outpt;
			desc.Width = width;
			desc.Height = height;
			desc.RefreshRate.Denominator = 0;
			desc.RefreshRate.Numerator = 0;
			desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			desc.Stereo = FALSE;
			desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			desc.Format = DX::DXGI::ScreenFormat;
			mDisplays.at(screen).screen->FindClosestMatchingMode1(&desc, &outpt, NULL);
		}

	private:
		void GetScreensAttachedToAdapter() {
			unsigned int i = 0;
			ComPtr<IDXGIOutput>		output;
			while (mAdapter->EnumOutputs(0, &output) != DXGI_ERROR_NOT_FOUND) {
				ComPtr<IDXGIOutput1> output1;
				output.As(&output1);

				Displays ai;
				ai.screen = output1;
				mDisplays.push_back(std::move(ai));
				++i;
			}
		};

		void GetScreenModes(std::size_t screen) {
			unsigned int numModes = 0;

			auto result = mDisplays.at(screen).screen->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
			if (FAILED(result))
			{
				//return false;
			}

			//ai.modes = new DXGI_MODE_DESC[numModes];

			//auto result = mDisplays.at(screen).screen->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, ai.modes->get());
		};


	};
};