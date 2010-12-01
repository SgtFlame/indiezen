//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Enterprise Framework
//
// Copyright (C) 2001 - 2008 Tony Richards
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
#ifndef ZEN_APPSERVER_I_FULLY_QUALIFIED_RESOURCE_LOCATION_HPP_INCLUDED
#define ZEN_APPSERVER_I_FULLY_QUALIFIED_RESOURCE_LOCATION_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <boost/noncopyable.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Networking {
        class I_Endpoint;
    }   // namespace Networking
namespace Enterprise {
namespace AppServer {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_ResourceLocation;

/// @brief Fully Qualified Resource Location
///
///
class APPSERVER_DLL_LINK I_FullyQualifiedResourceLocation
:   boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<Networking::I_Endpoint>    pEndpoint_type;
    typedef Zen::Memory::managed_ptr<I_ResourceLocation>        pResourceLocation_type;
    /// @}

    /// @name I_FullyQualifiedResourceLocation interface.
    /// @{
public:
    /// Get the endpoint.
    virtual pEndpoint_type getEndpoint() const = 0;

    /// Get the relative resource location.
    virtual pResourceLocation_type getRelativeLocation() const = 0;

    /// Get the location as a URL string.
    virtual const std::string toString() const = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_FullyQualifiedResourceLocation();
    virtual ~I_FullyQualifiedResourceLocation();
    /// @}

};  // interface I_FullyQualifiedResourceLocation

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace AppServer
}   // namespace Enterprise
namespace Memory {
    // I_FullyQualifiedResourceLocation is managed by factory
    template<>
    struct is_managed_by_factory<Zen::Enterprise::AppServer::I_FullyQualifiedResourceLocation> 
    :   public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_APPSERVER_I_FULLY_QUALIFIED_RESOURCE_LOCATION_HPP_INCLUDED
