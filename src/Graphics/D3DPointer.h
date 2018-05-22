#pragma once


template <typename D3DType>
class D3DPointer 
{
public:
	D3DPointer(void) : mData(nullptr) { }
	D3DPointer( D3DType * data ) : mData( data )
	{
		if( mData )
			mData->AddRef();
	}
	D3DPointer( const D3DPointer<D3DType> & mPtr ) : mData( mPtr.mData )
	{
		if( mData )
			mData->AddRef();
	}
	D3DPointer( D3DPointer<D3DType> && mPtr ) : mData( mPtr.mData )
	{
		mPtr.mData = nullptr;
	}
			
		
	~D3DPointer(void) {
		if( mData )
			mData->Release();
	}

	D3DType * get() { return mData; }
	
	void set(D3DType * st) 
	{
		if( mData )
		{
			mData->Release();
			mData = nullptr;
		}

		if( st )
		{
			mData = st;
			mData->AddRef();
		}
	}

	operator D3DType * () { return mData; }

	D3DType * operator -> () { return mData; }

	 D3DPointer<D3DType> & operator = (const D3DPointer<D3DType> & mPtr)
	 {
		 if( mData )
			 mData->Release();

		 mData = mPtr.mData;

		 if( mData )
			 mData->AddRef();

		 return *this;
	 }

	 D3DPointer<D3DType> & operator = (D3DPointer<D3DType> && mPtr)
	 {
		 if( mData )
			 mData->Release();

		 mData = mPtr.mData;
		 mPtr.mData = nullptr;

		 if( mData )
			 mData->AddRef();

		 return *this;
	 }



protected:
	D3DType * mData;
};

