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

#include "DatabaseTransaction.hpp"
#include "DatabaseStaticQuery.hpp"
#include "DatabaseDynamicQuery.hpp"
#include "DatabaseCommand.hpp"
#include "DatabaseResult.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <boost/bind.hpp>
#include <boost/functional/hash.hpp>
#include <boost/lexical_cast.hpp>
#include <sstream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZPostgres {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
DatabaseTransaction::DatabaseTransaction(PGconn* _pConnection)
:   m_pConnection(_pConnection)
,   m_isCommitted(false)
,   m_isActive(true)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
DatabaseTransaction::~DatabaseTransaction()
{
    if (m_isActive)
    {
        rollback();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
DatabaseTransaction::pStaticQuery_type
DatabaseTransaction::createStaticQuery()
{
    DatabaseStaticQuery* pRawStaticQuery = 
        new DatabaseStaticQuery();

    pStaticQuery_type pStaticQuery(
        pRawStaticQuery,
        boost::bind(&DatabaseTransaction::onDestroyStaticQuery, this, _1)
    );

    PGresult* pResult = PQexec(m_pConnection, "BEGIN");
    if (PQresultStatus(pResult) != PGRES_COMMAND_OK)
    {
        PQclear(pResult);
        std::stringstream message;
        message << "DatabaseTransaction::createStaticQuery() : Error beginning transaction -- "
            << std::string(PQerrorMessage(m_pConnection));
        throw Utility::runtime_exception(message.str().c_str());
    }

    m_isCommitted = false;
    m_isActive = true;

    return pStaticQuery;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
DatabaseTransaction::pDynamicQuery_type
DatabaseTransaction::createDynamicQuery()
{
    DatabaseDynamicQuery* pRawDynamicQuery = new DatabaseDynamicQuery();

    pDynamicQuery_type pDynamicQuery(
        pRawDynamicQuery,
        boost::bind(&DatabaseTransaction::onDestroyDynamicQuery, this, _1)
    );

    return pDynamicQuery;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
DatabaseTransaction::executeQuery(pQuery_type _pQuery,
                                  pQueryResultHandler_type _pQueryResultHandler)
{
    // Cast the query to a DatabaseQuery object
    DatabaseQuery* pRawQuery = dynamic_cast<DatabaseQuery*>(_pQuery.get());

    if( pRawQuery )
    {
        const char* pSQL = pRawQuery->getQuery().c_str();

        //while(*pSQL != '\0')
        //{
            //std::string statementName = generateStatementName(pRawQuery);
            PGresult* pResult = PQprepare(
                m_pConnection,
                //statementName.c_str(),
                "",
                pSQL,
                0,
                NULL
            );

            if (PQresultStatus(pResult) == PGRES_COMMAND_OK)
            {
                //DatabaseResult* pRawDatabaseResult =
                //    new DatabaseResult(m_pConnection, statementName);
                DatabaseResult* pRawDatabaseResult =
                    new DatabaseResult(m_pConnection, "");

                pDatabaseResult_type pDatabaseResult(
                    pRawDatabaseResult,
                    boost::bind(&DatabaseTransaction::onDestroyDatabaseResult, this, _1)
                );

                /// TODO Handle this async -- only finishing here to
                /// make sure there is something to handle.
                pRawDatabaseResult->finish();

                _pQueryResultHandler->handleResult(pDatabaseResult);
            }
            else
            {
                std::stringstream message;
                message << "DatabaseTransaction::executeQuery() : Postgres result retrieval error -- "
                        << std::string(PQerrorMessage(m_pConnection));
                throw Utility::runtime_exception(message.str().c_str());
            }
        //}
    }
    else
    {
        throw Utility::runtime_exception("DatabaseTransaction::executeQuery() : _pQuery is not a valid DatabaseQuery.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
DatabaseTransaction::executeQuery(pQuery_type _pQuery)
{
    // Cast the query to a DatabaseQuery object
    DatabaseQuery* pRawQuery = dynamic_cast<DatabaseQuery*>(_pQuery.get());

    if( pRawQuery )
    {
        const char* pSQL = pRawQuery->getQuery().c_str();

        //while(*pSQL != '\0')
        //{
            //std::string statementName = generateStatementName(pRawQuery);
            PGresult* pResult = PQprepare(
                m_pConnection,
                //statementName.c_str(),
                "",
                pSQL,
                0,
                NULL
            );

            if (PQresultStatus(pResult) == PGRES_COMMAND_OK)
            {
                //DatabaseResult* pRawDatabaseResult =
                //    new DatabaseResult(m_pConnection, statementName);
                DatabaseResult* pRawDatabaseResult =
                    new DatabaseResult(m_pConnection, "");

                pDatabaseResult_type pDatabaseResult(
                    pRawDatabaseResult,
                    boost::bind(&DatabaseTransaction::onDestroyDatabaseResult, this, _1)
                );

                pRawDatabaseResult->finish();
            }
            else
            {
                std::stringstream message;
                message << "DatabaseTransaction::executeQuery() : Postgres result retrieval error -- "
                        << std::string(PQerrorMessage(m_pConnection));
                throw Utility::runtime_exception(message.str().c_str());
            }
        //}
    }
    else
    {
        throw Utility::runtime_exception("DatabaseTransaction::executeQuery() : _pQuery is not a valid DatabaseQuery.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
DatabaseTransaction::pFutureKey_type
DatabaseTransaction::executeInsert(pQuery_type _pQuery, const std::string& _keyName)
{
    class InsertResultHandler
    :   public I_QueryResultHandler
    {
        /// @name Types
        /// @{
    public:
        /// @}

        /// @name Inner Structures
        /// @{
    public:
        class InsertResultRowVisitor
        :   public Zen::Database::I_DatabaseResult::I_ResultVisitor
        {
        public:
            class InsertResultFieldVisitor
            :   public Zen::Database::I_DatabaseRow::I_FieldVisitor
            {
            public:
                virtual void begin()
                {
                }

                virtual void visit(const Zen::Database::I_DatabaseColumn& _column, const boost::any& _value)
                {
                    m_pKey->setValue(
                        boost::lexical_cast<boost::uint64_t, std::string>(
                            boost::any_cast<std::string>(_value)
                        )
                    );
                }

                virtual void end()
                {
                }

                InsertResultFieldVisitor(pFutureKey_type& _pKey)
                :   m_pKey(_pKey)
                {
                }

            private:
                pFutureKey_type&    m_pKey;
            };  // class InsertResultFieldVisitor

            virtual void begin()
            {
            }

            virtual void visit(const Zen::Database::I_DatabaseRow& _row)
            {
                InsertResultFieldVisitor visitor(m_pKey);
                _row.getFields(visitor);
            }

            virtual void end()
            {
            }

            InsertResultRowVisitor(pFutureKey_type& _pKey)
            :   m_pKey(_pKey)
            {
            }

        private:
            pFutureKey_type&    m_pKey;

        };  // class InsertResultRowVisitor
        /// @}

        /// @name I_QueryResultHandler implementation
        /// @{
    public:
        virtual void handleResult(pDatabaseResult_type _pResult)
        {
            InsertResultRowVisitor visitor(m_pKey);
            _pResult->getResults(visitor);
        }
        /// @}

        /// @name 'Structors
        /// @{
    public:
        InsertResultHandler(pFutureKey_type& _pKey)
        :   m_pKey(_pKey)
        {
        }
        /// @}

        /// @name Member Variables
        /// @{
    private:
        pFutureKey_type&         m_pKey;
        /// @}

    };  // class InsertResultHandler

    // This is a future instead of just returning the value because eventually
    // this will be executing in a different thread.
    pFutureKey_type pKey(new FutureKey_type);

    pQueryResultHandler_type pHandler(
        new InsertResultHandler(pKey)
    );

    DatabaseQuery* pQuery = 
        dynamic_cast<DatabaseQuery*>(_pQuery.get());
    assert(pQuery != NULL);

    std::stringstream stream;
    stream << pQuery->getQuery();
    stream << " RETURNING " << _keyName;

    DatabaseStaticQuery* pStaticQuery =
        dynamic_cast<DatabaseStaticQuery*>(_pQuery.get());
    assert(pStaticQuery != NULL);

    pStaticQuery->setQuery(stream.str());

    executeQuery(_pQuery, pHandler);

    return pKey;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
DatabaseTransaction::pDatabaseCommand_type
DatabaseTransaction::createCommand()
{
    DatabaseCommand* pRawDatabaseCommand = new DatabaseCommand();

    pDatabaseCommand_type pDatabaseCommand(
        pRawDatabaseCommand,
        boost::bind(&DatabaseTransaction::onDestroyDatabaseCommand,this,_1)
    );

    return pDatabaseCommand;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
DatabaseTransaction::executeCommand(pDatabaseCommand_type _pCommand, pCommandResultHandler_type _pCommandResultHandler)
{
    // TODO Implement
    throw Utility::runtime_exception("DatabaseTransaction::executeCommand() : Not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
DatabaseTransaction::commit()
{
    PGresult* pResult = PQexec(m_pConnection, "COMMIT");
    if (PQresultStatus(pResult) != PGRES_COMMAND_OK)
    {
        PQclear(pResult);
        std::stringstream message;
        message << "DatabaseTransaction::createStaticQuery() : Error committing transaction -- "
            << std::string(PQerrorMessage(m_pConnection));
        throw Utility::runtime_exception(message.str().c_str());
    }

    m_isCommitted = true;
    m_isActive = false;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
DatabaseTransaction::rollback()
{
    PGresult* pResult = PQexec(m_pConnection, "ROLLBACK");
    if (PQresultStatus(pResult) != PGRES_COMMAND_OK)
    {
        PQclear(pResult);
        std::stringstream message;
        message << "DatabaseTransaction::createStaticQuery() : Error rolling back transaction -- "
            << std::string(PQerrorMessage(m_pConnection));
        throw Utility::runtime_exception(message.str().c_str());
    }

    m_isCommitted = false;
    m_isActive = false;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
DatabaseTransaction::onDestroyStaticQuery(wpStaticQuery_type _wpStaticQuery)
{
    /// Fire the DatabaseStaticQuery's onDestroyEvent
    _wpStaticQuery->onDestroyEvent(_wpStaticQuery);

    /// Delete the DatabaseStaticQuery
    DatabaseStaticQuery* pStaticQuery =
        dynamic_cast<DatabaseStaticQuery*>(_wpStaticQuery.get());

    if( pStaticQuery )
    {
        delete pStaticQuery;
    }
    else
    {
        throw Utility::runtime_exception("Zen::Database::DatabaseTransaction::onDestroyStaticQuery() : _wpStaticQuery is not a valid DatabaseStaticQuery.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
DatabaseTransaction::reset()
{
    if(m_isActive == true )
    {
        this->rollback();
    }

    this->m_isActive = true;
    this->m_isCommitted = false;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
std::string
DatabaseTransaction::generateStatementName(DatabaseQuery* _pQuery)
{
    boost::hash<std::string> string_hash;

    std::stringstream name;
    name << string_hash(std::string((const char *)&_pQuery, sizeof(DatabaseQuery*)));

    return name.str();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
DatabaseTransaction::isFinalized() const
{
    return !m_isActive;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
DatabaseTransaction::isActive() const
{
    return m_isActive;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
DatabaseTransaction::onDestroyDynamicQuery(wpDynamicQuery_type _wpDynamicQuery)
{
    /// Fire the DatabaseDynamicQuery's onDestroyEvent
    _wpDynamicQuery->onDestroyEvent(_wpDynamicQuery);

    /// Delete the DatabaseDynamicQuery
    DatabaseDynamicQuery* pDynamicQuery =
        dynamic_cast<DatabaseDynamicQuery*>(_wpDynamicQuery.get());

    if( pDynamicQuery )
    {
        delete pDynamicQuery;
    }
    else
    {
        throw Utility::runtime_exception("Zen::Database::DatabaseTransaction::onDestroyDynamicQuery() : _wpDynamicQuery is not a valid DatabaseDynamicQuery.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
DatabaseTransaction::onDestroyDatabaseCommand(wpDatabaseCommand_type _wpDatabaseCommand)
{
    /// Fire the DatabaseCommand's onDestroyEvent
    _wpDatabaseCommand->onDestroyEvent(_wpDatabaseCommand);

    /// Delete the DatabaseCommand
    DatabaseCommand* pDatabaseCommand =
        dynamic_cast<DatabaseCommand*>(_wpDatabaseCommand.get());

    if( pDatabaseCommand )
    {
        delete pDatabaseCommand;
    }
    else
    {
        throw Utility::runtime_exception("Zen::Database::DatabaseTransaction::onDestroyDatabaseCommand() : _wpDatabaseCommand is not a valid DatabaseCommand.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
DatabaseTransaction::onDestroyDatabaseResult(wpDatabaseResult_type _wpDatabaseResult)
{
    /// Fire the DatabaseResult's onDestroyEvent
    _wpDatabaseResult->onDestroyEvent(_wpDatabaseResult);

    /// Delete the DatabaseResult
    DatabaseResult* pDatabaseResult =
        dynamic_cast<DatabaseResult*>(_wpDatabaseResult.get());

    if( pDatabaseResult )
    {
        delete pDatabaseResult;
    }
    else
    {
        throw Utility::runtime_exception("Zen::Database::DatabaseTransaction::onDestroyDatabaseResult() : _wpDatabaseResult is not a valid DatabaseResult.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZPostgres
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
