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
#ifndef ZEN_APPSERVER_FULLY_QUALIFIED_RESOURCE_LOCATION_HPP_INCLUDED
#define ZEN_APPSERVER_FULLY_QUALIFIED_RESOURCE_LOCATION_HPP_INCLUDED

#include "../I_FullyQualifiedResourceLocation.hpp"
#include "../I_ApplicationService.hpp"

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace AppServer {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class FullyQualifiedResourceLocation
:   public I_FullyQualifiedResourceLocation
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<I_ApplicationService>  pApplicationService_type;
    /// @}

    /// @name I_FullyQualifiedResourceLocation implementation.
    /// @{
public:
    virtual pEndpoint_type getEndpoint() const;
    virtual pResourceLocation_type getRelativeLocation() const;
    virtual const std::string toString() const;
    /// @}

    /// @name FullyQualifiedResourceLocation implementation
    /// @{
public:
    /// @}

    /// @name Events
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class ApplicationServerManager;
    explicit FullyQualifiedResourceLocation(const std::string& _fromURLString);
             FullyQualifiedResourceLocation(pEndpoint_type _pEndpoint, const std::string& _fromLocationString);
             FullyQualifiedResourceLocation(pEndpoint_type _pEndpoint, pResourceLocation_type _pLocation);
    virtual ~FullyQualifiedResourceLocation();
    /// @}

    /// @name Member Variables
    /// @{
private:
    pEndpoint_type          m_pEndpoint;
    pResourceLocation_type  m_pLocation;
    /// @}

};  // class FullyQualifiedResourceLocation

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace AppServer
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_APPSERVER_FULLY_QUALIFIED_RESOURCE_LOCATION_HPP_INCLUDED
