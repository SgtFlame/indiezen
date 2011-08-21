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
#include "DatabaseResult.hpp"
#include "DatabaseColumn.hpp"
#include "DatabaseRow.hpp"

#include "libpq-fe.h"

#include <Zen/Core/Threading/I_Thread.hpp>

#include <boost/bind.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZPostgres {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
DatabaseResult::DatabaseResult(PGconn* _pConnection, const std::string& _statementName)
:   m_pConnection(_pConnection)
,   m_statementName(_statementName)
,   m_pResult(NULL)
,   m_done(false)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
DatabaseResult::~DatabaseResult()
{
    if (m_pResult != NULL)
    {
        PQclear(m_pResult);
        m_pResult = NULL;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
DatabaseResult::getColumns(I_ColumnVisitor& _visitor)
{
    if (m_pResult != NULL)
    {
        const int numFields = PQnfields(m_pResult);

        _visitor.begin();

        for(int i = 0 ; i < numFields ; i++ )
        {
            int columnType = PQftype(m_pResult, i);
            const char* const pColumnName = PQfname(m_pResult, i);

            DatabaseColumn column(pColumnName, columnType);
            _visitor.visit(column);
        }
        
        _visitor.end();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
DatabaseResult::getResults(I_ResultVisitor& _visitor)
{
    if (m_pResult != NULL)
    {
        /// TODO Async impl?
        const int rows = PQntuples(m_pResult);

        _visitor.begin();

        for (int i = 0; i < rows; ++i)
        {
            DatabaseRow row(i, m_pResult);
            _visitor.visit(row);
        }

        _visitor.end();
    }
}


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
DatabaseResult::finish()
{
    //while(!m_done)
    //{
        m_pResult = PQexecPrepared(
            m_pConnection,
            m_statementName.c_str(),
            0,
            NULL,
            NULL,
            NULL,
            0
        );

        //switch(PQresultStatus(m_pResult))
        //{
        /// TODO Async impl?
        //case PGRES_COPY_OUT:
        //case PGRES_COPY_IN:
        //    // Sleep for a bit then continue
        //    Threading::I_Thread::sleepForMilliseconds(100);
        //    continue;
        //case PGRES_COMMAND_OK:
        //case PGRES_TUPLES_OK:
        //    m_done = true;
        //    break;
        //default:
        //    // TODO Error?
        //    m_done = true;
        //    break;
        //}
    //}
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PGresult*
DatabaseResult::getPGResult() const
{
    return m_pResult;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZPostgres
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
