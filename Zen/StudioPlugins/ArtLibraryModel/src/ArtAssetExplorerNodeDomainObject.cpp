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
#include "ArtAssetExplorerNodeDomainObject.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace ArtLibrary {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

ArtAssetExplorerNodeDomainObject::ArtAssetExplorerNodeDomainObject()
{
    m_elements["artAssetExplorerNodeId"] = m_pArtAssetExplorerNodeId = Zen::Enterprise::DataModel::I_DataElement::create();
    m_elements["explorerNodeId"] = m_pExplorerNodeId = Zen::Enterprise::DataModel::I_DataElement::create();
    m_elements["name"] = m_pName = Zen::Enterprise::DataModel::I_DataElement::create();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ArtAssetExplorerNodeDomainObject::~ArtAssetExplorerNodeDomainObject()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ArtAssetExplorerNodeDomainObject::ElementReference_type
ArtAssetExplorerNodeDomainObject::getArtAssetExplorerNodeId() const
{
    return *m_pArtAssetExplorerNodeId.get();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ArtAssetExplorerNodeDomainObject::setArtAssetExplorerNodeId(const boost::any& _value)
{
    m_pArtAssetExplorerNodeId->setAnyValue(_value);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ArtAssetExplorerNodeDomainObject::ElementReference_type
ArtAssetExplorerNodeDomainObject::getExplorerNodeId() const
{
    return *m_pExplorerNodeId.get();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ArtAssetExplorerNodeDomainObject::setExplorerNodeId(const boost::any& _value)
{
    m_pExplorerNodeId->setAnyValue(_value);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ArtAssetExplorerNodeDomainObject::ElementReference_type
ArtAssetExplorerNodeDomainObject::getName() const
{
    return *m_pName.get();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ArtAssetExplorerNodeDomainObject::setName(const boost::any& _value)
{
    m_pName->setAnyValue(_value);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Enterprise::DataModel::I_DataElement&
ArtAssetExplorerNodeDomainObject::getElement(const std::string& _fieldName)
{
    // TODO Guard?

    FieldElementMap_type::iterator iter = m_elements.find(_fieldName);
    if (iter != m_elements.end())
    {
        return *iter->second;
    }
    else
    {
        pElement_type pElement = Zen::Enterprise::DataModel::I_DataElement::create();
        m_elements[_fieldName] = pElement;

        return *pElement;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ArtLibrary
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

