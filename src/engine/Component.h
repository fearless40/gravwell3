
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


template <class DataType, class Allocator, class DataHandler, class InputLinker, class OutputLinker >
class Component {
public:
	typedef DataType Data_Type;
	static const bool isComponentClass = true;


protected:

	DataHandler  data;
	InputLinker  inLink;
	OutputLinker outLink;

	OutputLinker::In_ID add(InputLinker::In_ID entity, Data_Type & data_entry) {
		auto posl = inLinker.add(entity, data.getNextIndex() );
		assert(posl != data.getNextIndex());
		/*if (posl != mEnd) // User is updating an entity component rather than adding a new one
		{
			mBegin[posl] = data;
			return handles.find(posl)
		}*/
		
		data[posl] = data_entry;
		return outLink.add(posl);
	}

	void remove(Linker::InputType entity) {
		auto pos = linker.remove(entity);
		data.remove(handles.remove(pos));
	}
};