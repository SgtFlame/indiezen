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
#ifndef ZEN_ENTERPRISE_SESSION_PROTOCOL_I_LOGOUT_REQUEST_HPP_INCLUDED
#define ZEN_ENTERPRISE_SESSION_PROTOCOL_I_LOGOUT_REQUEST_HPP_INCLUDED

#include "../Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <Zen/Enterprise/AppServer/I_Request.hpp>

#include <boost/serialization/string.hpp>
#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace Session {
    class I_Session;
namespace Protocol {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class SESSION_CLIENT_DLL_LINK I_LogoutRequest
:   public virtual Zen::Enterprise::AppServer::I_Request
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<Zen::Enterprise::AppServer::I_Request> pRequest_type;
    /// @}

    /// @name Getter / Setter methods
    /// @{
public:
    /// Get the session element.
    virtual const Enterprise::Session::I_Session& getSession() const = 0;

    /// Set the session element.
    virtual void setSession(const Enterprise::Session::I_Session& _session) = 0;
    /// @}

    /// @name Static methods
    /// @{
public:
    static pRequest_type create(pEndpoint_type _pSourceEndpoint,
                                pEndpoint_type _pDestinationEndpoint,
                                pResourceLocation_type _pSrcLocation,
                                pResourceLocation_type _pDestLocation);

    static pMessageType_type getStaticMessageType();
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_LogoutRequest();
    virtual ~I_LogoutRequest();
    /// @}

};  // interface I_LogoutRequest

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Protocol
}   // namespace Session
}   // namespace Enterprise
namespace Memory {
    // I_LogoutRequest is managed by factory
    template<>
    struct is_managed_by_factory<Zen::Enterprise::Session::Protocol::I_LogoutRequest> 
    :   public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENTERPRISE_SESSION_PROTOCOL_I_LOGOUT_REQUEST_HPP_INCLUDED
