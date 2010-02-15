//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Spaces
//
// Copyright (C) 2001 - 2009 Tony Richards
//
// Licensed under the Games by Sarge Publishing License - See your licensing
// agreement for terms and conditions.
//
// Do not redistribute this source code.
//
// Tony Richards trichards@gamesbysarge.com
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// This is generated by the Zen Spaces Code Generator.  Do not modify!
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_STUDIO_WORKBENCH_I_EXPLORERNODE_DATA_COLLECTION_HPP_INCLUDED
#define ZEN_STUDIO_WORKBENCH_I_EXPLORERNODE_DATA_COLLECTION_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>


#include <boost/noncopyable.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_ExplorerNodeDomainObject;

class WORKBENCHMODEL_DLL_LINK I_ExplorerNodeDataCollection
:	public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<I_ExplorerNodeDomainObject>  pExplorerNodeDomainObject_type;

    /// Declared below
    struct I_CollectionVisitor;
    /// @}

    /// @name I_ExplorerNodeDataCollection interface.
    /// @{
public:
    /// Push a domain object onto the end of the collection.
    /// This is normally only used by the DomainMap implementation.
    virtual void push_back(pExplorerNodeDomainObject_type _pDomainObject) = 0;
    
    /// Get all of the docmain objects in this collection using a visitor.
    virtual void getAll(I_CollectionVisitor& _visitor) = 0;
    
    /// @return true if this collection is empty.
    virtual bool isEmpty() const = 0;
    /// @}

    /// @name Inner Classes
    /// @{
public:
    struct I_CollectionVisitor
    {
        virtual void begin() = 0;
        virtual void visit(pExplorerNodeDomainObject_type _pDomainObject) = 0;
        virtual void end() = 0;
    };  // interface I_CollectionVisitor

    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ExplorerNodeDataCollection();
    virtual ~I_ExplorerNodeDataCollection();
    /// @}

};  // interface I_ExplorerNodeDataCollection

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Zen
}   // namespace Studio
}   // namespace Workbench
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

namespace Zen {
namespace Memory {
    /// I_ExplorerNodeDataCollection is a managed by factory
    template<>
    struct is_managed_by_factory<Zen::Studio::Workbench::I_ExplorerNodeDataCollection> : public boost::true_type{};
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_STUDIO_WORKBENCH_I_EXPLORERNODE_DATA_COLLECTION_HPP_INCLUDED
