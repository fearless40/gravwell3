#pragma once


enum class RenderQueBinds {
	VertexShader,
	PixelShader,
	GeometryShader
};

class RenderQue {
public:
	void draw(Model, Texture, Shaders, Matrix);

	void drawInstanced(Model, Texture, Shaders, std::span<Matrix>);

	void writeBuffer(Buffer, gsl::span<unsigned char>);
	void writeBufferAndSet(Buffer, gsl::span<unsigned char>, RenderQueBinds place, unsigned int position = 0);
	void bindBuffer(Buffer, RenderQueBinds place, unsigned int position = 0);

	void bindRenderState(RenderState);

	void optimize(); 
	void clear();

	setReservedConstantBuffer(RenderQueBinds place, unsigned int nbr = 2);

private:
	std::set<Texture &> textures;
	std::set<Model &> models;
	std::set<Shaders &> shaders;

	

	struct instruction {

	};

};