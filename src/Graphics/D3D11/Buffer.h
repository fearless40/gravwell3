#pragma once

namespace Graphics::D3D11 {

	/*class Buffer {
	public:
		Buffer() = delete;
		Buffer(comptr<ID3D11Buffer> buffer) : mBuffer{ buffer } {}
		Buffer(ID3D11Buffer * buffer) : mBuffer{buffer} {}

		std::size_t size() {
			D3D11_BUFFER_DESC desc;
			mBuffer->GetDesc(&desc);
			return desc.ByteWidth;
		}
		
		ID3D11Buffer * get() { return mBuffer.get(); }
		operator ID3D11Buffer * () { return mBuffer.get(); }
		operator ID3D11Buffer ** () { return mBuffer.put(); };
	
	private:
		comptr<ID3D11Buffer> mBuffer;
	};*/
}