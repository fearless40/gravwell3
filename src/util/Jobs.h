#include "CriticalSection.h"

namespace Util {
namespace Work {    
    
   /// Work Flow / ownership of memory
   // 1. Call new Job and then set it up. Once done send the Job pointer into Jobs. 
   // 2. Now Jobs owns the job and memory associated with it. The user must not use the pointer as it is now invalid.
   // 3. 

    class Jobs
    {
        private:
        static const int MaxNumberOfWaitingJobs = 100;
        
        typedef uint_32 JobHandle; 
        
    	CriticalSection mCS;
    	
    	// Useing a static array allows me to use less locking. As now I always know who owns a 
    	// job pointer and that the array will never be in validated.
    	Job * mDep[MaxNumberOfWaitingJobs];
    	std::vector<int> mFreeList;
    	ThreadPool & mPool;
    	std::atomic_int SubmitJobsThatDependenciesHaveBeenMetCounter;
    	
        Jobs();
        
        // Call backs
        static void JobSubmitDependentWork( WorkData data );
        
        // Background Thread that cleans mDep and submits Job that all dependencies have been met
        static void SubmitJobsThatDependenciesHaveBeenMet( WorkData * data )
        
        // Signal that a job is complete (handle scheduling backgground thread)
        void JobDependentWorkCompleted( Job * job );
    
    public:
        Jobs( ThreadPool & pool );
    
        // Submit work without any dependencies 
    	void createSubmit(WorkFunction func, void * inData, void * outData);
    
    	// Also will add all the dependents from the job to the waiting array
    	// In debug mode will fail if you attempt to add a Job with its waiting member not = 0 
    	void submit(Job * job);
    	
    	friend class Jobs;
    };
		
}; // Work
}; // Util