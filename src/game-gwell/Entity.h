

#pragma once

#include "Game.h"


typedef unsigned __int32 uint32_t;


struct Instance {
	uint32_t id;
};

template <class _Value>
class SimpleArray {
public:
	typedef _Value		value_type;
	typedef * _Value	value_ptr;

	value_ptr data;

	uint32_t	end;
	uint32_t	size;

#ifdef _DEBUG
	uint32_t nbrGrowthCycles;
#endif 

	void reserve(uint32_t size) {
		if (data)
			delete[] data;

		data = new value_type[size];
	}

	void grow() {
#ifdef _DEBUG
		// Output growth number of times
		++nbrGrowthCycles;
#endif
		if (data) {
			value_ptr dold = data;
			data = new value_type[size + size / 2]; // Grow by 50%
			memcpy(dold, data, sizeof(value_type) * size + size / 2);
			size += size / 2;
		}
	};

	void push_back(value_type & type) {
		if (end >= size) {
			grow();
		}
	}
};



template <class _Type, class _IndexType = uint32_t, class _ListType = std::vector<_Type> >
class Component 
{
    public: 
        typedef _Type           value_type;
        typedef _ListType       list_type;
        typedef _IndexType      index_type;
		
        
        ListType data;
        
		index_type add(value_type & value) {
			data.push_back(value);
			return std::distance(data.begin(), data.end());
		}

		index_type put(index_type location, value_type & type) {
			data.insert(location, type);
		}
		void remove(index_type index) {
			data.remove( )
		}
        
        // Access a single member
        value_type & get( index_type index );
        const value_type & getConst( index_type index ) const;
        
        // Access the whole array
		list_type::value_type * get();
        const list_type::value_type * getConst() const;
        
        
};

template <class _Type, class _ListType = std::vector<_Type>, class _FreeListType = std::vector<uint32_t> >
class ComponentFreeList  
{
    public: 
        typedef _Type           value_type;
        typedef _ListType       list_type;
        typedef _FreeListType   freelist_type;
}


template <class _Type,  class _ComponentType >
class ComponentHandle   {
    public:
    typedef GenericHandle<10,22>          index_type;
    typedef _Type						  value_type;

	_ComponentType						data;
}

template< class Type, class ComponentLink > 
class ComponentLinkFull : protected Component<Type> {
    public:
    typedef ComponentLink::Handle   Handle;
    typedef Type                    DataType;
    typedef ComponentOneToOne<Type, ComponentLink> type;
    
    
    add( Handle h, Type & value ) {
        type::put( h.mIndex, value );
    }
    
    remove( HandleType h ) {
        type::remove( h.mIndex );
    }
    
    datatype & get( Handle h) {
        type::get( h.mIndex );
    }
    
    uint32_t size() {
        type::size();
    }
}


template< class _Value, class _Component >
class ComponentLinkedSparse {
	typedef Component< value_type, uint32_t > Values;
	typedef _Value					value_type;
	typedef _Component::index_type	index_type;
	typedef Values::list_type		list_type;
	
protected:
	std::hash_map< _Component::index_type, Values::index_type > indirection;
	static const uint32_t bad_index = 0xFFFFFFFF;


public:

	Values values;


	void add(index_type item, value_type & value) {
		indirection[item] = type;
	}
	void put(index_type item, value_type & value) {
		add(item, value);
	}
	void remove(index_type item) {
		Values::index_type index = indirection[item];
		values.remove(index);
		indirection.erase(item);
	}

	// Access a single member
	value_type & get(index_type index) {
		return values.get(indirection[index]);
	}

	const value_type & getConst(index_type index) const {
		return values.get(indirection[index]);
	}

	// Access the whole array
	list_type::value_type * get() {
		return values.data();
	}

	const list_type::value_type * getConst() const {
		return values.data();
	}
};


class EntityManager {
    public:
    ComponentHandle< EntityData, EntityHandle > entities;
    ComponentOneToOne< DebugName, EntityHandle >  names;
    ComponentOneToMaybe< Team >       teams;    
    
    EntityHandle newEntity( EntityData & data, const char * n ) {
        EntityHandle h = entities.add( data );
        names.add( h, n );
    }
    
    void removeEntity( EntityHandle h ) {
        entities.remove( h );
        names.remvoe( h );
    }
    
    
}

teams.add( EntityHandle t, teamID ) 

class LinkType = {
	class LinkbyID< IDTYPE, OutputType >;
class LinkOneToOne< InType, OutType >;
class LinkOneToMaybe< InType, OutType >
}

class HandleType = {
	class HandleArray< InValue,OutValue >;
	class HandleGenerations< InValue, OutValue >;
}
class Component< LinkType, ComponentType >
class ComponentManagerID< IDDef, Position, Movable, Collidable, Bullet, Missle, Explosion, FakeOrbitalMotion > Entities;


IDDef id = Entities.new();
Entities.set<Position>(id, &pos);

Entities.processParellel<Movable>(doSomethingWithTheData)
Movable * items = Entities.get<Movable>();
class ComponentMangerHandle;

Component <  DataHandlerFreeList<Position>, LinkById< EntityID, uint32_t > , HandleGenerator> Positions;

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

template < class InputType, class OutputType, int Offset = 0 >
	class LinkByID {
		HashMap< InputType, OutputType >  mapper;
		OutputType get(InputType);
		OutputType add(InputType ItemID, OutputType next);
		OutputType remove(InputType);
		void move(InputType, InputType);
		static size_t getMemSizeForNumberOfItems(int nbr_items);
	};


template <class InType, class OutType>
struct HandleGenerator {
	OutType add(InType value);
	OutType remove(InType value);
};

template <class InType, class OutType = InType>
struct HandleGeneratorNone {
	OutType add(InType value) {
		return value;
	}

	InType get(OutType) {
		return OutType;
	}

	OutType remove(InType value) {
		return value;
	}
};

template <class InType, class OutType, class GenSize>
struct HandleGeneratorGenerations {
	GenSize * generations;
	uint32_t size;
	OutType create(InType value) {
		++generations[value];
		return OutType::create(generations[value], value);
	}
	InType get(OutType) {
		if (generations[OutType::getIndex(OutType)] = OutType::getGenerationData(OutType))
			return OutType::getIndex(OutType);
	}
	OutType remove(InType value) {
		++generations[value];
	}
	OutType find(InType value) {
		return OutType::create(generations[value], value);
	}
};

template <class DataType>
struct DataHandlerFreeList {
	DataType * mBegin;
	uint32_t * mFreeList;
	size_t	   mMaxItems;

	
};

template <class DataType, class Linker, class Handles >
struct Component {
	DataType	* mBegin;
	uint32_t	mEnd
	size_t	 	mMaxNbrItems;
	Linker linker;
	Handles handles;

	Handles::Handle add(Linker::InputType entity, DataType & data) {
		uint32_t posl = linker.add(entity, mEnd);
		if (posl != mEnd) // User is updating an entity component rather than adding a new one
		{
			mBegin[posl] = data;
			return handles.find(posl)
		}
		else {
			mBegin[mEnd] = data;
			return handles.create(mEnd--);
		}
	}

	void remove(Linker::InputType entity) {
		auto pos = linker.remove(entity);

	}
};
Component <  Position, LinkById< EntityID, uint32_t >, HandleGenerator> Positions;