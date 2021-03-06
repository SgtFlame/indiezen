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
#ifndef WORLDBUILDER_ZONE_DOMAIN_OBJECT_HPP_INCLUDED
#define WORLDBUILDER_ZONE_DOMAIN_OBJECT_HPP_INCLUDED

#include "../I_ZoneDomainObject.hpp"

#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace WorldBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~


class ZoneDomainObject
:	public I_ZoneDomainObject
{
    /// @name Types
    /// @{
public:
    typedef std::map<std::string, pElement_type>    FieldElementMap_type;
    /// @}

    /// @name Getter / Setter methods
    /// @{
public:
    /// Get the ZoneId element.
    /// Use this return value to check to see if the column is NULL in the database,
    /// or you can use it to modify the value instead of calling setZoneId().
    virtual ElementReference_type getZoneId() const;
    
    /// Set the ZoneId element value.
    /// This method does not directly update the database.  
    /// You must use the I_ZoneDataMap::update() method for the modifications
    /// to be persisted.
    virtual void setZoneId(const boost::any& _value);

    /// Get the ExplorerNodeId element.
    /// Use this return value to check to see if the column is NULL in the database,
    /// or you can use it to modify the value instead of calling setExplorerNodeId().
    virtual ElementReference_type getExplorerNodeId() const;
    
    /// Set the ExplorerNodeId element value.
    /// This method does not directly update the database.  
    /// You must use the I_ZoneDataMap::update() method for the modifications
    /// to be persisted.
    virtual void setExplorerNodeId(const boost::any& _value);

    /// Get the Name element.
    /// Use this return value to check to see if the column is NULL in the database,
    /// or you can use it to modify the value instead of calling setName().
    virtual ElementReference_type getName() const;
    
    /// Set the Name element value.
    /// This method does not directly update the database.  
    /// You must use the I_ZoneDataMap::update() method for the modifications
    /// to be persisted.
    virtual void setName(const boost::any& _value);
    
    virtual Zen::Enterprise::DataModel::I_DataElement& getElement(const std::string& _fieldName);
    /// @}

    /// @name Member Variables
    /// @{
private:
    pElement_type               m_pZoneId;
    pElement_type               m_pExplorerNodeId;
    pElement_type               m_pName;

    FieldElementMap_type            m_elements;
    /// @}

    /// @name 'Structors
    /// @{
public:
             ZoneDomainObject();
    virtual ~ZoneDomainObject();
    /// @}

};  // class ZoneDomainObject

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace WorldBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~


#endif // WORLDBUILDER_ZONE_DOMAIN_OBJECT_HPP_INCLUDED
