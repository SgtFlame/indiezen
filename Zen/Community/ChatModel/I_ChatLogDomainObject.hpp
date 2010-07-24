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
#ifndef ZEN_COMMUNITY_CHAT_I_CHATLOG_DOMAIN_OBJECT_HPP_INCLUDED
#define ZEN_COMMUNITY_CHAT_I_CHATLOG_DOMAIN_OBJECT_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Enterprise/DataModel/I_DataElement.hpp>

#include <boost/noncopyable.hpp>
#include <boost/any.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
namespace Chat {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~


class CHATMODEL_DLL_LINK I_ChatLogDomainObject
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
    /// Get the ChatLogId element.
    /// Use this return value to check to see if the column is NULL in the database,
    /// or you can use it to modify the value instead of calling setChatLogId().
    virtual ElementReference_type getChatLogId() const = 0;
    
    /// Set the ChatLogId element value.
    /// This method does not directly update the database.  
    /// You must use the I_ChatLogDataMap::update() method for the modifications
    /// to be persisted.
    virtual void setChatLogId(const boost::any& _value) = 0;

    /// Get the ChannelId element.
    /// Use this return value to check to see if the column is NULL in the database,
    /// or you can use it to modify the value instead of calling setChannelId().
    virtual ElementReference_type getChannelId() const = 0;
    
    /// Set the ChannelId element value.
    /// This method does not directly update the database.  
    /// You must use the I_ChatLogDataMap::update() method for the modifications
    /// to be persisted.
    virtual void setChannelId(const boost::any& _value) = 0;

    /// Get the AccountId element.
    /// Use this return value to check to see if the column is NULL in the database,
    /// or you can use it to modify the value instead of calling setAccountId().
    virtual ElementReference_type getAccountId() const = 0;
    
    /// Set the AccountId element value.
    /// This method does not directly update the database.  
    /// You must use the I_ChatLogDataMap::update() method for the modifications
    /// to be persisted.
    virtual void setAccountId(const boost::any& _value) = 0;

    /// Get the MessageType element.
    /// Use this return value to check to see if the column is NULL in the database,
    /// or you can use it to modify the value instead of calling setMessageType().
    virtual ElementReference_type getMessageType() const = 0;
    
    /// Set the MessageType element value.
    /// This method does not directly update the database.  
    /// You must use the I_ChatLogDataMap::update() method for the modifications
    /// to be persisted.
    virtual void setMessageType(const boost::any& _value) = 0;

    /// Get the Message element.
    /// Use this return value to check to see if the column is NULL in the database,
    /// or you can use it to modify the value instead of calling setMessage().
    virtual ElementReference_type getMessage() const = 0;
    
    /// Set the Message element value.
    /// This method does not directly update the database.  
    /// You must use the I_ChatLogDataMap::update() method for the modifications
    /// to be persisted.
    virtual void setMessage(const boost::any& _value) = 0;

    /// Get the LogTime element.
    /// Use this return value to check to see if the column is NULL in the database,
    /// or you can use it to modify the value instead of calling setLogTime().
    virtual ElementReference_type getLogTime() const = 0;
    
    /// Set the LogTime element value.
    /// This method does not directly update the database.  
    /// You must use the I_ChatLogDataMap::update() method for the modifications
    /// to be persisted.
    virtual void setLogTime(const boost::any& _value) = 0;
    
    virtual Zen::Enterprise::DataModel::I_DataElement& getElement(const std::string& _fieldName) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ChatLogDomainObject();
    virtual ~I_ChatLogDomainObject();
    /// @}

};  // interface I_ChatLogDomainObject

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Zen
}   // namespace Community
}   // namespace Chat
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

namespace Zen {
namespace Memory {
    /// I_ChatLogDomainObject is a managed by factory
    template<>
    struct is_managed_by_factory<Zen::Community::Chat::I_ChatLogDomainObject> : public boost::true_type{};
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_COMMUNITY_CHAT_I_CHATLOG_DOMAIN_OBJECT_HPP_INCLUDED
