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
#ifndef ARTLIBRARY_I_ARTASSETEXPLORERNODE_DOMAIN_OBJECT_HPP_INCLUDED
#define ARTLIBRARY_I_ARTASSETEXPLORERNODE_DOMAIN_OBJECT_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Enterprise/DataModel/I_DataElement.hpp>

#include <boost/noncopyable.hpp>
#include <boost/any.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace ArtLibrary {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~


class ARTLIBRARYMODEL_DLL_LINK I_ArtAssetExplorerNodeDomainObject
:	public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<Zen::Enterprise::DataModel::I_DataElement> pElement_type;
    typedef Zen::Enterprise::DataModel::I_DataElement&                          ElementReference_type;
    /// @}

    /// @name Getter / Setter methods
    /// @{
public:
    /// Get the ArtAssetExplorerNodeId element.
    /// Use this return value to check to see if the column is NULL in the database,
    /// or you can use it to modify the value instead of calling setArtAssetExplorerNodeId().
    virtual ElementReference_type getArtAssetExplorerNodeId() const = 0;
    
    /// Set the ArtAssetExplorerNodeId element value.
    /// This method does not directly update the database.  
    /// You must use the I_ArtAssetExplorerNodeDataMap::update() method for the modifications
    /// to be persisted.
    virtual void setArtAssetExplorerNodeId(const boost::any& _value) = 0;

    /// Get the ExplorerNodeId element.
    /// Use this return value to check to see if the column is NULL in the database,
    /// or you can use it to modify the value instead of calling setExplorerNodeId().
    virtual ElementReference_type getExplorerNodeId() const = 0;
    
    /// Set the ExplorerNodeId element value.
    /// This method does not directly update the database.  
    /// You must use the I_ArtAssetExplorerNodeDataMap::update() method for the modifications
    /// to be persisted.
    virtual void setExplorerNodeId(const boost::any& _value) = 0;

    /// Get the Name element.
    /// Use this return value to check to see if the column is NULL in the database,
    /// or you can use it to modify the value instead of calling setName().
    virtual ElementReference_type getName() const = 0;
    
    /// Set the Name element value.
    /// This method does not directly update the database.  
    /// You must use the I_ArtAssetExplorerNodeDataMap::update() method for the modifications
    /// to be persisted.
    virtual void setName(const boost::any& _value) = 0;
    
    virtual Zen::Enterprise::DataModel::I_DataElement& getElement(const std::string& _fieldName) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ArtAssetExplorerNodeDomainObject();
    virtual ~I_ArtAssetExplorerNodeDomainObject();
    /// @}

};  // interface I_ArtAssetExplorerNodeDomainObject

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ArtLibrary
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

namespace Zen {
namespace Memory {
    /// I_ArtAssetExplorerNodeDomainObject is a managed by factory
    template<>
    struct is_managed_by_factory<ArtLibrary::I_ArtAssetExplorerNodeDomainObject> : public boost::true_type{};
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ARTLIBRARY_I_ARTASSETEXPLORERNODE_DOMAIN_OBJECT_HPP_INCLUDED
