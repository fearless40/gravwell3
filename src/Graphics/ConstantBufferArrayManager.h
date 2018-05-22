#pragma once

#include "Handles.h"

class D3D11;


class ConstantBufferArrayManager
{
private:
	static void rebuildCache();
public:
		
	static void initalize(uint32_t nbrBufferArrays = 1024, uint32_t EstimatedNbrBuffersPerArray = 2);
	static void shutdown();


		static ConstantBufferArrayHandle newBufferArray(uint32_t nbrBuffers, uint32_t startBindPos = 0);
		static void	 deleteHandle( ConstantBufferArrayHandle buffer );
		static bool  isValidHandle( ConstantBufferArrayHandle buffer );
		static void  deleteAll();

		static void  editBuffer( D3D11 & d3d, ConstantBufferArrayHandle buffer, uint32_t index, void * data, uint32_t memSize );

		static void  addDynamicBuffer( D3D11 & d3d, ConstantBufferArrayHandle buffer, uint32_t index, void * data, uint32_t memSize );
		//static void  addPartialBuffer( D3D11 & d3d, ConstantBufferArrayHandle buffer, uint32_t index, void * data, uint32_t memSize );

		static void		setStartBindingLocation( ConstantBufferArrayHandle buffer, uint16_t bindLocation );
		static uint32_t getStartBindingLocation( ConstantBufferArrayHandle buffer );

		static uint32_t	getNbrBuffers( ConstantBufferArrayHandle buffer );

		static ID3D11Buffer ** getBindableBuffers( ConstantBufferArrayHandle buffer );

		
		

};



class ConstantBufferArray
{
private:
	ConstantBufferArrayHandle me;
	ConstantBufferArray();

public:
	

	ConstantBufferArray( ConstantBufferArrayHandle buffer ) : me( buffer ) { }
	ConstantBufferArray( int nbrBuffers ) {
		me = ConstantBufferArrayManager::newBufferArray( nbrBuffers );
	}


	uint32_t nbrBuffers() { return ConstantBufferArrayManager::getNbrBuffers( me ); }
	void editBuffer( D3D11 & d3d, uint32_t index, void * data, uint32_t memSize ) { ConstantBufferArrayManager::editBuffer( d3d, me, index, data, memSize ); }
	void addDynamicBuffer( D3D11 & d3d, uint32_t index, void * data, uint32_t memSize ) { ConstantBufferArrayManager::addDynamicBuffer( d3d, me, index, data, memSize ); }

	void deleteMe() { ConstantBufferArrayManager::deleteHandle( me ); me.mIndex = 0; }
	bool isValid()  { return ConstantBufferArrayManager::isValidHandle(me); }

	void setStartBindLocation( uint16_t location ) { ConstantBufferArrayManager::setStartBindingLocation( me, location ); }
	uint32_t getStartBindLocation( ) { return ConstantBufferArrayManager::getStartBindingLocation( me ); }

	ConstantBufferArrayHandle getHandle() { return me; }

	operator ID3D11Buffer ** () { return ConstantBufferArrayManager::getBindableBuffers( me );  }

	operator  ConstantBufferArrayHandle() { return me; }
};

