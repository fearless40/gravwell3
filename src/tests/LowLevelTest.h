
class D3D11;

namespace Test {
		

	class LowLevelTest {

		struct InternalData;
		
	private:
		InternalData *	mData;
		D3D11 &			md3d;
		bool			mIsOk;
		void clearData();

	public:
		LowLevelTest( D3D11 & d3d );
		~LowLevelTest();
		



		bool isOk(); 
		
		bool initalize(float width, float height);

		bool frame();

	};
};