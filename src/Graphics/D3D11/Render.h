#pragma once

#include "../Generic/Color.h"
#include "Vertexs.h"
#include "Types.h"


namespace Graphics::D3D11 {
	class Driver;
	using RawMemory = gsl::span<const std::byte>;
	class Render {
	
	public:
		comptr<ID3D11DeviceContext> mRender{ nullptr };
		Driver				*		mDevice{ nullptr };
	
		Render() {}

		Render(Driver * device) :
			mDevice{ device },
			mRender{ device->getContext_comptr() }
		{}

		void setDevice(Driver * device) {
			mDevice = device;
			mRender = nullptr;
			mRender = device->getContext_comptr();
		}

		ConstantBuffer createBuffer(void * mem, std::size_t memSize,
			D3D11_USAGE bufferMemoryType,
			unsigned int bindFlags,
			unsigned int CPUAccessFlags);

		ConstantBuffer createConstantBuffer(RawMemory memory) {
			return createBuffer(
				static_cast<void*>(const_cast<std::byte *>(memory.data())), memory.size_bytes(),
				D3D11_USAGE_DYNAMIC,
				D3D11_BIND_CONSTANT_BUFFER,
				D3D11_CPU_ACCESS_WRITE);
		}

		template<typename BufferValues>
		ConstantBuffer createConstantBuffer(BufferValues & bf) {
			return mDevice->createConstantBuffer({ &bf, sizeof(BufferValues) });
		}

		template<typename BufferTraitsT>
		VertexBuffer createVertexBuffer(RawMemory memory) {
			using BT = Graphics::D3D11::BufferTraits::BufferTraits< BufferTraitsT>;
			return createBuffer(static_cast<void*>(const_cast<std::byte *>(memory.data())), memory.size_bytes(),
				BT::Binding,
				D3D11_BIND_VERTEX_BUFFER,
				BT::CPU);
		}

		template <typename BufferTraitsT>
		IndexBuffer createIndexBuffer(RawMemory memory) {
			using BT = Graphics::D3D11::BufferTraits::BufferTraits< BufferTraitsT>;
			return createBuffer(static_cast<void*>(const_cast<std::byte *>(memory.data())), memory.size_bytes(),
				BT::Binding,
				D3D11_BIND_INDEX_BUFFER,
				BT::CPU);
		}


		template <typename BufferBagType, typename ... ValueTypes>
		void initalizeConstantBufferBag(BufferBagType & bag, ValueTypes ... values) {
			(bag.get_comptr<ValueTypes> = nullptr), ...;
			(bag.get_comptr<ValueTypes> = mDevice->createConstantBuffer({ &values, sizeof(ValueTypes) })), ...;
			
		}

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

		template <typename BufferBagType, typename BufferType>
		// requires std::in_type_list<CbBufferType,BufferTypes...>
		void writeBuffer(BufferBagType & bag, BufferType const & value) {
			writeBuffer(bag.get<BufferType>(), buf);
		}

		void clear(Graphics::Generic::RGBA color = { 0.f,0.f,0.f,0.f }) {
			mRender->ClearRenderTargetView(mDevice->getRenderTargetView(), color.cdata());
			mRender->ClearDepthStencilView(mDevice->getDepthStencil(), D3D11_CLEAR_DEPTH, 1.0f, 0);
		}

		template <typename MeshType> 
		void initalizeIA() {
			using vt = VertexDescription<typename MeshType::vertex_type>;
			mRender->IASetPrimitiveTopology(Topology(md));
			mRender->IASetInputLayout(vt::getInputLayout());
		}

		template <typename BufferBag>
		void initalizeVS(BufferBag & bag) {
			std::array<ID3D11Resource*, bag.size()> buffers;
			std::transform(bag.begin(), bag.end(), buffers.begin(), [](auto & item) { return item.get(); });
			mRender->VSSetConstantBuffers(0, buffers.size(), buffers.data());
		}

		template <typename BufferBag>
		void initalizePS(BufferBag & bag) {
			std::array<ID3D11Resource*, bag.size()> buffers;
			std::transform(bag.begin(), bag.end(), buffers.begin(), [](auto & item) { return item.get(); });
			mRender->PSSetConstantBuffers(0, buffers.size(), buffers.data());
		}

		void meshBind(VertexBuffer const & vb, IndexBuffer const & ib, unsigned int vbStride, DXGI_FORMAT ibStride) {
			std::array<ID3D11Buffer *, 1> vBuffers{ vb.get() };
			mRender->IASetVertexBuffers(0, 1, vBuffers.data(), &vbStride, nullptr);
			mRender->IASetIndexBuffer(ib.get(), ibStride,0);
		}

		template <typename MeshType>
		void meshBind(MeshType & const mesh_value) {
			using vt = VertexDescription<typename MeshType::vertex_type>;
			meshBind(mesh_value.vertexBuffer, mesh_value.indexBuffer, vt::stride(), IndexSize<MeshType>());
		}

		template< typename VertexType, typename IndexType >
		void meshBind(VertexBuffer & vb, IndexBuffer & ib) {
			using vt = VertexDescription<Vertex>;
			meshBind(vb, ib, vt::stride(), IndexSize<MeshType>());
		}

		template<typename MeshType, typename BufferBagType, typename BufferType>
		void meshBind(MeshType & const mesh, BufferBagType & bag, BufferType & const cbValue) {
			meshBind(std::forward<MeshType>(mesh));
			writeBuffer(std::forward<BufferBagType>(bag), std::forward<BufferType>(cbValue));
		}

		void meshRender(std::size_t count, std::size_t start = 0) {
			mRender->DrawIndexed(count, start, 0);
		}

		template<typename MeshType>
		void meshRender(MeshType & const mesh) {
			meshRender(mesh.index_count, mesh.index_start);
		}

		

		void present() {
			mDevice->present();
		}
	};
}