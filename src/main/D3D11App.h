#pragma once
#include "App.h"


class D3D11App : public AppGame<D3D11App>
{
private:
	struct InternalData;
	
	std::unique_ptr<InternalData> mImpl;


public:
	D3D11App(HINSTANCE hinst);
	~D3D11App(void);

	bool initalize();

	bool Frame();
};

