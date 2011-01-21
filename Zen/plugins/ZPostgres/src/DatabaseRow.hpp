//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2011 Tony Richards
// Copyright (C)        2011 Matthew Alan Gray
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
#ifndef ZEN_ZPOSTGRES_DATABASE_ROW_HPP_INCLUDED
#define ZEN_ZPOSTGRES_DATABASE_ROW_HPP_INCLUDED

#include "libpq-fe.h"

#include "PostgresTypes.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Enterprise/Database/I_DatabaseRow.hpp>
#include <Zen/Enterprise/Database/I_DatabaseColumn.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZPostgres {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class DatabaseRow
:   public Zen::Database::I_DatabaseRow
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_DatabaseRow implementation
    /// @{
public:
    virtual void getFields(I_FieldVisitor& _visitor) const;
    /// @}

    /// @name DatabaseRow implementation
    /// @{
public:
    /// @}

    /// @name Event handlers
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
public:
             DatabaseRow(int _row, PGresult* _pResult);
    virtual ~DatabaseRow();
    /// @}

    /// @name Member variables
    /// @{
private:
    int             m_row;
    PGresult*       m_pResult;
    /// @}

};  // class DatabaseRow

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZPostgres
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZPOSTGRES_DATABASE_ROW_HPP_INCLUDED
