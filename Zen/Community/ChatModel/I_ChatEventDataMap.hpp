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
#ifndef ZEN_COMMUNITY_CHAT_I_CHATEVENT_DATA_MAP_HPP_INCLUDED
#define ZEN_COMMUNITY_CHAT_I_CHATEVENT_DATA_MAP_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Enterprise/Database/I_DatabaseConnection.hpp>
#include <Zen/Enterprise/Database/I_DatabaseService.hpp>
#include <Zen/Enterprise/Database/I_DatabaseTransaction.hpp>

#include <boost/cstdint.hpp>

#include <boost/noncopyable.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
namespace Chat {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_ChatEventDomainObject;
class I_ChatEventDataCollection;

class CHATMODEL_DLL_LINK I_ChatEventDataMap
:	public boost::noncopyable
{

    /// @name Types
    /// @{
public:
//    typedef Zen::Memory::managed_ptr<I_ChatEventDomainObject>   pChatEventDomainObject_type;
//    typedef Zen::Memory::managed_ptr<I_ChatEventDataCollection>     pChatEventDataCollection_type;
    typedef Zen::Memory::managed_ptr<I_ChatEventDataMap>        pChatEventDataMap_type;

    typedef Zen::Database::I_DatabaseService::pDatabaseConnection_type      pDatabaseConnection_type;

    typedef Zen::Memory::managed_ptr<I_ChatEventDataCollection>  pChatEventDataCollection_type;
    typedef Zen::Event::future_return_value<pChatEventDataCollection_type>  FutureChatEventDataCollection_type;
	typedef Zen::Memory::managed_ptr<FutureChatEventDataCollection_type>    pFutureChatEventDataCollection_type;
    
    typedef Zen::Memory::managed_ptr<I_ChatEventDomainObject>  pChatEventDomainObject_type;
    typedef Zen::Event::future_return_value<pChatEventDomainObject_type>  FutureChatEventDomainObject_type;
	typedef Zen::Memory::managed_ptr<FutureChatEventDomainObject_type>    pFutureChatEventDomainObject_type;
    /// @}

    /// @name I_ChatEventDataMap interface
    /// @{
public:
    /// Create a new ChatEventDomainObject.
    virtual pChatEventDomainObject_type createNew() = 0;
    
    /// Get an object by primary key.
    virtual pFutureChatEventDomainObject_type getByKey(boost::uint64_t _primaryKeyValue) = 0;

    /// Delete an object by primary key.
    virtual void deleteByKey(boost::uint64_t _primaryKeyValue) = 0;

    /// Get all of the records.
    virtual pFutureChatEventDataCollection_type getAll() = 0;

    /// Update a ChatEventDomainObject.
    /// If the ChatEventDomainObject hasn't been inserted yet, it will be inserted.
    virtual Zen::Database::I_DatabaseTransaction::pFutureKey_type update(pChatEventDomainObject_type _pChatEventDomainObject) = 0;
    /// @}

    /// @name Static class factory
    /// @{
public:
    static pChatEventDataMap_type create(pDatabaseConnection_type _pDatabaseConnection);
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ChatEventDataMap();
    virtual ~I_ChatEventDataMap();
    /// @}

};  // interface I_ChatEventDataMap

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Zen
}   // namespace Community
}   // namespace Chat
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

namespace Zen {
namespace Memory {
    /// I_ChatEventDataMap is a managed by factory
    template<>
    struct is_managed_by_factory<Zen::Community::Chat::I_ChatEventDataMap> : public boost::true_type{};
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_COMMUNITY_CHAT_I_CHATEVENT_DATA_MAP_HPP_INCLUDED
