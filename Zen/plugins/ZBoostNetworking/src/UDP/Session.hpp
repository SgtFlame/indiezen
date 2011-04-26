//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Enterprise Framework
//
// Copyright (C) 2001 - 2011 Tony Richards
// Copyright (C) 2008 - 2011 Matthew Alan Gray
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
//  Matthew Alan Gray mgray@hatboystudios.com
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_ENTERPRISE_APPSERVER_UDP_SESSION_HPP_INCLUDED
#define ZEN_ENTERPRISE_APPSERVER_UDP_SESSION_HPP_INCLUDED

#include "MessageBuffer.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <boost/enable_shared_from_this.hpp>

#include <boost/asio.hpp>
#include <boost/array.hpp>

#include <list>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Networking {
        class I_Endpoint;
    }   // namespace Networking
namespace Enterprise {
namespace AppServer {
    class UserDatagramProtocolService;
namespace UDP {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class Endpoint;

class Session
:   public boost::enable_shared_from_this<Session>
,   private boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<Zen::Networking::I_Endpoint>   pEndpoint_type;
    /// @}

    /// @name Session implementation
    /// @{
public:
    /// Get the socket associated with the Session
    boost::asio::ip::udp::socket& getSocketReference();

    /// Establish an endpoint.
    /// This session is not established yet.
    /// This establishes a UDP session to a server.
    void establish(pEndpoint_type _pEndpoint);

    /// Terminate a session.
    void terminate();

    /// The session is "established" so start processing it
    /// but please make it asynchronous :P
    void start(pEndpoint_type _pEndpoint);

    /// Write a message
    void write(const char* _buffer, boost::uint32_t _size);

    /// Get the endpoint to which this Session is established
    pEndpoint_type getPeer() const;

    UserDatagramProtocolService& getProtocolService();

protected:
    void writeMore();

    /// Called after a message buffer is received by the protocol adapter.
    void handleMessageBuffer(MessageBuffer& _message);

    /// Called after a write has completed.
    void handleWrite(const boost::system::error_code& _errorCode);

    /// Called after the session has been established.
    void handleEstablishSession(const boost::system::error_code& _errorCode);
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class UserDatagramProtocolService;
    explicit Session(boost::asio::io_service& _ioService, UserDatagramProtocolService& _protocolService, boost::asio::ip::udp::socket& _socket);
public:
    virtual ~Session();
    /// @}

    /// @name Member Variables
    /// @{
private:
    /// Socket for this session.
    boost::asio::ip::udp::socket&               m_socket;

    /// Used to ensure the Session's handlers
    boost::asio::io_service::strand             m_strand;

    UserDatagramProtocolService&                m_protocolService;

    /// This is the endpoint to which this Session is established
    typedef Zen::Memory::managed_ptr<Endpoint>  pConcreteEndpoint_type;
    pConcreteEndpoint_type                      m_pPeerEndpoint;

    /// True if this Session is established
    bool                                        m_isEstablished;

    MessageBuffer                               m_readMessage;

    typedef std::list<Zen::Enterprise::AppServer::UDP::MessageBuffer*>  MessageQueue_type;
    Threading::I_Mutex*                         m_pWriteQueueGuard;
    MessageQueue_type                           m_writeMessages;
    /// @}

};  // class Session

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace UDP
}   // namespace AppServer
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENTERPRISE_APPSERVER_UDP_SESSION_HPP_INCLUDED
