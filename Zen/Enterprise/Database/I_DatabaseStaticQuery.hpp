//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Enterprise Framework
//
// Copyright (C) 2001 - 2008 Tony Richards
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
#ifndef ZEN_DATABASE_I_DATABASE_STATIC_QUERY_HPP_INCLUDED
#define ZEN_DATABASE_I_DATABASE_STATIC_QUERY_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>
#include <Zen/Enterprise/Database/I_DatabaseQuery.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Database {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class DATABASE_DLL_LINK I_DatabaseStaticQuery
:   public virtual I_DatabaseQuery
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<I_DatabaseStaticQuery>         pDatabaseStaticQuery_type;
    typedef Zen::Memory::managed_weak_ptr<I_DatabaseStaticQuery>    wpDatabaseStaticQuery_type;
    typedef Zen::Event::Event<wpDatabaseStaticQuery_type>           databaseStaticQueryEvent_type;
    /// @}

    /// @name I_DatabaseStaticQuery interface
    /// @{
public:
    /// Set the query
    virtual void setQuery(const std::string& _query) = 0;
    /// @}

    /// @name Events
    /// @{
public:
    databaseStaticQueryEvent_type onDestroyEvent;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_DatabaseStaticQuery();
    virtual ~I_DatabaseStaticQuery();
    /// @}

};  // interface I_DatabaseStaticQuery

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Database
namespace Memory {
    // I_DatabaseStaticQuery is managed by I_DatabaseTransaction
    template<>
    struct is_managed_by_factory<Database::I_DatabaseStaticQuery> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_DATABASE_I_DATABASE_STATIC_QUERY_HPP_INCLUDED
