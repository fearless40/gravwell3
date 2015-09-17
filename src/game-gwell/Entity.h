

#pragma once

#include "Game.h"

struct EntityData {
    ;
}

class EntityManager {
    typedef char[32] DebugName;
    
    DECLARE_MANAGER( EntityData, EntityHandle );
    
    std::vector< DebugName > names;
}


template <class Type>
class Component {
    find( );
    Component<Type> clone( );
    add( Type & t );
    put( uint32_t place, Type & value);
    remove( uint32_t index );
    uint32_t size();
    Type & get( uint32_t index );
}



template <class _Type, class _ListType = std::vector<_Type> >
class Component 
{
    public: 
        typedef _Type           value_type;
        typedef _ListType       ListType;
        typedef _IndexType      index_type;
        
        ListType data;
        
        index_type add( value_type & type );
        index_type put( value_type & type );
        void remove( index_type index );
        
        // Access a single member
        DataType & get( index_type index );
        const DataType & getConst( index_type index ) const;
        
        // Access the whole array
        ListType::value_type * get();
        const ListType::value_type * getConst() const;
        
        
}

template <class _type, class _ListType = std::vector<_Type>, class _FreeListType = std::vector<uint32_t> >
class ComponentFreeList : protected _ListType
{
    public: 
        typedef _Type           DataType;
        typedef _ListType       ListType;
        typedef _FreeListType   FreeListType;
}


template <class _Type,  class _ComponentType >
class ComponentHandle  : protected _ComponentType {
    public:
    typedef HandleType          handle_type;
    typedef Component<Type>     type;
    typedef Type                datatype;
}

template< class Type, class ComponentLink > 
class ComponentOneToOne : protected Component<Type> {
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


template< class Type, class ComponentLink >
class ComponentOneToMaybe {
    
}


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