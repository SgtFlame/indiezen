//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Studio
//
// Copyright (C) 2001 - 2009 Tony Richards
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
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef WORKBENCH_I_CONTRIBUTOR_SERVICE_HPP_INCLUDED
#define WORKBENCH_I_CONTRIBUTOR_SERVICE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <Zen/Core/Event/Event.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_ContributorServiceFactory;
class I_Workbench;

/// Contributor Service.
/// This extension point is responsible for registering workbench
/// contributors.  Upon construction, the extension implementation
/// should register all of the contributors for which it is responseible
/// with the appropriaten resource types.
/// @see I_Contributor
class WORKBENCH_DLL_LINK I_ContributorService
{
    /// @name Types
    /// @{
public:
    typedef std::string                                     index_type;
    typedef I_ContributorServiceFactory                     factory_type;

    typedef Memory::managed_ptr<I_ContributorServiceFactory>
                                                            pService_type;
    typedef Memory::managed_weak_ptr<I_ContributorServiceFactory>
                                                            wpService_type;

    typedef Event::Event<wpService_type>                    serviceEvent_type;
    /// @}

    /// @name I_ContributorService interface
    /// @{
public:
    /// Enable the actions for this extension.
    virtual void enableActions() = 0;
    /// @}

    /// @name Static methods
    /// @{
public:
    static const std::string& getNamespace();
    static const std::string& getExtensionPointName();
    /// @}

    /// @name Events
    /// @{
public:
    serviceEvent_type   onDestroyEvent;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ContributorService();
    virtual ~I_ContributorService();
    /// @}

};  // interface I_ContributorService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
namespace Memory
{
    /// I_ContributorService is managed by a factory
    template<>
    struct is_managed_by_factory<Zen::Studio::Workbench::I_ContributorService> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // WORKBENCH_I_CONTRIBUTOR_SERVICE_HPP_INCLUDED
