#pragma once

#include "../Generic/Color.h"
#include "Vertexs.h"

namespace Graphics::D3D11 {
	class Render {
	private:
		struct GlobalLight {
			DirectX::XMFLOAT4	position;
			DirectX::XMFLOAT4	color;
		};

		comptr<ID3D11DeviceContext> mRender{ nullptr };
		Driver				*		mDevice{ nullptr };
		comptr<ID3D11Buffer>		mPerGame{ nullptr };
		comptr<ID3D11Buffer>		mPerFrame{ nullptr };
		comptr<ID3D11Buffer>		mPerItem{ nullptr };
		comptr<ID3D11Buffer>		mUser{ nullptr };


		bool						mUpdatePerGame{ true };

		DirectX::XMFLOAT4X4			mProjection;
		GlobalLight					mGlobalLight;

		// Storage of data
		uint32_t					mIndecies_start;
		uint32_t					mIndecies_count;
	
	public:
		Render(Driver * device, comptr<ID3D11DeviceContext> context);
		
		void InitalizeQue();

		void setProjection(DirectX::XMFLOAT4X4 matrix);
		void setGobalLight(DirectX::XMFLOAT4 position, DirectX::XMFLOAT4 colors);


		void begin(float TimeSinceLastFrame, float TimeSinceStart);
		void clear();
		void clear(Graphics::Generic::RGBA color);
		template<typename MeshType>
		void meshBind(MeshType const & md) {
			using vt = VertexDescription<MeshType::VertexTypeT>;
			//For now just set the topology will bench mark it
			// Todo: bench mark changing the topology per item
			mRender->IASetPrimitiveTopology(Topology(md));
			mRender->IASetInputLayout(vt::getInputLayout());
			mRender->IASetVertexBuffers(0, 1, md.vertexBuffer.put_void(), vt::stride(), nullptr);
			mRender->IASetIndexBuffer(md.indexBuffer.put_void(), IndexSize(md));
			mIndecies_start = md.index_start;
			mIndecies_count = md.index_count;
		}
		//void materialBind(Material & mat);
		void drawMesh(DirectX::XMFLOAT4X4 worldview);
		void end(bool autoPresent = true);
	};
}