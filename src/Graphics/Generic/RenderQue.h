#pragma once


class RenderQue {
public:

	template <typename Textures>
	void setTexture(Textures const & textures);

	template< typename Models >
	void draw(Models & model, Matrix const & matrix);

	template <typename Model, typename MatrixRange>
	void drawInstanced(Model const & model, MatrixRange const & mRange);

	template< typename Shaders>
	void setShaders(Shaders const & shaders);


	template <typename Matrix>
	void setProjection(Matrix const & matrix);

	template <typename Matrix>
	void setViewMatrix(Matrix const & matrix);

	
	void setPerFrameData(void * data, std::size_t size);

	void setSingleEntityData(void * data, std::size_t size);

	//void optimize();
	//void clear();

	//void render();

};