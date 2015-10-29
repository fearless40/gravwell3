
#pragma once
/*
Component <  DataHandlerFreeList<Position>, LinkById< EntityID, uint32_t >, HandleGenerator> Positions;

Positions.add(Entity, pos(x, y, z));
pos = Positions.get(Entity);
Positions * posArray = Positions.getAll();
Positions.gc();
Positions.remove(Entity);
View = Positions.view(FilterFunction).duplicate();
View = Positions.view(FilterFunction).readonly();
Positions[x] = pos;
Positions.set(Enitity, pos);
Positions.edit(Entity, posNewValue);
Positions.handleGet(Entity);
Positions.handleEdit(Handle value, pos);
*/
#include <../boost/mpl/if.hpp>
#include "ComponentSystem.h"
#include "GeneratorNull.h"
#include "LinkNull.h"
#include <boost\static_assert.hpp>

namespace ComponentSys {


	/// Class Understanding
	//	Terminology:
	//		Handle: local (produced from the component) method of finding a member in the array
	//		ID:	external (not produced from the component) method of associating another data item with this data item.
	//		Data: single data member in the storage system.
	//		DataStorage: All the data
	
	// 3 Versions of the Class
	// Partial template specalization 
	//		Kind of a pain to maintain however only easy way that I could think of due to how the functions change
	//		and how the interface changes. The reason I want the interface to change so that errors of accessing the 
	//		class are thrown by the compiler and not by the runtime. 
	//		I did think of leavin the interface the same and just throwing assert errors however I feel that would make it
	//		harder to work with. 
	namespace Hidden {
	template <class Data, class Allocator, class DataStorage, class InternalLinker, class ExtrefGenerator >
	class Component : public ComponentTraits {
	public:
		typedef Data Data_Type;



	protected:
		Allocator	 alloc;
		DataStorage  data;
		InternalLinker  linker;
		ExtrefGenerator generator;

		typedef ExtrefGenerator gen;
		typedef InternalLinker link;

	public:
		Component() { }
		~Component(){ }
	
	
		// Associates an ID with a new data entry and sets the value		
		// For speed does not return the item associated 
		void add(const link::ID_ext entity, const Data_Type & data_entry)  {
			linker.link(entity, add(data_entry));
		}

		// Creates a new value with no external link. Returns handle which allows
		// it to be accessd later. Handles can be invalidated!
		gen::ID_ext create(const Data_type & data_entry) {
			 return generator.create(data.add(data_entry));
		 }

		// Associates handle with id
		 void link(gen::ID_ext local, link::ID_ext external) {
			 linker.add(external, generator.get(local));
		 }
		 
		// Gets copy of Data associated with ID
		 Data_Type get(const link::ID_ext entity) const {
			 return data[linker.get(entity)];
		 }

		// Gets copy of Data associated with handle
		 Data_Type get(const gen::ID_ext id) const {
			 return data[generator.get(id)];
		 }

		// Gets the entire set of data as a contigous array for stream processing
		 Data_Type * getAll() {
			 return data.getAll();
		 }

		// Change a data by looking up by ID. Will not create a new Data.
		 void set(const link::ID_ext entity, const Data_Type & data) {
			 data[linker.get(entity)] = data;
		 }

		// Change a data by looking up handle. Will not create a new Data.
		 void set(const gen::ID_ext id, const Data_Type & data) {
			 data[generator.get(id)] = data;
		 }

		// Remove data by using ID
		void remove(linker::ID_ext entity) {
			auto pos = linker.unlink(entity);
			gen.remove(pos);
			data.remove(pos);
		}
		
		
		// Remove data by using handle.
		void remove(gen::ID_ext id){
			auto pos = generator.remove(id);
			linker.unlinkByInternal(pos);
			data.remove(pos);
		}
	};

// Specialization For class with NO external Link look up
template <class Data, class Allocator, class DataStorage,  class ExtrefGenerator >
	class Component<class Data, class Allocator, class DataStorage, LinkNull, class ExtrefGenerator > : public ComponentTraits {
	public:
		typedef Data Data_Type;



	protected:
		Allocator	 alloc;
		DataStorage  data;
		ExtrefGenerator generator;

		typedef ExtrefGenerator gen;
		typedef InternalLinker link;

	public:
		Component() { }
		~Component(){ }
	
	
		// Creates a new value with no external associations. Returns handle
		 gen::ID_ext create(const Data_type & data_entry) {
			 auto posl = data.getNextIndex();
			 data[posl] = data_entry;
			 return generator.create(posl);
		 }


		// Gets copy of Data associated with handle
		 Data_Type get(const gen::ID_ext id) const {
			 return data[generator.get(id)];
		 }

		// Gets the entire set of data as a contigous array for stream processing
		 Data_Type * getAll() {
			 return data.getAll();
		 }

		// Change a data by looking up handle. Will not create a new Data.
		 void set(const gen::ID_ext id, const Data_Type & data) {
			 data[generator.get(id)] = data;
		 }

		// Remove data by using handle.
		void remove(gen::ID_ext id){
			auto pos = generator.remove(id);
			data.remove(pos);
		}
	};
	
// Specialiation for class with NO internal linkadge 
	template <class Data, class Allocator, class DataStorage, class InternalLinker >
	class Component<class Data, class Allocator, class DataStorage, class InternalLinker, GeneratorNull > : public ComponentTraits {
	public:
		typedef Data Data_Type;



	protected:
		Allocator	 alloc;
		DataStorage  data;
		InternalLinker  linker;

		typedef ExtrefGenerator gen;
		typedef InternalLinker link;

	public:
		Component() { }
		~Component(){ }
	
	
		// Associates an ID with a new data entry and sets the value		
		 Data_Type & add(const link::ID_ext entity, const Data_Type & data_entry)  {
			auto internalIndex = linker.link(entity, data.getNextIndex());
			data[internalIndex] = data_entry;
			return data[internalIndex];
		}

		
		// Associates handle with id
		 void link(gen::ID_ext local, link::ID_ext external) {
			 linker.add(external, generator.get(local));
		 }
		 
		// Gets copy of Data associated with ID
		 Data_Type get(const link::ID_ext entity) const {
			 return data[linker.get(entity)];
		 }

		
		// Gets the entire set of data as a contigous array for stream processing
		 Data_Type * getAll() {
			 return data.getAll();
		 }

		// Change a data by looking up by ID. Will not create a new Data.
		 void set(const link::ID_ext entity, const Data_Type & data) {
			 data[linker.get(entity)] = data;
		 }

		
		// Remove data by using ID
		void remove(linker::ID_ext entity) {
			auto pos = linker.unlink(entity);
			data.remove(pos);
		}
		
		
		};
	
// Specialization For no look up at all. Very bad class. I may make this class throw an error. However to make the class still useable
// I have moved data into the public access for the class. 
	template <class Data, class Allocator, class DataStorage >
	class Component<class Data, class Allocator, class DataStorage, LinkNull, GeneratorNull > : public ComponentTraits {
		BOOST_STATIC_ASSERT_MSG(false, Need_To_specify_some_form_of_external_linkadge);
		/*public:
		typedef Data Data_Type;

		DataStorage  data;

	protected:
		Allocator	 alloc;


		typedef ExtrefGenerator gen;
		typedef InternalLinker link;

	public:
		Component() { }
		~Component(){ }
	

		// Gets the entire set of data as a contigous array for stream processing
		 Data_Type * getAll() {
			 return data.getAll();
		 }
	*/
	};
	};

	// Helper constructor to allow using void instead of having to use LinkNull or GeneratorNull
	
	template <class Data, class Allocator, class DataStorage, class InternalLinker, class ExtrefGenerator >
	struct Component : Hidden::Component<class Data, class Allocator, class DataStorage, class InternalLinker, class ExtrefGenerator > { };
	
	template <class Data, class Allocator, class DataStorage, class InternalLinker >
	struct Component<class Data, class Allocator, class DataStorage, void , void > : Hidden::Component<class Data, class Allocator, class DataStorage, InternalLinker, class GeneratorNull > { };
	
	template <class Data, class Allocator, class DataStorage,  class ExtrefGenerator >
	struct Component<class Data, class Allocator, class DataStorage, void , class ExtrefGenerator > : Hidden::Component<class Data, class Allocator, class DataStorage, LinkNull, class ExtrefGenerator > { };
	
	template <class Data, class Allocator, class DataStorage >
	struct Component<class Data, class Allocator, class DataStorage, void , void > : Hidden::Component<class Data, class Allocator, class DataStorage, LinkNull, GeneratorNull > { };
	
}