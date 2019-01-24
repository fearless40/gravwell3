#pragma once

#include "../Generic/Color.h"
#include "Vertexs.h"
#include "Types.h"

namespace Graphics::D3D11 {
	class Driver;

	namespace util {

		template <class T, class Tuple>
		struct Index;

		template <class T, class... Types>
		struct Index<T, std::tuple<T, Types...>> {
			static const std::size_t value = 0;
		};

		template <class T, class U, class... Types>
		struct Index<T, std::tuple<U, Types...>> {
			static const std::size_t value = 1 + Index<T, std::tuple<Types...>>::value;
		};
	}

	template <typename ... BufferTypes>
	class Render {
	
	public:
		comptr<ID3D11DeviceContext> mRender{ nullptr };
		Driver				*		mDevice{ nullptr };
	
	private:
		std::array< comptr<ID3D11Buffer>, sizeof...(BufferTypes)> cbBuffers;
		
		template<typename BufferType>
		constexpr int get_cb_index() const {
			using types_t = std::tuple<BufferTypes...>;
			return util::Index<BufferType, types_t>::value;
		}



	public:
		template<typename BufferType>
		ID3D11Buffer * getCbBuffer() {
			return cbBuffers[get_cb_index<BufferType>()].get();
		}

		template<typename BufferType>
		constexpr comptr<ID3D11Buffer> & getCbBufferComptr() {
			return cbBuffers[get_cb_index<BufferType>()];
		}


		template<typename BufferValues>
		void createCB(BufferValues bf) {
			getCbBufferComptr<BufferValues>() = mDevice->createConstantBuffer({ &bf, sizeof(BufferValues) });
		}
		
		Render() {}

		Render(Driver * device, comptr<ID3D11DeviceContext> context) :
			mDevice{ device },
			mRender{ context }
		{}

		inline void writeBuffer(ID3D11Resource * resource, void const * data, std::size_t size) {
				D3D11_MAPPED_SUBRESOURCE rawPtr;
				mRender->Map(resource, 0, D3D11_MAP_WRITE_DISCARD, 0, &rawPtr);
				memcpy(rawPtr.pData, data, size);
				mRender->Unmap(resource, 0);
		}

		template <typename T>
		void writeBuffer(ID3D11Resource * resource, T const & item) {
			writeBuffer(resource, static_cast<void const*>(&item), sizeof(T));
		}

		
		void clear() {
			float color[4] = { 0.0f,0.0f,0.0f,0.0f };

			// Clear the back buffer.
			mRender->ClearRenderTargetView(mDevice->mRenderTargetView.get(), color);

			// Clear the depth buffer.
			mRender->ClearDepthStencilView(mDevice->mDepthStencilView.get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
		}

		void clear(Graphics::Generic::RGBA color) {
			// Clear the back buffer.
			mRender->ClearRenderTargetView(mDevice->mRenderTargetView.get(), color.cdata());

			// Clear the depth buffer.
			mRender->ClearDepthStencilView(mDevice->mDepthStencilView.get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

		}

		template <typename MeshTraits> 
		void meshSetupIA() {
			using vt = VertexDescription<typename MeshType::vertex_type>;
			mRender->IASetPrimitiveTopology(Topology(md));
			mRender->IASetInputLayout(vt::getInputLayout());
		}

		template<typename MeshType>
		void meshBindAndRender(MeshType const & md) {
			using vt = VertexDescription<typename MeshType::vertex_type>;
			mRender->IASetVertexBuffers(0, 1, md.vertexBuffer.put_void(), vt::stride(), nullptr);
			mRender->IASetIndexBuffer(md.indexBuffer.put_void(), IndexSize<MeshType>() );
			meshRender(md.index_count, md.index_start);
		}
		//void materialBind(Material & mat);

		template< typename VertexType, typename IndexType >
		void meshBind(VertexBuffer & vb, IndexBuffer & ib) {
			using vt = VertexDescription<Vertex>;
			mRender->IASetVertexBuffers(0, 1, vb.put_void(), vt::stride(), nullptr);
			mRender->IASetIndexBuffer(ib.put_void(), IndexSize<IndexType>());
		}

		void meshRender(std::size_t count, std::size_t start = 0) {
			mRender->DrawIndexed(count, start, 0);
		}

		void present() {
			mDevice->present();
		}
	};
}