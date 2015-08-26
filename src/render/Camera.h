#pragma once



class Camera
{
public:
	Camera(void);
	~Camera(void);

	void lookAt( DirectX::XMFLOAT3 pos );
	void lookAt( float x, float y, float z );

	void setUpVector( DirectX::XMFLOAT3 up );
	void setUpVector( float x, float y, float z);

	void setPosition( float x, float y, float z );
	void setPosition( DirectX::XMFLOAT3 pos );

	void setRotationEuler( DirectX::XMFLOAT3 rotEuler );
	void setRotationEuler( float pitch, float yaw, float roll );

	void setRotationQuat( DirectX::XMFLOAT4 rotQuat  );
	void setRotationQuat( float x, float y, float z, float w  );


	void setPerspective( float width, float height, float near, float far );
	void setPerspectiveFOV( float fovAngleY, float aspectRation, float near, float far );

	void setOrthographic( float width, float height, float near, float far );

	DirectX::XMMATRIX getView();
	DirectX::XMMATRIX getProj();
	DirectX::XMMATRIX getViewProj();

	DirectX::XMFLOAT4X4 & getViewData( DirectX::XMFLOAT4X4 * value );
	DirectX::XMFLOAT4X4 & getProjData( DirectX::XMFLOAT4X4 * value );
	DirectX::XMFLOAT4X4 & getViewProjData( DirectX::XMFLOAT4X4 * value );

protected:

	__declspec(align(16)) struct privateData {
		DirectX::XMVECTOR mUp;
		DirectX::XMVECTOR mPos;
		DirectX::XMVECTOR mRotQuat;
		DirectX::XMVECTOR mLookAt;

		DirectX::XMMATRIX mViewMatrix;
		DirectX::XMMATRIX mProjMatrix;
		DirectX::XMMATRIX mViewProjMatrixT;	//T is that the matrix is transposed
	};

	privateData * mpd;

	void calcView();
	void calcViewProj();
};

