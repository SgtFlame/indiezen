//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Enterprise Framework
//
// Copyright (C) 2001 - 2010 Tony Richards
// Copyright (C) 2008 - 2010 Matthew Alan Gray
//
//  This software is provided 'as-is', without any express or implied
//  warranty.  In no event will the authors be held liable for any damages
//  arising from the use of this software.
//
//  Permission is granted to anyone to use this software for any purpose,
//  including commercial applications, and to alter it and redistribute it
//  freely, subject to the following restrictions:
//
//  1. The origin of this software must not be misrepresented; you must not
//     claim that you wrote the original software. If you use this software
//     in a product, an acknowledgment in the product documentation would be
//     appreciated but is not required.
//  2. Altered source versions must be plainly marked as such, and must not be
//     misrepresented as being the original software.
//  3. This notice may not be removed or altered from any source distribution.
//
//  Tony Richards trichards@indiezen.com
//  Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_ENTERPRISE_ACCOUNT_PROTOCOL_AUTHENTICATE_REQUEST_HPP_INCLUDED
#define ZEN_ENTERPRISE_ACCOUNT_PROTOCOL_AUTHENTICATE_REQUEST_HPP_INCLUDED

#include "../../Protocol/I_AuthenticateRequest.hpp"

#include "Request.hpp"

#include <Zen/Enterprise/AppServer/I_MessageFactory.hpp>
#include <Zen/Enterprise/AppServer/I_MessageType.hpp>

#include <boost/serialization/string.hpp>
#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
    namespace AppServer {
        class I_ApplicationServer;
        class I_MessageRegistry;
        class I_MessageType;
    }   // namespace AppServer
namespace Account {
namespace Protocol {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class AuthenticateRequest
:   public I_AuthenticateRequest
,   public Request
{
    /// @name Types
    /// @{
public:
    enum { type = 201 };    /// TODO Should we be hardcoding this?

    typedef Zen::Memory::managed_weak_ptr<Zen::Enterprise::AppServer::I_Request>    wpRequest_type;
    /// @}

    /// @name Zen::Enterprise::AppServer::I_Message implementation
    /// @{
public:
    virtual pEndpoint_type getSourceEndpoint() { return Message::getSourceEndpoint(); }
    virtual pEndpoint_type getDestinationEndpoint() { return Message::getDestinationEndpoint(); }
    virtual pMessageHeader_type getMessageHeader() const { return Message::getMessageHeader(); }
    virtual pResourceLocation_type getSourceLocation() { return Message::getSourceLocation(); }
    virtual pResourceLocation_type getDestinationLocation() { return Message::getDestinationLocation(); }

    virtual void serialize(pMessageHeader_type _pMessageHeader, boost::archive::polymorphic_iarchive& _archive, const int _version);
    virtual void serialize(boost::archive::polymorphic_oarchive& _archive, const int _version);
    /// @}

    /// @name I_Message implementation
    /// @{
public:
    virtual boost::uint64_t getMessageId() const { return Message::getMessageId(); } 
    virtual pMessageType_type getMessageType() const { return getStaticMessageType(); }
    /// @}

    /// @name Getter / Setter methods
    /// @{
public:
    virtual const std::string& getUserId() const;
    virtual void setUserId(const std::string& _userId);
    virtual const std::string& getPassword() const;
    virtual void setPassword(const std::string& _password);
    /// @}

    /// @name Static methods
    /// @{
public:
    static void registerMessage(Zen::Enterprise::AppServer::I_ApplicationServer& _appServer);
    static pMessageHeader_type createMessageHeader(boost::uint64_t _messageId, const std::string& _srcLocation, const std::string& _destLocation);
    static void destroy(wpRequest_type _wpRequest);
    static pMessageType_type getStaticMessageType();    
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class I_AuthenticateRequest;
    friend class MessageFactory;
             /// This constructor is used by the static create
             /// methods for creating outbound messages.
             AuthenticateRequest(pEndpoint_type _pSourceEndpoint,
                                 pEndpoint_type _pDestinationEndpoint,
                                 pResourceLocation_type _pSrcLocation,
                                 pResourceLocation_type _pDestLocation);
             /// This constructor is used by the message factory
             /// for creating inbound messages.
             AuthenticateRequest(pMessageHeader_type _pMessageHeader,
                                 pEndpoint_type _pSourceEndpoint,
                                 pEndpoint_type _pDestinationEndpoint);
    virtual ~AuthenticateRequest();
    /// @}

    /// @name Member Variables
    /// @{
private:
    boost::uint64_t                                         m_id;

    static pMessageType_type                                sm_pType;
    static Zen::Enterprise::AppServer::I_MessageRegistry*   sm_pMessageRegistry;

    std::string                                             m_userId;
    std::string                                             m_password;
    /// @}

};  // class AuthenticateRequest

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Protocol
}   // namespace Account
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENTERPRISE_ACCOUNT_PROTOCOL_AUTHENTICATE_REQUEST_HPP_INCLUDED
