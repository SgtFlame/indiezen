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

#include "DatabaseService.hpp"
#include "DatabaseConnection.hpp"
#include "DataElement.hpp"

#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <Zen/Core/Threading/CriticalSection.hpp>
#include <Zen/Core/Threading/MutexFactory.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <boost/bind.hpp>

#include <sstream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZPostgres {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
DatabaseService::DatabaseService(config_type& _config)
{
    m_pConnectionsGuard = Threading::MutexFactory::create();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
DatabaseService::~DatabaseService()
{
    Threading::MutexFactory::destroy(m_pConnectionsGuard);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
DatabaseService::pDatabaseConnection_type
DatabaseService::connect(const std::string &_name, config_type& _config, bool _asynchronous)
{
    Threading::CriticalSection lock(m_pConnectionsGuard);

    connections_types::iterator iter = m_namedConnections.find(_name);
    if (iter != m_namedConnections.end())
    {
        pDatabaseConnection_type pConn(iter->second);

        onConnectedEvent(pConn);
        return pConn;
    }

    std::stringstream connInfo;
    config_type::const_iterator configIter = _config.begin();
    while (configIter != _config.end())
    {
        if (configIter->first != "text")
        {
            connInfo << configIter->first << "=\'" << configIter->second << "\' ";
        }
        configIter++;
    }

    PGconn* pConnection = PQconnectdb(connInfo.str().c_str());

    if (PQstatus(pConnection) == CONNECTION_OK)
    {
        pDatabaseConnection_type pConn(
            new DatabaseConnection(getSelfReference().lock(), _name, pConnection),
            boost::bind(&DatabaseService::onDestroy, this, _1)
        );

        wpDatabaseConnection_type pWeakConn(pConn);
        m_namedConnections[_name] = pWeakConn;

        onConnectedEvent(pConn);

        return pConn;
    }
    else
    {
        return pDatabaseConnection_type();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
DatabaseService::pElement_type
DatabaseService::createElement() const
{
    return pElement_type(new DataElement, &onDestroyElement);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
DatabaseService::onDestroy(wpDatabaseConnection_type _pConnection)
{
    Threading::CriticalSection lock(m_pConnectionsGuard);

    connections_types::iterator iter = m_namedConnections.find(_pConnection->getName());
    if (iter != m_namedConnections.end())
    {
        // This is safe since the only place that this can transition
        // from expired to non-expired is in connect() where m_pConnectionsGuard
        // is acquired.
        if (iter->second.expired())
        {
            m_namedConnections.erase(iter);

            DatabaseConnection* pConn = dynamic_cast<DatabaseConnection*>(_pConnection.get());

            pConn->onDestroyEvent(_pConnection);
            delete pConn;
        }
        else
        {
            // Not expired, so we are here for no reason
            // We can get here during race conditions where one shared_ptr
            // is being deleted while connect() is being called and connect()
            // beat onDestroy() to the lock(m_pConnectionsGuard).
            return;
        }
    }
    else
    {
        // TODO Severe error, should never get here
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
DatabaseService::onDestroyElement(wpElement_type _pElement)
{
    DataElement* pElement =
        dynamic_cast<DataElement*>(_pElement.get());

    if (pElement != NULL)
    {
        delete pElement;
    }
    else
    {
        throw Zen::Utility::runtime_exception("DatabaseService::onDestroy(wpElement) : Invalid type.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZPostgres
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
