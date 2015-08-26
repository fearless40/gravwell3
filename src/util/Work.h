#pragma once
namespace Util
{
	namespace Work
	{
		struct WorkData
		{
			WorkData( WorkData && data ) = default;
			void * inData;
			void * outData;
		};

		typedef void(*WorkFunction)(WorkData data);

		struct WorkItem
		{
			WorkItem( WorkFunction f, void * inData, void * outData ) : func(f) {
				data.inData = inData;
				data.outData = outData;
			}
			
			WorkItem( WorkItem && item ) = default;
			
			WorkFunction func;
			WorkData	 data;
		};
	};
};