#pragma once

#include "../Generic/Color.h"
#include "Vertexs.h"
#include "Types.h"
//#include "ShaderCompiler.h"
#include "BufferTraits.h"

namespace Graphics::D3D11 {
	class Driver;
	using RawMemory = gsl::span<const std::byte>;
	class Render {
	
		void * toVoidPtr(RawMemory memory) {
			return static_cast<void*>(const_cast<std::byte *>(memory.data()));
		}

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

		template< typename VertexDescription >
		void registerVertexDescription(VertexDescription & vd) {
			if constexpr (VertexDescription::requires_compilation::value) {
				auto shader = ShaderCompiler::compile_vertexshader(vd.layoutstring());
				auto inputarr = vd.layoutarray();
				winrt::check_hresult(
					mDevice->get()->CreateInputLayout(inputarr.data(), inputarr.size(), shader->GetBufferPointer(), shader->GetBufferSize(), &vd)
				);
			}
			else {
				auto shader = vd.compiledshader();
				auto inputarr = vd.layoutarray();
				winrt::check_hresult(
					mDevice->get()->CreateInputLayout(inputarr.data(), inputarr.size(), shader.data(), shader.size(), &vd)
					);
			}
		}

		comptr<ID3D11Buffer> createBuffer(void * mem, std::size_t memSize,
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

		VertexShader createVSShader(RawMemory memory) {
			VertexShader shader{ nullptr };
			winrt::check_hresult(
				mDevice->get()->CreateVertexShader(toVoidPtr(memory), memory.size(), nullptr, shader.put())
			);
			return shader;
		}

		PixelShader createPSShader(RawMemory memory) {
			PixelShader shader{ nullptr };
			winrt::check_hresult(
				mDevice->get()->CreatePixelShader(toVoidPtr(memory), memory.size(), nullptr, shader.put())
			);
			return shader;
		}

		template <typename BufferBagType, typename ... ValueTypes>
		void initalizeConstantBufferBag(BufferBagType & bag, ValueTypes ... values) {
			auto setNullPtr = [](auto & bag, auto type) {
				bag.get_comptr<decltype(type)>() = nullptr;
			};

			(setNullPtr(bag, values),...);
		
			auto setValuePtr = [this](auto & bag, auto & value) -> void{ 
				using value_type = typename std::decay_t<decltype(value)>;
				RawMemory rm( (std::byte *)(void*)(&value) , sizeof(value_type) );
				bag.get_comptr<value_type>() = this->createConstantBuffer(rm);
			};
			(setValuePtr(bag, values), ...);
			
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
			writeBuffer(bag.get<BufferType>(), value);
		}

		void clear(Graphics::Generic::RGBA color = { 0.f,0.f,0.f,0.f }) {
			mRender->ClearRenderTargetView(mDevice->getRenderTargetView(), color.cdata());
			mRender->ClearDepthStencilView(mDevice->getDepthStencil(), D3D11_CLEAR_DEPTH, 1.0f, 0);
		}

		template <typename MeshType> 
		void initalizeIA() {
			using vt = VertexDescription<typename MeshType::vertex_type>;
			mRender->IASetPrimitiveTopology(Topology<MeshType>());
			mRender->IASetInputLayout(vt::getInputLayout());
		}

		template <typename BufferBag>
		void initalizeVS(BufferBag & bag) {
			std::array<ID3D11Resource*, BufferBag::size_n::value> buffers;
			std::transform(bag.begin(), bag.end(), buffers.begin(), [](auto & item) { return item.get(); });
			mRender->VSSetConstantBuffers(0, buffers.size(), (ID3D11Buffer * const*) buffers.data());
		}

		template <typename BufferBag>
		void initalizePS(BufferBag & bag) {
			std::array<ID3D11Resource*, BufferBag::size_n::value> buffers;
			std::transform(bag.begin(), bag.end(), buffers.begin(), [](auto & item) { return item.get(); });
			mRender->PSSetConstantBuffers(0, buffers.size(), (ID3D11Buffer * const*) buffers.data());
		}

		
		void meshBind(const VertexBuffer & vb, const IndexBuffer & ib, unsigned int vbStride, DXGI_FORMAT ibStride) {
			auto vBuffers = vb.get();
			unsigned int offsets = 0;
			mRender->IASetVertexBuffers(0, 1, &vBuffers, &vbStride, &offsets);
			mRender->IASetIndexBuffer(ib.get(), ibStride,0);
		}

		template <typename MeshType>
		void meshBind(const MeshType & mesh_value) {
			using vt = VertexDescription<typename MeshType::vertex_type>;
			meshBind(mesh_value.vertexBuffer, mesh_value.indexBuffer, vt::stride(), IndexSize<MeshType>());
		}

		template< typename VertexType, typename IndexType >
		void meshBind(VertexBuffer & vb, IndexBuffer & ib) {
			using vt = VertexDescription<VertexType>;
			meshBind(vb, ib, vt::stride(), IndexSize<IndexType>());
		}

		template<typename MeshType, typename BufferBagType, typename BufferType>
		void meshBind(const MeshType & mesh, BufferBagType & bag, const BufferType & cbValue) {
			meshBind(std::forward<MeshType>(mesh));
			writeBuffer(std::forward<BufferBagType>(bag), std::forward<BufferType>(cbValue));
		}

		void meshRender(std::size_t count, std::size_t start = 0) {
			mRender->DrawIndexed(count, start, 0);
		}

		/*template<typename MeshType>
		void meshRender(const MeshType & mesh) {
			meshRender(mesh.index_count, mesh.index_start);
		}*/

		void vsShaderBind(VertexShader & vs) {
			mRender->VSSetShader(vs.get(), nullptr, 0);
		}

		void psShaderBind(PixelShader & ps) {
			mRender->PSSetShader(ps.get(), nullptr, 0);
		}
		

		void present() {
			mDevice->present();
		}
	};
}