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
#ifndef ZEN_APPSERVER_SCRIPTABLE_GENERIC_SERVICE_HPP_INCLUDED
#define ZEN_APPSERVER_SCRIPTABLE_GENERIC_SERVICE_HPP_INCLUDED

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Core/Memory/managed_self_ref.hpp>

#include <Zen/Core/Threading/I_Mutex.hpp>
#include <Zen/Core/Threading/MutexFactory.hpp>
#include <Zen/Core/Threading/CriticalSection.hpp>
#include <Zen/Core/Threading/ThreadPool.hpp>

#include <Zen/Core/Scripting.hpp>

#include <Zen/Enterprise/AppServer/I_ApplicationService.hpp>
#include <Zen/Enterprise/AppServer/I_ApplicationServer.hpp>
#include <Zen/Enterprise/AppServer/I_ApplicationServerManager.hpp>

#include <Zen/Enterprise/AppServer/I_MessageType.hpp>
#include <Zen/Enterprise/AppServer/I_Message.hpp>
#include <Zen/Enterprise/AppServer/I_MessageHandler.hpp>
#include <Zen/Enterprise/AppServer/I_Request.hpp>
#include <Zen/Enterprise/AppServer/I_RequestHandler.hpp>
#include <Zen/Enterprise/AppServer/I_Response.hpp>
#include <Zen/Enterprise/AppServer/I_ResponseHandler.hpp>
#include <Zen/Enterprise/AppServer/I_TimeoutHandler.hpp>
#include <Zen/Enterprise/AppServer/I_ProtocolService.hpp>

#include <Zen/Enterprise/Networking/I_Endpoint.hpp>

#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>

#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Threading {
        class ThreadPool;
    } // namespace Threading
namespace Enterprise {
namespace AppServer {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace detail {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;
/// base response handler.
/// Used to make the I_ResponseHandler destructor public.
class base_response_handler
: public I_ResponseHandler
{
public:
    virtual ~base_response_handler()
    {
    }

};  // class base_response_handler

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
/// Server side response handler.
/// When the server recieves a request as a message (i.e. not as an
/// internal request, but as a request recieved via a protocol adapter)
/// a server_response_handler is created and the request is dispatched
/// to the appropriate request handler.  When the request handler
/// is finished and has a reply, it invokes I_ResponseHandler::handleReponse()
/// with the reply.  This lightweight handler simply sends the response
/// back to the application server which in turn (filters?) and then sends
/// the message to the client.
class server_response_handler
:   public base_response_handler
{
public:
    virtual void handleResponse(pResponse_type _pResponse)
    {
        // Send the response back to the app server.
        m_appServer.handleMessage(_pResponse);
    }

    server_response_handler(I_ApplicationServer& _appServer)
    :   m_appServer(_appServer) 
    {
    }

    virtual ~server_response_handler()
    {
    }

    static inline void destroy(Memory::managed_weak_ptr<I_ResponseHandler> _pResponseHandler)
    {
        delete dynamic_cast<base_response_handler*>(_pResponseHandler.get());
    }

private:
    I_ApplicationServer&        m_appServer;
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
/// Response handler for the client side.
/// When a client sends a request, it creates this response handler and
/// associates a request, payload and handler function with the request.
/// When the reply comes back from the server, the function is invoked, 
/// passing the original request, the response and the payload.
/// This provides a mechanism by which the client application can
/// have additional data associated with the request.
template<typename Request_type, typename Payload_type>
struct client_response_handler
:   public base_response_handler
{
    typedef typename Request_type::pRequest_type                pRequest_type;
    typedef boost::function<void(pResponse_type, Request_type&, Payload_type)>  Function_type;

    virtual void handleResponse(pResponse_type _pResponse)
    {
        // Use the function to dispatch the handler.
        m_function(_pResponse, *dynamic_cast<Request_type*>(m_pRequest.get()), m_payload);
    }

    client_response_handler(pRequest_type _pRequest, Payload_type _payload, Function_type _function)
    :   m_function(_function)
    ,   m_pRequest(_pRequest)
    ,   m_payload(_payload)
    {
    }

    virtual ~client_response_handler()
    {
    }

public:

    Function_type               m_function;
    pRequest_type               m_pRequest;
    Payload_type                m_payload;
};  // struct client_response_handler

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;
/// base timeout handler.
/// Used to make the I_TimeoutHandler destructor public.
class base_timeout_handler
: public I_TimeoutHandler
{
public:
    virtual ~base_timeout_handler()
    {
    }

};  // class base_timeout_handler

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
/// Timeout handler for the client side.
/// When a client sends a request, it creates this timeout handler and
/// associates a timeout handler with the request.  If the reply does not
/// come back from the server within the allotted timeout time, the function
/// is invoked, passing the original request and payload.  This provides a
/// mechanism by which the client application can have additional data 
/// associated with the request timeout.
template<typename Request_type, typename Payload_type>
struct client_timeout_handler
:   public base_timeout_handler
{
    typedef typename Request_type::pRequest_type                pRequest_type;
    typedef boost::function<void(Request_type&, Payload_type)>  Function_type;

    virtual void handleTimeout()
    {
        // Use the function to dispatch the handler.
        if (!m_function.empty())
        {
            m_function(*dynamic_cast<Request_type*>(m_pRequest.get()), m_payload);
        }
    }

    client_timeout_handler(pRequest_type _pRequest, Payload_type _payload, Function_type _function)
    :   m_function(_function)
    ,   m_pRequest(_pRequest)
    ,   m_payload(_payload)
    {
    }

    virtual ~client_timeout_handler()
    {
    }

public:
    Function_type       m_function;
    pRequest_type       m_pRequest;
    Payload_type        m_payload;
};  // struct client_timeout_handler

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
struct request_handler_base
:   public I_RequestHandler
{
    virtual ~request_handler_base()

    {
    }
};  // struct request_handler_base

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
struct request_handler
:   public request_handler_base
{
    typedef boost::function<void(pRequest_type, pResponseHandler_type)>    Function_type;

    virtual void handleRequest(pRequest_type _pRequest, pResponseHandler_type _pResponseHandler)
    {
        m_function(_pRequest, _pResponseHandler);
    }

    request_handler(Function_type _function)
    :   m_function(_function)
    {
    }

    virtual ~request_handler()
    {
    }

public:

    Function_type               m_function;
};  // struct request_handler

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
struct message_handler_base
:   public I_MessageHandler
{
    virtual ~message_handler_base()
    {
    }
};  // struct message_handler_base

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
struct message_handler
:   public message_handler_base
{
    typedef boost::function<void(pMessage_type)>    Function_type;

    virtual void handleMessage(pMessage_type _pMessage)
    {
        m_function(_pMessage);
    }

    message_handler(Function_type _function)
    :   m_function(_function)
    {
    }

    virtual ~message_handler()
    {
    }

public:

    Function_type               m_function;
};  // struct message_handler

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
} // namespace detail
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

template<typename Request_type, typename Payload_type>
class create_request
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<Zen::Networking::I_Endpoint>                       pEndpoint_type;
    typedef Zen::Memory::managed_ptr<Zen::Enterprise::AppServer::I_ResourceLocation>    pResourceLocation_type;
    typedef Zen::Memory::managed_ptr<Enterprise::AppServer::I_Request>                  pInternalRequest_type;
    /// @}

public:
    inline
    Request_type* operator->()
    {
        return m_pRawRequest;
    }

    /// @name 'Structors
    /// @{
public:
    inline
    create_request(pEndpoint_type _pEndpoint, Payload_type _payload, pResourceLocation_type _pSrcLocation, pResourceLocation_type _pDestLocation)
    :   m_payload(_payload)
    ,   m_pRequest(Request_type::create(pEndpoint_type(), _pEndpoint, _pSrcLocation, _pDestLocation))
    {
        m_pRawRequest = dynamic_cast<Request_type*>(m_pRequest.get());
    }
    /// @}

    /// @name Member Variables
    /// @{
public:
    Payload_type                m_payload;
    Request_type*               m_pRawRequest;
    pInternalRequest_type       m_pRequest;
    /// @}
};

struct ClientHandlers
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<I_ResponseHandler>      pResponseHandler_type;
    typedef Memory::managed_ptr<I_TimeoutHandler>       pTimeoutHandler_type;
    /// @}

    /// @name 'Structors
    /// @{
public:
    ClientHandlers(pResponseHandler_type _pResponseHandler, pTimeoutHandler_type _pTimeoutHandler)
    :   m_pResponseHandler(_pResponseHandler)
    ,   m_pTimeoutHandler(_pTimeoutHandler)
    {
    }
    /// @}

    /// @name Member Variables
    /// @{
public:
    pResponseHandler_type           m_pResponseHandler;
    pTimeoutHandler_type            m_pTimeoutHandler;
    /// @}

};  // struct ClientHandlers

template<typename BaseClass_type, typename Class_type>
class scriptable_generic_service
:   public BaseClass_type
,   public Zen::Scripting::I_ScriptableService
,   public Zen::Memory::managed_self_ref<BaseClass_type>
{
    /// @name Types
    /// @{
public:
    /// Super class
    typedef scriptable_generic_service<
        BaseClass_type,
        Class_type
    > super;

    typedef Class_type*                                             pScriptObject_type;
    typedef Zen::Scripting::ObjectReference<Class_type>             ScriptObjectReference_type;
    typedef ScriptObjectReference_type                              ScriptWrapper_type;
    typedef ScriptWrapper_type*                                     pScriptWrapper_type;

    typedef Zen::Memory::managed_ptr<Zen::Networking::I_Endpoint>                       pEndpoint_type;
    typedef Zen::Memory::managed_ptr<Zen::Enterprise::AppServer::I_ResourceLocation>    pResourceLocation_type;
    typedef I_ApplicationServer::pRequest_type                                          pRequest_type;
    typedef I_ApplicationServer::pResponse_type                                         pResponse_type;
    typedef I_ApplicationServer::pMessage_type                                          pMessage_type;

    typedef Memory::managed_ptr<I_ResponseHandler>                                      pResponseHandler_type;
    typedef Memory::managed_ptr<I_TimeoutHandler>                                       pTimeoutHandler_type;

    typedef std::map<boost::uint64_t, boost::shared_ptr<ClientHandlers> >               ClientHandlersMap_type;

    typedef Zen::Memory::managed_ptr<I_MessageType>                                     pMessageType_type;
    typedef Zen::Memory::managed_ptr<I_RequestHandler>                                  pRequestHandler_type;
    typedef std::map<pMessageType_type, pRequestHandler_type>                           RequestHandlers_type;

    typedef Zen::Memory::managed_ptr<I_MessageHandler>                                  pMessageHandler_type;
    typedef std::map<pMessageType_type, pMessageHandler_type>                           MessageHandlers_type;
    /// @}

    /// @name I_StartupShutdownParticipant implementation
    /// @{
public:
    virtual void setConfiguration(const Zen::Plugins::I_ConfigurationElement& _config);
    virtual Zen::Threading::I_Condition* prepareToStart(Zen::Threading::ThreadPool& _threadPool);
    virtual void start();
    virtual Zen::Threading::I_Condition* prepareToStop();
    virtual void stop();
    /// @}

    /// @name I_RequestHandler implementation
    /// @{
public:
    virtual void handleRequest(pRequest_type _pRequest, pResponseHandler_type _pResponseHandler);
    /// @}

    /// @name I_ApplicationService implementation
    /// @{
public:
    virtual I_ApplicationServer& getApplicationServer();
    virtual const std::string& getServiceName() const;
    virtual pEndpoint_type getServiceEndpoint();
    virtual pResourceLocation_type getServiceLocation();
    virtual void handleMessage(pMessage_type _pMessage);
    /// @}

    /// @name Implementation
    /// @{
public:
    void handleTimeout(boost::uint64_t _requestId);

    template<typename Request_type, typename Payload_type>
    void send(create_request<Request_type, Payload_type>& _request, 
              boost::function<void(pResponse_type, Request_type&, Payload_type)> _responseFunction,
              boost::function<void(Request_type&, Payload_type)> _timeoutFunction);

    void registerRequestHandler(pMessageType_type _pMessageType, boost::function<void(pRequest_type, pResponseHandler_type)> _function);
    void unregisterRequestHandler(pMessageType_type _pMessageType);

    void registerMessageHandler(pMessageType_type _pMessageType, boost::function<void(pMessage_type)> _function);
    void unregisterMessageHandler(pMessageType_type _pMessageType);
    void pushTask(Zen::Threading::ThreadPool::Task* _pTask);

    I_ApplicationServer::pProtocolService_type getProtocol();
    /// @}

    /// @name 'Structors
    /// @{
protected:
             scriptable_generic_service(Zen::Enterprise::AppServer::I_ApplicationServer& _appServer, const std::string& _name);
    virtual ~scriptable_generic_service();
    /// @}

    /// @name Member Variables
    /// @{
private:
    const std::string                                   m_name;

    std::string                                         m_protocol;

    I_ApplicationServer&                                m_appServer;

    pResourceLocation_type                              m_pLocation;

    // Map from getRequestId() to the client handlers.
    ClientHandlersMap_type                              m_clientHandlersMap;

    /// Map from pMessageType_type to the request handler.
    RequestHandlers_type                                m_requestHandlers;

    /// Map from pMessageType_type to the message handler.
    MessageHandlers_type                                m_messageHandlers;

    /// Mutex to guard m_clientHandlersMap.
    Zen::Threading::I_Mutex*                            m_pHandlersMutex;

protected:
    /// Thread Pool from the application server.
    /// Use this if you need a thread instead of spawning one yourself.
    Zen::Threading::ThreadPool*                         m_pThreadPool;
    /// @}
};  // template scriptable_generic_service

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename BaseClass_type, typename Class_type>
inline
scriptable_generic_service<BaseClass_type, Class_type>::scriptable_generic_service(Zen::Enterprise::AppServer::I_ApplicationServer& _appServer, const std::string& _name)
:   m_appServer(_appServer)
,   m_name(_name)
,   m_protocol("default")
,   m_pHandlersMutex(Zen::Threading::MutexFactory::create())
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename BaseClass_type, typename Class_type>
inline
scriptable_generic_service<BaseClass_type, Class_type>::~scriptable_generic_service()
{
    Zen::Threading::MutexFactory::destroy(m_pHandlersMutex);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename BaseClass_type, typename Class_type>
inline
void
scriptable_generic_service<BaseClass_type, Class_type>::setConfiguration(const Zen::Plugins::I_ConfigurationElement& _config)
{
    m_pLocation = Zen::Enterprise::AppServer::I_ApplicationServerManager::getSingleton()
        .createLocation(_config.getAttribute("location"));

    std::string protocol = _config.getAttribute("protocol");
    m_protocol = protocol.empty() ? "default" : protocol;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename BaseClass_type, typename Class_type>
inline
Zen::Threading::I_Condition*
scriptable_generic_service<BaseClass_type, Class_type>::prepareToStart(Zen::Threading::ThreadPool& _threadPool)
{
    m_pThreadPool = &_threadPool;

    // Ready to go, so don't bother returning a condition variable
    return NULL;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename BaseClass_type, typename Class_type>
inline
void
scriptable_generic_service<BaseClass_type, Class_type>::start()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename BaseClass_type, typename Class_type>
inline
Zen::Threading::I_Condition*
scriptable_generic_service<BaseClass_type, Class_type>::prepareToStop()
{
    return NULL;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename BaseClass_type, typename Class_type>
inline
void
scriptable_generic_service<BaseClass_type, Class_type>::stop()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename BaseClass_type, typename Class_type>
I_ApplicationServer&
scriptable_generic_service<BaseClass_type, Class_type>::getApplicationServer()
{
    return m_appServer;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename BaseClass_type, typename Class_type>
const std::string&
scriptable_generic_service<BaseClass_type, Class_type>::getServiceName() const
{
    return m_name;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename BaseClass_type, typename Class_type>
I_ApplicationService::pEndpoint_type
scriptable_generic_service<BaseClass_type, Class_type>::getServiceEndpoint()
{
    if (getProtocol().isValid())
    {
        return getProtocol()->getEndpoint();
    }

    return pEndpoint_type();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename BaseClass_type, typename Class_type>
I_ApplicationService::pResourceLocation_type
scriptable_generic_service<BaseClass_type, Class_type>::getServiceLocation()
{
    return m_pLocation;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename BaseClass_type, typename Class_type>
inline
void
scriptable_generic_service<BaseClass_type, Class_type>::handleMessage(pMessage_type _pMessage)
{
    // Handle an inbound message, request or response.

    // Check to see if it's a response
    Zen::Memory::managed_ptr<I_Response> pResponse(
        _pMessage.as<Zen::Memory::managed_ptr<I_Response> >());

    // If it is a response, handle it.
    if(pResponse.isValid())
    {
        pResponseHandler_type pHandler;

        // Find the handler
        {
            Zen::Threading::CriticalSection guard(m_pHandlersMutex);
            ClientHandlersMap_type::iterator iter = m_clientHandlersMap.find(pResponse->getRequestMessageId());

            if (iter != m_clientHandlersMap.end())
            {
                pHandler = iter->second->m_pResponseHandler;

                m_clientHandlersMap.erase(iter);
            }
        }

        // If the handler was found, use it.
        if (pHandler.isValid())
        {
            // Dispatch the response
            pHandler->handleResponse(pResponse);
        }
        else
        {
            // Probably a duplicate response... just ignore it
            // Or maybe it was a message that timed out?
        }

        return;
    }

    // Check to see if it's a request
    Zen::Memory::managed_ptr<I_Request> pRequest(
        _pMessage.as<Zen::Memory::managed_ptr<I_Request> >());

    if( pRequest.isValid() )
    {
        // TODO If this is a request, create a lightweight response handler then
        // dispatch to scriptable_generic_service::handleRequest.
        // This code path is invoked only when the message is coming from a 
        // protocol adapter.  When the response handler handleResponse() is called,
        // it should simply dispatch to AppServer::handleMessage().  Since
        // the destination endpoint is outbound, the app server will send it
        // via the appropriate protocol adapter.

        RequestHandlers_type::iterator iter = m_requestHandlers.find(pRequest->getMessageType());

        if (iter != m_requestHandlers.end())
        {
            pResponseHandler_type pResponseHandler
            (
                new detail::server_response_handler(this->getApplicationServer()),
                &detail::server_response_handler::destroy
            );
            iter->second->handleRequest(pRequest, pResponseHandler);
        }
        else
        {
            // No request handler.
            // TODO Pass the request to a default request handler.
            throw Utility::runtime_exception("scriptable_generic_service::handleMessage(): Error, no handler specified for message type");
        }

        return;
    }

    // Not a request or response, it must be a message.
    MessageHandlers_type::iterator iter = m_messageHandlers.find(_pMessage->getMessageType());

    if(iter != m_messageHandlers.end())
    {
        iter->second->handleMessage(_pMessage);
        return;
    }

    throw Utility::runtime_exception("scriptable_generic_service::handleMessage(): Error, no handler found or message is not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename BaseClass_type, typename Class_type>
inline
void
scriptable_generic_service<BaseClass_type, Class_type>::handleTimeout(boost::uint64_t _requestId)
{
    pTimeoutHandler_type pHandler;

    // Find the handler
    {
        Zen::Threading::CriticalSection guard(m_pHandlersMutex);
        ClientHandlersMap_type::iterator iter = m_clientHandlersMap.find(_requestId);

        if (iter != m_clientHandlersMap.end())
        {
            pHandler = iter->second->m_pTimeoutHandler;

            m_clientHandlersMap.erase(iter);
        }
    }

    // If the handler was found, use it.
    if (pHandler.isValid())
    {
        // Dispatch the timeout handler
        pHandler->handleTimeout();
    }
    else
    {
        // No timeout handler was specified... just ignore it
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline void destroyTimeoutHandler(Memory::managed_weak_ptr<I_TimeoutHandler> _pHandler)
{
    delete dynamic_cast<detail::base_timeout_handler*>(_pHandler.get());
}


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline void destroyResponseHandler(Memory::managed_weak_ptr<I_ResponseHandler> _pHandler)
{
    delete dynamic_cast<detail::base_response_handler*>(_pHandler.get());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename BaseClass_type, typename Class_type>
class AsyncTimeoutTask
:   public Threading::ThreadPool::Task
{
public:
virtual void call()
{
    boost::asio::io_service io;
    boost::asio::deadline_timer t(io, boost::posix_time::milliseconds(m_milliseconds));
    t.async_wait(boost::bind(&AsyncTimeoutTask::callHandler, this, boost::asio::placeholders::error));
    io.run();
}

void callHandler(const boost::system::error_code&)
{
    m_service.handleTimeout(m_requestId);
}

AsyncTimeoutTask(int _milliseconds, boost::uint64_t _requestId, scriptable_generic_service<BaseClass_type, Class_type>& _service)
:   m_milliseconds(_milliseconds)
,   m_requestId(_requestId)
,   m_service(_service)
{
}

private:
int                                                     m_milliseconds;
boost::uint64_t                                         m_requestId;
scriptable_generic_service<BaseClass_type, Class_type>& m_service;

};  // class AsyncTimeoutTask

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename BaseClass_type, typename Class_type>
template<typename Request_type, typename Payload_type>
inline
void
scriptable_generic_service<BaseClass_type, Class_type>::send(create_request<Request_type, Payload_type>& _request, 
                                                             boost::function<void(pResponse_type, Request_type&, Payload_type)> _responseFunction,
                                                             boost::function<void(Request_type&, Payload_type)> _timeoutFunction)
{
    // Send an outbound request.
    detail::client_response_handler<Request_type, Payload_type>* pRawResponseHandler = 
        new detail::client_response_handler<Request_type, Payload_type>(
            _request.m_pRequest, 
            _request.m_payload, 
            _responseFunction
        );

    Memory::managed_ptr<I_ResponseHandler> pResponseHandler(
        pRawResponseHandler, 
        destroyResponseHandler
    );

    detail::client_timeout_handler<Request_type, Payload_type>* pRawTimeoutHandler =
        new detail::client_timeout_handler<Request_type, Payload_type>(
            _request.m_pRequest,
            _request.m_payload,
            _timeoutFunction
        );

    Memory::managed_ptr<I_TimeoutHandler> pTimeoutHandler(
        pRawTimeoutHandler,
        destroyTimeoutHandler
    );

    {
        Zen::Threading::CriticalSection guard(m_pHandlersMutex);

        m_clientHandlersMap[_request.m_pRawRequest->getMessageId()] = 
            boost::shared_ptr<ClientHandlers>(
                new ClientHandlers(
                    pResponseHandler,
                    pTimeoutHandler
                )
            );
    }

    getApplicationServer().handleRequest(_request.m_pRequest, pResponseHandler);

    AsyncTimeoutTask<BaseClass_type, Class_type>* pTask = new AsyncTimeoutTask<BaseClass_type, Class_type>(
        5000,
        _request.m_pRawRequest->getMessageId(),
        *this
    );

    m_pThreadPool->pushRequest(pTask);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename BaseClass_type, typename Class_type>
inline
void
scriptable_generic_service<BaseClass_type, Class_type>::handleRequest(pRequest_type _pRequest, pResponseHandler_type _pResponseHandler)
{
    // Handle an inbound request.
    RequestHandlers_type::iterator iter = m_requestHandlers.find(_pRequest->getMessageType());
    if( iter != m_requestHandlers.end() )
    {
        iter->second->handleRequest(_pRequest, _pResponseHandler);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline void destroyRequestHandler(Memory::managed_weak_ptr<I_RequestHandler> _pRequestHandler)
{
    delete dynamic_cast<detail::request_handler_base*>(_pRequestHandler.get());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename BaseClass_type, typename Class_type>
inline
void
scriptable_generic_service<BaseClass_type, Class_type>::registerRequestHandler(pMessageType_type _pMessageType, boost::function<void(pRequest_type, pResponseHandler_type)> _function)
{
    RequestHandlers_type::iterator iter = m_requestHandlers.find(_pMessageType);
    if( iter == m_requestHandlers.end() )
    {
        I_RequestHandler* pRaw = new detail::request_handler(_function);

        pRequestHandler_type pRequestHandler(
            pRaw,
            destroyRequestHandler
        );
        m_requestHandlers[_pMessageType] = pRequestHandler;
    }

    /// TODO Exception?
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename BaseClass_type, typename Class_type>
inline
void
scriptable_generic_service<BaseClass_type, Class_type>::unregisterRequestHandler(pMessageType_type _pMessageType)
{
    RequestHandlers_type::iterator iter = m_requestHandlers.find(_pMessageType);
    if( iter != m_requestHandlers.end() )
    {
        m_requestHandlers.erase(iter);
    }

    /// TODO Exception?
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline void destroyMessageHandler(Memory::managed_weak_ptr<I_MessageHandler> _pMessageHandler)
{
    delete dynamic_cast<detail::message_handler_base*>(_pMessageHandler.get());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename BaseClass_type, typename Class_type>
inline
void
scriptable_generic_service<BaseClass_type, Class_type>::registerMessageHandler(pMessageType_type _pMessageType, boost::function<void(pMessage_type)> _function)
{
    MessageHandlers_type::iterator iter = m_messageHandlers.find(_pMessageType);
    if( iter == m_messageHandlers.end() )
    {
        I_MessageHandler* pRaw = new detail::message_handler(_function);

        pMessageHandler_type pMessageHandler(
            pRaw,
            destroyMessageHandler
        );
        m_messageHandlers[_pMessageType] = pMessageHandler;
    }

    /// TODO Exception?
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename BaseClass_type, typename Class_type>
inline
void
scriptable_generic_service<BaseClass_type, Class_type>::unregisterMessageHandler(pMessageType_type _pMessageType)
{
    MessageHandlers_type::iterator iter = m_messageHandlers.find(_pMessageType);
    if( iter != m_messageHandlers.end() )
    {
        m_messageHandlers.erase(iter);
    }

    /// TODO Exception?
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename BaseClass_type, typename Class_type>
inline
void
scriptable_generic_service<BaseClass_type, Class_type>::pushTask(Zen::Threading::ThreadPool::Task* _pTask)
{
    m_pThreadPool->pushRequest(_pTask);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename BaseClass_type, typename Class_type>
inline
I_ApplicationServer::pProtocolService_type
scriptable_generic_service<BaseClass_type, Class_type>::getProtocol()
{
    return m_appServer.getProtocol(m_protocol);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace AppServer
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_APPSERVER_SCRIPTABLE_GENERIC_SERVICE_HPP_INCLUDED
