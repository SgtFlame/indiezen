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

#include "Session.hpp"
#include "Endpoint.hpp"

#include "../UserDatagramProtocolService.hpp"

#include <Zen/Core/Threading/MutexFactory.hpp>
#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Enterprise/Networking/I_Endpoint.hpp>

#include <boost/bind.hpp>

#include <iostream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace AppServer {
namespace UDP {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Session::Session(boost::asio::io_service& _ioService, UserDatagramProtocolService& _protocolService, boost::asio::ip::udp::socket& _socket)
:   m_socket(_socket)
,   m_strand(_ioService)
,   m_protocolService(_protocolService)
,   m_pPeerEndpoint()
,   m_isEstablished(false)
,   m_pWriteQueueGuard(Zen::Threading::MutexFactory::create())
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Session::~Session()
{
    if (m_isEstablished)
    {
        terminate();
    }

    {
        Threading::CriticalSection lock(m_pWriteQueueGuard);

        while (!m_writeMessages.empty())
        {
            delete m_writeMessages.front();
            m_writeMessages.pop_front();
        }
    }

    Zen::Threading::MutexFactory::destroy(m_pWriteQueueGuard);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
boost::asio::ip::udp::socket&
Session::getSocketReference()
{
    return m_socket;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Session::establish(pEndpoint_type _pEndpoint)
{
    // Establish session to server.
    m_pPeerEndpoint = _pEndpoint.as<pConcreteEndpoint_type>();

    m_socket.async_connect(m_pPeerEndpoint->getEndpoint(),
        boost::bind(
            &Session::handleEstablishSession,
            shared_from_this(),
            boost::asio::placeholders::error
        )
    );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Session::terminate()
{
    m_isEstablished = false;
    m_socket.close();
    m_protocolService.onTerminateSession(shared_from_this());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Session::handleEstablishSession(const boost::system::error_code& _errorCode)
{
    if (!_errorCode)
    {
        Zen::Threading::CriticalSection lock(m_pWriteQueueGuard);

        m_isEstablished = true;

        m_protocolService.onEstablishSession(shared_from_this());

        // Now check to see if there are any pending write messages
        if (!m_writeMessages.empty())
        {
            writeMore();
        }
    }
    else
    {
        terminate();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Session::start(pEndpoint_type _pEndpoint)
{
    m_isEstablished = true;
    m_pPeerEndpoint = _pEndpoint;

    m_protocolService.onEstablishSession(shared_from_this());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Session::write(const char* _buffer, boost::uint32_t _size)
{
    if (_size > MessageBuffer::MAX_BODY_LENGTH)
    {
        // TODO Error
        return;
    }

    MessageBuffer* pMsg = new MessageBuffer;
    pMsg->setBodyLength(_size);
    memcpy(pMsg->getBody(), _buffer, _size);
    pMsg->encodeHeader();

    Zen::Threading::CriticalSection lock(m_pWriteQueueGuard);

    bool writeInProgress = !m_writeMessages.empty();
    m_writeMessages.push_back(pMsg);

    // Write the message on the front of the queue if a write wasn't 
    // already in progress.
    if (!writeInProgress)
    {
        writeMore();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Session::pEndpoint_type
Session::getPeer() const
{
    return m_pPeerEndpoint;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
UserDatagramProtocolService&
Session::getProtocolService()
{
    return m_protocolService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Session::handleMessageBuffer(MessageBuffer& _message)
{
    m_protocolService.onHandleMessage(shared_from_this(), _message);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Session::handleWrite(const boost::system::error_code& _errorCode)
{
    if (!_errorCode)
    {
        // The write succeeded, so delete the message
        Zen::Threading::CriticalSection lock(m_pWriteQueueGuard);
        delete m_writeMessages.front();
        m_writeMessages.pop_front();

        // Now write some more if necessary
        writeMore();
    }
    else
    {
        // The write failed, so don't delete the message
        std::cout << _errorCode.message() << std::endl;
        terminate();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Session::writeMore()
{
    // If there's more to write then write the next message in line
    if (!m_writeMessages.empty() && m_isEstablished)
    {
        m_socket.async_send_to(
            boost::asio::buffer(m_writeMessages.front()->getData(), m_writeMessages.front()->getLength()),
            m_pPeerEndpoint->getEndpoint(),
            boost::bind(
                &Session::handleWrite, 
                shared_from_this(),
                boost::asio::placeholders::error
            )
        );
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace UDP
}   // namespace AppServer
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
