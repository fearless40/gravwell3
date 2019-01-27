#include "../stdafx.h"
#include "Camera.h"


Camera::Camera(void) 
{
	mpd = (privateData*) _aligned_malloc(sizeof(privateData),16);
	mpd->mUp = DirectX::XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
	mpd->mPos = DirectX::XMVectorReplicate( 0.0f );
	mpd->mRotQuat = DirectX::XMQuaternionRotationRollPitchYaw( 0.5f, 0.5f, 0.5f );
	mpd->mLookAt = DirectX::XMVectorSet( 0.0f, 0.0f, 1.0f, 0.0f );
}


Camera::~Camera(void)
{
	_aligned_free( mpd );
}


void Camera::calcView()
{
	DirectX::XMVECTOR rup = mpd->mUp, rlook = mpd->mLookAt;

	// Rotate the look and the up position to have the camera rotate
	DirectX::XMVector3Rotate( rup, mpd->mRotQuat );
	DirectX::XMVector3Rotate( rlook, mpd->mRotQuat );

	// Generate the view matrix
	mpd->mViewMatrix = DirectX::XMMatrixLookAtLH( mpd->mPos, rlook, rup );
}

void Camera::calcViewProj()
{
	calcView();
	mpd->mViewProjMatrixT = DirectX::XMMatrixMultiplyTranspose( mpd->mViewMatrix, mpd->mProjMatrix);
}

DirectX::XMMATRIX Camera::getView()
{
	calcView();
	return mpd->mViewMatrix;
}


DirectX::XMMATRIX Camera::getProj() 
{
	return mpd->mProjMatrix;
}

DirectX::XMMATRIX Camera::getViewProj()
{
	calcViewProj();
	return mpd->mViewProjMatrixT;
}

DirectX::XMFLOAT4X4 & Camera::getViewData( DirectX::XMFLOAT4X4 * value )
{
	calcView();
	DirectX::XMStoreFloat4x4( value, DirectX::XMMatrixTranspose(mpd->mViewMatrix) );
	return *value;
}

DirectX::XMFLOAT4X4 & Camera::getProjData( DirectX::XMFLOAT4X4 * value )
{
	
	DirectX::XMStoreFloat4x4( value, DirectX::XMMatrixTranspose(mpd->mProjMatrix) );
	return *value;
}

DirectX::XMFLOAT4X4 Camera::getProjData()
{
	DirectX::XMFLOAT4X4 value;
	DirectX::XMStoreFloat4x4(&value, DirectX::XMMatrixTranspose(mpd->mProjMatrix));
	return value;
}


DirectX::XMFLOAT4X4 & Camera::getViewProjData( DirectX::XMFLOAT4X4 * value )
{
	calcViewProj();
	DirectX::XMStoreFloat4x4( value, mpd->mViewProjMatrixT );
	return *value;
}


void Camera::lookAt( DirectX::XMFLOAT3 pos )
{
	mpd->mLookAt = DirectX::XMLoadFloat3( &pos );
}

void Camera::lookAt( float x, float y, float z )
{
	lookAt( DirectX::XMFLOAT3( x,y,z ) );
}

void Camera::setUpVector( DirectX::XMFLOAT3 up )
{
	mpd->mUp = DirectX::XMLoadFloat3( &up );
}

void Camera::setUpVector( float x, float y, float z ) 
{
	setUpVector( DirectX::XMFLOAT3(x,y,z) );
}

void Camera::setPosition( float x, float y, float z )
{
	setPosition( DirectX::XMFLOAT3( x,y,z ) );
}

void Camera::setPosition( DirectX::XMFLOAT3 pos )
{
	mpd->mPos = DirectX::XMLoadFloat3( &pos );
}

void Camera::setRotationEuler( DirectX::XMFLOAT3 rotEuler )
{
	mpd->mRotQuat = DirectX::XMQuaternionRotationRollPitchYaw( rotEuler.x, rotEuler.y, rotEuler.z );
}

void Camera::setRotationEuler( float pitch, float yaw, float roll )
{
	mpd->mRotQuat = DirectX::XMQuaternionRotationRollPitchYaw( pitch, yaw, roll );
}


void Camera::setRotationQuat( DirectX::XMFLOAT4 rotQuat  )
{
	mpd->mRotQuat = DirectX::XMLoadFloat4( &rotQuat );
}

void Camera::setRotationQuat( float x, float y, float z, float w  )
{
	setRotationQuat( DirectX::XMFLOAT4( x,y,z,w) );
}

void Camera::setPerspective( float width, float height, float vnear, float vfar )
{
	mpd->mProjMatrix = DirectX::XMMatrixPerspectiveLH( width, height, vnear, vfar );
}

void Camera::setPerspectiveFOV( float fovAngleY, float aspectRatio, float vnear, float vfar )
{
	mpd->mProjMatrix = DirectX::XMMatrixPerspectiveFovLH( fovAngleY, aspectRatio, vnear, vfar );
}

void Camera::setOrthographic( float width, float height, float vnear, float vfar )
{
	mpd->mProjMatrix = DirectX::XMMatrixOrthographicLH( width, height, vnear, vfar );
}