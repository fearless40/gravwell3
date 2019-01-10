#pragma once


namespace Graphics::D3D11 {
	class RenderQue {
	private:
		comptr<ID3D11DeviceContext> mRender{ nullptr };
		Driver				*		mDevice{ nullptr };
		comptr<ID3D11Buffer>		mPerGame{ nullptr };
		comptr<ID3D11Buffer>		mPerFrame{ nullptr };
		comptr<ID3D11Buffer>		mPerItem{ nullptr };
		comptr<ID3D11Buffer>		mUser{ nullptr };
	
	public:
		RenderQue(Driver * device, comptr<ID3D11DeviceContext> context);
		
		void InitalizeQue();

		void setWorldView(DirectX::XMFLOAT4X4 matrix);
		void setProjection(DirectX::XMFLOAT4X4 matrix);
		void setTimeElapsed(float time, float time2);

		void setGobalLight(DirectX::XMFLOAT4 position, DirectX::XMFLOAT4 colors);


		void BeginRender();

		void RenderModel(Model & md, FLOAT4 position, FLOAT4 rotation);

		void EndRender();
	};
}