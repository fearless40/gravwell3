
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

namespace ComponentSys {


	template <class Data, class Allocator, class DataStorage, class InternalLinker, class ExtrefGenerator >
	class _Component : public ComponentTraits {
	public:
		typedef Data Data_Type;



	protected:
		Allocator	 alloc;
		DataStorage  data;
		InternalLinker  linker;
		ExtrefGenerator generator;

		typedef ExtrefGenerator gen;
		typedef InternalLinker link;

		
		
		 gen::ID_ext add(const link::ID_ext entity, const Data_Type & data_entry)  {
			auto posl = linker.add(entity, data.getNextIndex());
			assert(posl != data.getNextIndex());
				/*if (posl != mEnd) // User is updating an entity component rather than adding a new one
				{
				mBegin[posl] = data;
				return handles.find(posl)
				}*/
			data[posl] = data_entry;
			return generator.create( posl );
		}

		 gen::ID_ext create(const Data_type & data_entry) {
			 auto posl = data.getNextIndex();
			 data[posl] = data_entry;
			 return generator.create(posl);
		 }

		 void link(gen::ID_ext local, link::ID_ext external) {
			 linker.add(external, generator.get(local));
		 }
		 

		 Data_Type get(const link::ID_ext entity) const {
			 return data[linker.get(entity)];
		 }

		 Data_Type get(const gen::ID_ext id) const {
			 return data[generator.get(id)];
		 }

		 Data_Type * getAll() {
			 return data.getAll();
		 }

		 void set(const link::ID_ext entity, const Data_Type & data) {
			 data[linker.get(entity)] = data;
		 }

		 void set(const gen::ID_ext id, const Data_Type & data) {
			 data[generator.get(id)] = data;
		 }

		void remove(linker::ID_ext entity) {
			auto pos = linker.remove(entity);
			gen.remove(pos);
			data.remove(pos);
		}
	};


	// Partial Specialization for Null Generator not fully fleshed out yet.
	// I might need a partial specialization for null linker too! Not sure yet.
	template <class Data, class Allocator, class DataStorage, class InternalLinker>
	class _Component<Data,Allocator,DataStorage,InternalLinker,GeneratorNull> : public ComponentTraits {
	public:
		typedef Data Data_Type;

	protected:

		
		Allocator	 alloc;
		DataStorage  data;
		InternalLinker  linker;
		ExtrefGenerator genertor;;

		typedef ExtrefGenerator gen;
		typedef InternalLinker link;

		void add(link::ID_ext entity, Data_Type & data_entry) {
			auto posl = linker.add(entity, data.getNextIndex());
			assert(posl != data.getNextIndex());
			/*if (posl != mEnd) // User is updating an entity component rather than adding a new one
			{
			mBegin[posl] = data;
			return handles.find(posl)
			}*/
			data[posl] = data_entry;
			return;
		};



		void remove(Linker::InputType entity) {
			auto pos = linker.remove(entity);
			data.remove(handles.remove(pos));
		}
	};
}