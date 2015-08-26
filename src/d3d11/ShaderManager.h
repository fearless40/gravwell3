#pragma once


struct ID3D11VertexShader;
struct ID3D11PixelShader;
class D3D11;

// Very basic class!
// only a placeholder for something more useful in the future

class ShaderManager
{
public:
	ShaderManager(void);
	~ShaderManager(void);

	ID3D11VertexShader * loadVertexFromFile(D3D11 * d3d, std::wstring filename );
	ID3D11PixelShader * loadPixelFromFile(D3D11 * d3d, std::wstring filename );

protected:
	std::wstring getRelativePath(const std::wstring inputFileName, const std::wstring resourceFolder);
};

