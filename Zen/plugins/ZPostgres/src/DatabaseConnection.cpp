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

#include "DatabaseConnection.hpp"
#include "DatabaseTransaction.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <boost/bind.hpp>

#include <sstream>
#include <string>
#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZPostgres {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
DatabaseConnection::DatabaseConnection(pDatabaseService_type _pDatabase, const std::string& _name, PGconn* _pConnection)
:   m_pDatabase(_pDatabase)
,   m_name(_name)
,   m_pConnection(_pConnection)
,   m_pTransaction(
        new DatabaseTransaction(m_pConnection),
        DatabaseConnection::onDestroyTransaction
    )
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
DatabaseConnection::~DatabaseConnection()
{
    if (m_pConnection != NULL)
    {
        PQfinish(m_pConnection);
        m_pConnection = NULL;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
DatabaseConnection::getName() const
{
    return m_name;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
DatabaseConnection::pDatabaseTransaction_type
DatabaseConnection::beginTransaction()
{
    if (m_pTransaction->isActive())
    {
        m_pTransaction->completeCondition().requireCondition();
    }

    m_pTransaction->reset();

    return m_pTransaction.as<pDatabaseTransaction_type>();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
DatabaseConnection::commitTransaction(pDatabaseTransaction_type _pDatabaseTransaction)
{
    _pDatabaseTransaction->commit();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
DatabaseConnection::rollbackTransaction(pDatabaseTransaction_type _pDatabaseTransaction)
{
    _pDatabaseTransaction->rollback();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
DatabaseConnection::onDestroyTransaction(wpConcreteDatabaseTransaction_type _wpDatabaseTransaction)
{
    /// Delete the DatabaseTransaction
    DatabaseTransaction* pDatabaseTransaction = 
        dynamic_cast<DatabaseTransaction*>(_wpDatabaseTransaction.get());

    if( pDatabaseTransaction )
    {
        delete pDatabaseTransaction;
    }
    else
    {
        throw Utility::runtime_exception("Zen::Database::DatabaseConnection::onDestroyTransaction() : _wpDatabaseTransaction is not a valid DatabaseTransaction.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
std::string
DatabaseConnection::escapeString(const std::string& _string)
{
    std::string str(_string);

    // Escape the project name
    std::string searchString("\'"); 
    std::string replaceString("\'\'");

    std::string::size_type pos = 0;
    while ((pos = str.find(searchString, pos)) != std::string::npos) 
    {
        str.replace(pos, searchString.size(), replaceString);
        pos += replaceString.size();
    }

    searchString = "\"\"";
    pos = 0;

    while ((pos = str.find(searchString, pos)) != std::string::npos) 
    {
        str.replace(pos, searchString.size(), replaceString);
        pos += replaceString.size();
    }

    return str;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
DatabaseConnection::pDatabaseService_type
DatabaseConnection::getDatabaseService() const
{
    return m_pDatabase;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZPostgres
}   // namespace IndieZen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
