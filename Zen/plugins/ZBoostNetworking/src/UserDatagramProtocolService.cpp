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
#include <boost/asio.hpp>

#include "UserDatagramProtocolService.hpp"

#include "UDP/Endpoint.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Core/Threading/ThreadFactory.hpp>
#include <Zen/Core/Threading/MutexFactory.hpp>
#include <Zen/Core/Threading/CriticalSection.hpp>

#include <Zen/Core/Plugins/I_ConfigurationElement.hpp>

#include <Zen/Core/Event/I_Event.hpp>
#include <Zen/Core/Event/I_EventService.hpp>

#include <Zen/Enterprise/AppServer/I_Message.hpp>
#include <Zen/Enterprise/AppServer/I_MessageFactory.hpp>
#include <Zen/Enterprise/AppServer/I_MessageType.hpp>
#include <Zen/Enterprise/AppServer/I_MessageRegistry.hpp>
#include <Zen/Enterprise/AppServer/I_MessageHeader.hpp>
#include <Zen/Enterprise/AppServer/I_ApplicationServer.hpp>
#include <Zen/Enterprise/AppServer/I_ApplicationServerManager.hpp>

#include <boost/bind.hpp>

#include <boost/archive/polymorphic_binary_oarchive.hpp>
#include <boost/archive/polymorphic_binary_iarchive.hpp>

#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>

#include <iostream>

#include <stdlib.h>
#include <string.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace AppServer {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
UserDatagramProtocolService::UserDatagramProtocolService(Zen::Enterprise::AppServer::I_ApplicationServer& _appServer)
:   m_appServer(_appServer)
,   m_pConfig(NULL)
,   m_ioService()
,   m_pWork(NULL)
,   m_pSocket(NULL)
,   m_strand(m_ioService)
,   m_address()
,   m_port()
,   m_threadCount(2)
,   m_threadsStarted(false)
,   m_pSessionsGuard(Zen::Threading::MutexFactory::create())
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
UserDatagramProtocolService::~UserDatagramProtocolService()
{
    if (m_pSocket != NULL)
    {
        delete m_pSocket;
    }

    Zen::Threading::MutexFactory::destroy(m_pSessionsGuard);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
UserDatagramProtocolService::setConfiguration(const Zen::Plugins::I_ConfigurationElement& _config)
{
    m_pConfig = &_config;

    m_address = _config.getAttribute("address");
    m_port = _config.getAttribute("port");
    m_threadCount = strtol(_config.getAttribute("threads").c_str(), NULL, 10);

    if (m_address.empty() || m_port.empty())
    {
        m_isServer = false;
    }
    else
    {
        m_isServer = true;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Plugins::I_ConfigurationElement*
UserDatagramProtocolService::getConfiguration() const
{
    return m_pConfig;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
UserDatagramProtocolService::pEndpoint_type
UserDatagramProtocolService::resolveEndpoint(const std::string& _address,
                                             const std::string& _port)
{
    boost::asio::ip::udp::resolver resolver(m_ioService);
    boost::asio::ip::udp::resolver::query query(_address,_port);
    boost::asio::ip::udp::endpoint endpoint = *resolver.resolve(query);

    pEndpoint_type pEndpoint(
        new UDP::Endpoint(getSelfReference(), endpoint),
        boost::bind(&UserDatagramProtocolService::destroyEndpoint, this, _1)
    );

    // Default to true.  Generally an endpoint is outbound when resolveEndpoint()
    // is called.  Since "listen()" is not a valid method (listen ports are 
    // determined by the configuration) then we probably aren't ever creating
    // a non-local endpoint.
    pEndpoint->setIsLocal(false);
    return pEndpoint;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
UserDatagramProtocolService::destroyEndpoint(wpEndpoint_type _pEndpoint)
{
    // Delete the Endpoint
    UDP::Endpoint* pEndpoint = dynamic_cast<UDP::Endpoint*>(_pEndpoint.get());

    if( pEndpoint != NULL )
    {
        delete pEndpoint;
    }
    else
    {
        throw Zen::Utility::runtime_exception("Zen::Enterprise::AppServer::UDP::UserDatagramProtocolService::destroyEndpoint() : _pEndpoint is not a valid UDP::Endpoint.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Threading::I_Condition*
UserDatagramProtocolService::prepareToStart(Zen::Threading::ThreadPool& _threadPool)
{
    if (m_isServer)
    {
        // Resolve the address
        boost::asio::ip::udp::resolver resolver(m_ioService);
        boost::asio::ip::udp::resolver::query query(m_address, m_port);
        boost::asio::ip::udp::endpoint endpoint = *resolver.resolve(query);

        m_pSocket = new boost::asio::ip::udp::socket(m_ioService, endpoint);
    }
    else
    {
        m_pSocket = new boost::asio::ip::udp::socket(m_ioService);
    }

    // Create a new session
    createSession();

    // Ready to go, so don't bother returning a condition variable
    return NULL;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
UserDatagramProtocolService::start()
{
    asyncEstablish();        

    startThreads();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Threading::I_Condition*
UserDatagramProtocolService::prepareToStop()
{
    delete m_pWork;

    Zen::Threading::CriticalSection lock(m_pSessionsGuard);

    for (Threads_type::iterator iter = m_threads.begin(); iter != m_threads.end(); iter++)
    {
        (*iter)->stop();
    }

    m_ioService.stop();

    return NULL;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
UserDatagramProtocolService::stop()
{
    // Join all of the threads
    for (Threads_type::iterator iter = m_threads.begin(); iter != m_threads.end(); iter++)
    {
        (*iter)->join();
        Zen::Threading::ThreadFactory::destroy(*iter);
    }

    // Don't lock until we're done joining, otherwise we may get
    // a deadlock.
    Zen::Threading::CriticalSection lock(m_pSessionsGuard);

    m_threads.clear();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
void
UserDatagramProtocolService::startThreads()
{
    // Check to make sure the threads have not been started
    if (m_threadsStarted)
    {
        return;
    }

    // Not started, but need to double-check while inside of
    // a critical section
    Threading::CriticalSection lock(m_pSessionsGuard);

    // Still not started?
    if (m_threadsStarted)
    {
        return;
    }

    // Start the threads

    class Runnable
    :   public Threading::I_Runnable
    {
    public:
        virtual void run() throw()
        {
            while(!m_stopping)
            {
                try
                {
                    boost::system::error_code ec;
                    m_ioService.run(ec);
                    
                    if( ec )
                    {
                        std::cout << "ASIO Error: " << ec << std::endl;
                    }
                }
                catch(std::exception& _ex)
                {
                    std::cout << "Exception in ASIO loop: " << _ex.what() << std::endl;
                }
                catch(...)
                {
                    std::cout << "Unknown exception in ASIO loop" << std::endl;
                }
            }
        }

        virtual void stop()
        {
            m_stopping = true;
        }

        Runnable(boost::asio::io_service& _ioService) 
            : m_ioService(_ioService), m_stopping(false) {}
        
        boost::asio::io_service& m_ioService;
        volatile bool m_stopping;
    };

    m_pWork = new boost::asio::io_service::work(m_ioService);

    // Reserve the correct amount of space.
    m_threads.reserve(m_threadCount);

    // Start some threads that will execute m_ioService.run()
    for(int x = 0; x < m_threadCount; x++)
    {
        Zen::Threading::I_Thread* pThread = Zen::Threading::ThreadFactory::create(new Runnable(m_ioService));
        m_threads.push_back(pThread);
        pThread->start();
    }

    m_threadsStarted = true;

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
UserDatagramProtocolService::handleEstablish(const boost::system::error_code& _error, std::size_t _bytesTransferred)
{
    if (!_error && _bytesTransferred > 0 && m_readMessage.decodeHeader())
    {
        pSession_type pSession;
        SessionMap_type::iterator iter = m_sessionMap.find(m_remoteEndpoint);
        if (iter == m_sessionMap.end())
        {
            // The new session is now established, so start it.
            pEndpoint_type pEndpoint(
                new UDP::Endpoint(
                    getSelfReference(),
                    m_remoteEndpoint
                ),
                boost::bind(
                    &UserDatagramProtocolService::destroyEndpoint,
                    this,
                    _1
                )
            );

            // This endpoint is not local since it was newly established.
            pEndpoint->setIsLocal(false);

            m_pNewSession->start(pEndpoint);

            m_pNewSession->handleMessageBuffer(m_readMessage);

            // Now, create another session and do an async establish on it.
            createSession();
        }
        else
        {
            iter->second->handleMessageBuffer(m_readMessage);
        }
    }

    // And asynchronously accept the new connection/listen for new messages.
    asyncEstablish();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
UserDatagramProtocolService::createSession()
{
    boost::shared_ptr<UDP::Session> pSession(new UDP::Session(m_ioService, *this, *m_pSocket));
    m_pNewSession.swap(pSession);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
UserDatagramProtocolService::asyncEstablish()
{
    m_pSocket->async_receive_from(
        boost::asio::buffer(m_readMessage.getData(), UDP::MessageBuffer::MAX_LENGTH),
        m_remoteEndpoint,
        boost::bind(
            &UserDatagramProtocolService::handleEstablish,
            this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred
        )
    );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Enterprise::AppServer::I_ApplicationServer&
UserDatagramProtocolService::getApplicationServer()
{
    return m_appServer;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
UserDatagramProtocolService::sendTo(pMessage_type _pMessage,
                                    pEndpoint_type _pEndpoint)
{
    // TODO Push onto a queue and handle in a worker thread?
    {
        typedef Memory::managed_ptr<UDP::Endpoint> pConcreteEndpoint_type;
        pConcreteEndpoint_type pEndpoint(_pEndpoint.as<pConcreteEndpoint_type>());

        pSession_type pSession;

        // Find or create the connection.
        {
            Threading::CriticalSection lock(m_pSessionsGuard);

            // Find the connection associated with this endpoint.
            SessionMap_type::iterator iter = m_sessionMap.find(pEndpoint->getEndpoint());

            if (iter == m_sessionMap.end())
            {
                if (m_isServer)
                {
                    // TODO Error?
                    return;
                }

                pSession = m_pNewSession;

                createSession();

                // Assume this is an outbound endpoint.
                pEndpoint->setIsLocal(false);

                m_sessionMap[pEndpoint->getEndpoint()] = pSession;
                pSession->establish(_pEndpoint);
            }
            else
            {
                pSession = iter->second;
            }
        }

        // TODO Create a task to handle this asynchronously?
        std::stringstream buffer;

        boost::archive::polymorphic_binary_oarchive archive(buffer,
                                                            boost::archive::no_header |
                                                            boost::archive::no_tracking);

        // Serialize the header
        // TODO Handle the version correctly
        _pMessage->getMessageHeader()->serialize(archive, 0);

        // Serialize the rest of the message
        _pMessage->serialize(archive, 0);

        pSession->write(buffer.str().c_str(), (boost::uint32_t)buffer.str().length());

    }

    // Make sure the threads have been started.
    startThreads();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
UserDatagramProtocolService::disconnect(pEndpoint_type _pEndpoint)
{
    typedef Memory::managed_ptr<UDP::Endpoint> pConcreteEndpoint_type;
    pConcreteEndpoint_type pEndpoint(_pEndpoint.as<pConcreteEndpoint_type>());

    if (pEndpoint.isValid())
    {
        Threading::CriticalSection guard(m_pSessionsGuard);

        SessionMap_type::iterator iter = m_sessionMap.find(pEndpoint->getEndpoint());
        if (iter != m_sessionMap.end())
        {
            iter->second->terminate();
        }
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Event::I_Event&
UserDatagramProtocolService::getConnectedEvent()
{
    return getApplicationServer().getEventService()->getEvent("UserDatagramProtocolService/connectedEvent");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Event::I_Event&
UserDatagramProtocolService::getDisconnectedEvent()
{
    return getApplicationServer().getEventService()->getEvent("UserDatagramProtocolService/disconnectedEvent");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
UserDatagramProtocolService::onEstablishSession(pSession_type _pSession)
{
    Zen::Threading::CriticalSection lock(m_pSessionsGuard);

    typedef Zen::Memory::managed_ptr<UDP::Endpoint>     pConcreteEndpoint_type;

    m_sessionMap[_pSession->getPeer().as<pConcreteEndpoint_type>()->getEndpoint()] = _pSession;

    // Dispatch this event.
    getConnectedEvent().fireEvent(_pSession->getPeer());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
UserDatagramProtocolService::onTerminateSession(pSession_type _pSession)
{
    Zen::Threading::CriticalSection lock(m_pSessionsGuard);

    typedef Zen::Memory::managed_ptr<UDP::Endpoint>     pConcreteEndpoint_type;

    m_sessionMap.erase(_pSession->getPeer().as<pConcreteEndpoint_type>()->getEndpoint());

    // Dispatch this event.
    getDisconnectedEvent().fireEvent(_pSession->getPeer());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
UserDatagramProtocolService::onHandleMessage(pSession_type _pSession, UDP::MessageBuffer& _message)
{
    boost::iostreams::stream<boost::iostreams::array> 
        stream(_message.getBody(), _message.getBodyLength());

    boost::archive::polymorphic_binary_iarchive archive(stream, 
                                                        boost::archive::no_header |
                                                        boost::archive::no_tracking);


    
    // Deserialize the header
    I_Message::pMessageHeader_type pHeader = getApplicationServer().getMessageRegistry()->getMessageHeader(archive);

    // Construct the appropriate message
    // The way we're doing it now, we don't know which one of these to call
    // create() or createResponse().
    // If we put that detail in the header somehow, then we can either call
    // the correct method... *or* we can let the create() method figure it out.
    pMessage_type pMessage = pHeader->getMessageType()->getMessageFactory()
        ->create(
            pHeader, 
            _pSession->getPeer(), 
            pEndpoint_type()
        );

    // Deserialize the message
    pMessage->serialize(pHeader, archive, 0);

    // Send the message to the application server
    getApplicationServer().handleMessage(pMessage);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace AppServer
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
