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
//  Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_ENTERPRISE_APPSERVER_UDP_PROTOCOL_SERVICE_HPP_INCLUDED
#define ZEN_ENTERPRISE_APPSERVER_UDP_PROTOCOL_SERVICE_HPP_INCLUDED

#include "UDP/Session.hpp"
#include "UDP/MessageBuffer.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Core/Memory/managed_self_ref.hpp>

#include <Zen/Core/Threading/I_Thread.hpp>
#include <Zen/Core/Threading/I_Mutex.hpp>

#include <Zen/Core/Event/I_Event.hpp>

#include <Zen/Enterprise/AppServer/I_ProtocolService.hpp>

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <boost/asio.hpp>

#include <boost/shared_ptr.hpp>

#include <map>
#include <string>
#include <vector>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace AppServer {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class MessageBuffer;

class UserDatagramProtocolService
:   public I_ProtocolService
,   public Zen::Memory::managed_self_ref<Zen::Enterprise::AppServer::I_ProtocolService>
{
    /// @name Types
    /// @{
public:
    typedef std::vector<Threading::I_Thread*>                                               Threads_type;
    typedef Zen::Memory::managed_weak_ptr<Networking::I_Endpoint>                           wpEndpoint_type;

    typedef boost::shared_ptr<UDP::Session>                                                 pSession_type;
    typedef boost::asio::ip::udp::endpoint                                                  EndpointKey_type;
    typedef std::map<EndpointKey_type, pSession_type>                                       SessionMap_type;

    typedef boost::shared_ptr<boost::asio::ip::udp::endpoint>                               pUDPEndpoint_type;
    /// @}

    /// @name I_StartupShutdownParticipant implementation
    /// @{
public:
    virtual void setConfiguration(const Zen::Plugins::I_ConfigurationElement& _config);
    virtual const Plugins::I_ConfigurationElement* getConfiguration() const;
    virtual Zen::Threading::I_Condition* prepareToStart(Zen::Threading::ThreadPool& _threadPool);
    virtual void start();
    virtual Zen::Threading::I_Condition* prepareToStop();
    virtual void stop();
    /// @}

    /// @name I_ProtocolService implementation
    /// @{
public:
    virtual I_ApplicationServer& getApplicationServer();
    virtual pEndpoint_type resolveEndpoint(const std::string& _address, const std::string& _port);
    virtual void sendTo(pMessage_type _pMessage, pEndpoint_type _pEndpoint);
    virtual void disconnect(pEndpoint_type _pEndpoint);
    virtual Event::I_Event& getConnectedEvent();
    virtual Event::I_Event& getDisconnectedEvent();
    /// @}

    /// @name UserDatagramProtocolService implementation
    /// @{
public:
    void startThreads();

    void handleEstablish(const boost::system::error_code& _error, std::size_t _bytesTransferred);

    void createSession();
    void asyncEstablish();
    void destroyEndpoint(wpEndpoint_type _pEndpoint);

    /// This is called by the session after it has been established
    void onEstablishSession(pSession_type _pSession);

    /// This is called by the session after it has been terminated
    void onTerminateSession(pSession_type _pSession);

    void onHandleMessage(pSession_type _pSession, UDP::MessageBuffer& _message);
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class ProtocolServiceFactory;
    explicit UserDatagramProtocolService(I_ApplicationServer& _appServer);
    virtual ~UserDatagramProtocolService();
    /// @}

    /// @name Member Variables
    /// @{
private:
    /// Reference to the parent application server to which this protocol service is bound
    I_ApplicationServer&                                m_appServer;

    /// Configuration element for this protocol
    const Plugins::I_ConfigurationElement*              m_pConfig;

    boost::asio::ip::udp::socket                        m_socket;

    /// IO Service to perform asynchronous operations
    boost::asio::io_service                             m_ioService;
    boost::asio::io_service::strand                     m_strand;
    boost::asio::io_service::work*                      m_pWork;

    /// Address on which to bind
    std::string                                         m_address;

    /// Port on which to bind
    std::string                                         m_port;

    /// Number of threads (defaults to 1)
    int                                                 m_threadCount;

    /// True if this is a server that is listening
    bool                                                m_isServer;

    volatile bool                                       m_threadsStarted;

    /// New session used for the next new UDP session
    /// if this is a server side service.
    /// This is a shared pointer instead of a managed pointer
    /// because boost::bind<> handles shared_ptr correctly
    boost::shared_ptr<UDP::Session>                     m_pNewSession;

    /// Collection of threads
    Threads_type                                        m_threads;

    /// Endpoint to which this service is bound (if any)
    boost::asio::ip::tcp::endpoint                      m_endpoint;

    SessionMap_type                                     m_sessionMap;

    boost::asio::ip::udp::endpoint                      m_remoteEndpoint;
    UDP::MessageBuffer                                  m_readMessage;

    /// Guard for m_sessionMap.
    /// Also used as a guard for m_threads.
    Threading::I_Mutex*                                 m_pSessionsGuard;

    pMessageRegistry_type                               m_pMessageRegistry;
    /// @}

};  // UserDatagramProtocolService class

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace AppServer
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENTERPRISE_APPSERVER_UDP_PROTOCOL_SERVICE_HPP_INCLUDED
