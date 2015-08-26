#pragma once
#include "App.h"


class D3D11App : public AppGame<D3D11App>
{
private:
	struct InternalData;
	
	InternalData * pimpl;


public:
	D3D11App(HINSTANCE hinst);
	~D3D11App(void);

	bool initalize();

	bool Frame();
};

