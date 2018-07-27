#pragma once

namespace Graphics::generic {
	template <typename TextureView, typename BaseClass> 
	struct Textures {
	private:
		constexpr BaseClass * base() { return static_cast<BaseClass*>(this); }
		
		std::size_t size() {
			return base()->size();
		}

		TextureView & getTexture(unsigned int position) {
			base()->getTexture(position);
		}

		auto begin() {
			return base()->begin();
		}

		auto end() {
			return base()->end();
		}
	};

	template <typename TextureView> 
	struct SingleTexture : Textures<TextureView, SingleTexture> {
		TextureView & mTex;

		constexpr std::size_t size() {
			return 1;
		}

		auto begin() {
			return &mTex;
		}

		auto end() {
			return (&mTex) + 1;
		}

		TextureView & getTexture(unsigned int position) {
			return mTex;
		}


	};

	template <typename TextureView>
	struct MultiTexture : Textures<TextureView, MultiTexture> {

		std::vector<TextureView> mTextures;

		std::size_t size() {
			return mTextures.size();
		}

		TextureView & getTexture(unsigned int position) {
			return mTextures[position];
		}

		auto begin() {
			return mTextures.begin();
		}

		auto end() {
			return mTextures.end();
		}

	};
}