//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Spaces
//
// Copyright (C) 2001 - 2009 Tony Richards
//
// Licensed under the Games by Sarge Publishing License - See your licensing
// agreement for terms and conditions.
//
// Do not redistribute this source code.
//
// Tony Richards trichards@gamesbysarge.com
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// This is generated by the Zen Spaces Code Generator.  Do not modify!
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include "GameObjectElementDataMap.hpp"

#include "GameObjectElementDomainObject.hpp"
#include "GameObjectElementDataCollection.hpp"

#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <Zen/Enterprise/Database/I_DatabaseStaticQuery.hpp>
#include <Zen/Enterprise/Database/I_DatabaseResult.hpp>
#include <Zen/Enterprise/Database/I_DatabaseColumn.hpp>
#include <Zen/Enterprise/Database/I_DatabaseRow.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace GameBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

GameObjectElementDataMap::GameObjectElementDataMap(pDatabaseConnection_type _pDatabaseConnection)
:   m_pDatabaseConnection(_pDatabaseConnection)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GameObjectElementDataMap::~GameObjectElementDataMap()
{
}

static void 
destroyObject(Zen::Memory::managed_weak_ptr<I_GameObjectElementDomainObject> _wpObject);

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace {
struct FieldHandler
:   public Zen::Database::I_DatabaseRow::I_FieldVisitor
{
    virtual void begin()    {}

    virtual void visit(const Zen::Database::I_DatabaseColumn& _column, const boost::any& _value)
    {
        m_pValue->getElement(_column.getFieldName()).setAnyValue(_value);
    }

    virtual void end()      {}

    FieldHandler(GameObjectElementDataMap::pGameObjectElementDomainObject_type _pValue)
    : m_pValue(_pValue)
    {
    }

    virtual ~FieldHandler()
    {
    }

private:
    GameObjectElementDataMap::pGameObjectElementDomainObject_type       m_pValue;
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
struct RowHandler
:   public Zen::Database::I_DatabaseResult::I_ResultVisitor
{
    virtual void begin()
    {
    }

    virtual void visit(const Zen::Database::I_DatabaseRow& _databaseRow)
    {
        FieldHandler fieldHandler(m_pValue);

        _databaseRow.getFields(fieldHandler);
    }

    virtual void end()
    {
    }

    RowHandler(GameObjectElementDataMap::pGameObjectElementDomainObject_type _pValue)
    : m_pValue(_pValue)
    {
    }

    virtual ~RowHandler()
    {
    }
    
private:
    GameObjectElementDataMap::pGameObjectElementDomainObject_type           m_pValue;
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class ResultHandler
:   public Zen::Database::I_DatabaseTransaction::I_QueryResultHandler
{
public:
    virtual void handleResult(pDatabaseResult_type _pResult)
    {
        RowHandler rowHandler(m_pValue);

        _pResult->getResults(rowHandler);

        m_pFutureValue->setValue(m_pValue);

        // Done, so commit the transaction.
        m_pTransaction->commit();
    }

    /// @param _pTransaction - We want to keep a reference to the transaction so it does 
    ///         not go out of scope until the results have been fully handled.
    ResultHandler(Zen::Database::I_DatabaseConnection::pDatabaseTransaction_type _pTransaction,
                 GameObjectElementDataMap::pFutureGameObjectElementDomainObject_type _pFutureValue,
                 GameObjectElementDataMap::pGameObjectElementDomainObject_type _pValue)
    :   m_pTransaction(_pTransaction)
    ,   m_pFutureValue(_pFutureValue)
    ,   m_pValue(_pValue)
    {
    }

    virtual ~ResultHandler()
    {
    }

private:
    Zen::Database::I_DatabaseConnection::pDatabaseTransaction_type  m_pTransaction;
    GameObjectElementDataMap::pFutureGameObjectElementDomainObject_type     m_pFutureValue;
    GameObjectElementDataMap::pGameObjectElementDomainObject_type           m_pValue;
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
struct CollectionRowHandler
:   public Zen::Database::I_DatabaseResult::I_ResultVisitor
{
    virtual void begin()
    {
    }

    virtual void visit(const Zen::Database::I_DatabaseRow& _databaseRow)
    {
        GameObjectElementDataMap::pGameObjectElementDomainObject_type pValue(new GameObjectElementDomainObject, &destroyObject);
        
        m_pCollection->push_back(pValue);

        FieldHandler fieldHandler(pValue);

        _databaseRow.getFields(fieldHandler);
    }

    virtual void end()
    {
    }

    CollectionRowHandler(GameObjectElementDataMap::pGameObjectElementDataCollection_type _pCollection)
    : m_pCollection(_pCollection)
    {
    }

    virtual ~CollectionRowHandler()
    {
    }
    
private:
    GameObjectElementDataMap::pGameObjectElementDataCollection_type     m_pCollection;
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class CollectionResultHandler
:   public Zen::Database::I_DatabaseTransaction::I_QueryResultHandler
{
public:
    virtual void handleResult(pDatabaseResult_type _pResult)
    {
        CollectionRowHandler rowHandler(m_pCollection);

        _pResult->getResults(rowHandler);

        m_pFutureCollection->setValue(m_pCollection);

        // Done, so commit the transaction.
        m_pTransaction->commit();
    }

    /// @param _pTransaction - We want to keep a reference to the transaction so it does 
    ///         not go out of scope until the results have been fully handled.
    CollectionResultHandler(Zen::Database::I_DatabaseConnection::pDatabaseTransaction_type _pTransaction,
                 GameObjectElementDataMap::pFutureGameObjectElementDataCollection_type _pFutureCollection,
                 GameObjectElementDataMap::pGameObjectElementDataCollection_type _pCollection)
    :   m_pTransaction(_pTransaction)
    ,   m_pFutureCollection(_pFutureCollection)
    ,   m_pCollection(_pCollection)
    {
    }

    virtual ~CollectionResultHandler()
    {
    }

private:
    Zen::Database::I_DatabaseConnection::pDatabaseTransaction_type  m_pTransaction;
    GameObjectElementDataMap::pFutureGameObjectElementDataCollection_type     m_pFutureCollection;
    GameObjectElementDataMap::pGameObjectElementDataCollection_type     m_pCollection;
};

} // namespace
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static void
destroyObject(Zen::Memory::managed_weak_ptr<I_GameObjectElementDomainObject> _wpObject)
{
    GameObjectElementDomainObject* pObject = dynamic_cast<GameObjectElementDomainObject*>(_wpObject.get());
    if (pObject != NULL)
    {
        delete pObject;
    }
    else
    {
        // TODO Error?
    }
}
 
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static void
destroyCollection(Zen::Memory::managed_weak_ptr<I_GameObjectElementDataCollection> _wpCollection)
{
    GameObjectElementDataCollection* pCollection = dynamic_cast<GameObjectElementDataCollection*>(_wpCollection.get());
    if (pCollection != NULL)
    {
        delete pCollection;
    }
    else
    {
        // TODO Error?
    }
}
 
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GameObjectElementDataMap::pGameObjectElementDomainObject_type 
GameObjectElementDataMap::createNew()
{
    return pGameObjectElementDomainObject_type(new GameObjectElementDomainObject, &destroyObject);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GameObjectElementDataMap::pFutureGameObjectElementDomainObject_type
GameObjectElementDataMap::getByKey(boost::uint64_t _primaryKeyValue)
{
    // Start a transaction
    Zen::Database::I_DatabaseConnection::pDatabaseTransaction_type 
        pTransaction = getDatabaseConnection()->beginTransaction();

    // Create a query
    Zen::Database::I_DatabaseTransaction::pStaticQuery_type 
        pQuery = pTransaction->createStaticQuery();

	const boost::uint64_t primaryKeyValue(_primaryKeyValue);

    // Get a record from the GameObjectElement.
    std::stringstream query;
    query << "SELECT * FROM GameObjectElement WHERE gameObjectElementId = " << primaryKeyValue;

    pQuery->setQuery(query.str());
    
	pFutureGameObjectElementDomainObject_type pReturnValue(new FutureGameObjectElementDomainObject_type);
    pGameObjectElementDomainObject_type pValue(new GameObjectElementDomainObject, &destroyObject);

    // Create a result handler to handle the execution results
    Zen::Database::I_DatabaseTransaction::pQueryResultHandler_type pResultHandler(new ResultHandler(pTransaction, pReturnValue, pValue));

    // Execute the query.  
    pTransaction->executeQuery(pQuery, pResultHandler);
    
    return pReturnValue;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectElementDataMap::deleteByKey(boost::uint64_t _primaryKeyValue)
{
    // Don't do anything if the key value is 0.  But should this be an error?
    if (_primaryKeyValue == 0)
    {
        // TODO Throw an exception or return an error?
        return;
    }

    // Start a transaction
    Zen::Database::I_DatabaseConnection::pDatabaseTransaction_type 
        pTransaction = getDatabaseConnection()->beginTransaction();

    // Create a query
    Zen::Database::I_DatabaseTransaction::pStaticQuery_type 
        pQuery = pTransaction->createStaticQuery();

	const boost::uint64_t primaryKeyValue(_primaryKeyValue);

    // Delete a record in the GameObjectElement.
    std::stringstream query;
    query << "DELETE FROM GameObjectElement WHERE gameObjectElementId = " << primaryKeyValue;

    pQuery->setQuery(query.str());

    // Execute the query.  
    pTransaction->executeQuery(pQuery);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Database::I_DatabaseTransaction::pFutureKey_type
GameObjectElementDataMap::update(pGameObjectElementDomainObject_type _pGameObjectElementDomainObject)
{
    // Start a transaction
    Zen::Database::I_DatabaseConnection::pDatabaseTransaction_type 
        pTransaction = getDatabaseConnection()->beginTransaction();

    // Create a query
    Zen::Database::I_DatabaseTransaction::pStaticQuery_type 
        pQuery = pTransaction->createStaticQuery();

    std::stringstream query;

    if (_pGameObjectElementDomainObject->getGameObjectElementId().isNull())
    {
        // Construct the query to insert a record into the GameObjectElement table.

        query << "INSERT into GameObjectElement ( "
            << getNonPKFieldNames() << " ) VALUES ( ";

        // Append all of the values
        std::string escapedStringValue;
        
        escapedStringValue = escapeString(_pGameObjectElementDomainObject->getGameObjectTypeId().getStringValue());
        query << "\'" << escapedStringValue << "\'";
        escapedStringValue = escapeString(_pGameObjectElementDomainObject->getOverrideParent().getStringValue());
        query << ", \'" << escapedStringValue << "\'";
        escapedStringValue = escapeString(_pGameObjectElementDomainObject->getSequenceNumber().getStringValue());
        query << ", \'" << escapedStringValue << "\'";
        escapedStringValue = escapeString(_pGameObjectElementDomainObject->getName().getStringValue());
        query << ", \'" << escapedStringValue << "\'";
        escapedStringValue = escapeString(_pGameObjectElementDomainObject->getDataType().getStringValue());
        query << ", \'" << escapedStringValue << "\'";
        escapedStringValue = escapeString(_pGameObjectElementDomainObject->getDefaultValue().getStringValue());
        query << ", \'" << escapedStringValue << "\'";
        escapedStringValue = escapeString(_pGameObjectElementDomainObject->getPropertiesId().getStringValue());
        query << ", \'" << escapedStringValue << "\'";

        // Final closing parenthesis
        query << ")";

        pQuery->setQuery(query.str());

        // Execute the query.
        Zen::Database::I_DatabaseTransaction::pFutureKey_type 
            pKey(pTransaction->executeInsert(pQuery));

        _pGameObjectElementDomainObject->getGameObjectElementId() = pKey->getValue();
        
        // Done, so commit the transaction.
        pTransaction->commit();

        return pKey;
    }
    else
    {
        // Construct the query to update a record into the GameObjectElement table.

        // Count the number of dirty fields.
        int dirtyFields = 0;

        query << "UPDATE GameObjectElement SET ";

        std::string separator("");

        // Append all of the field names and values
        std::string escapedStringValue;

        if (_pGameObjectElementDomainObject->getGameObjectTypeId().isDirty())
        {
            escapedStringValue = escapeString(_pGameObjectElementDomainObject->getGameObjectTypeId().getStringValue());
            query << separator << "gameObjectTypeId = \'" 
                << escapedStringValue << "\'";
            separator = ", ";
            dirtyFields++;
        }
        if (_pGameObjectElementDomainObject->getOverrideParent().isDirty())
        {
            escapedStringValue = escapeString(_pGameObjectElementDomainObject->getOverrideParent().getStringValue());
            query << separator << "overrideParent = \'" 
                << escapedStringValue << "\'";
            separator = ", ";
            dirtyFields++;
        }
        if (_pGameObjectElementDomainObject->getSequenceNumber().isDirty())
        {
            escapedStringValue = escapeString(_pGameObjectElementDomainObject->getSequenceNumber().getStringValue());
            query << separator << "sequenceNumber = \'" 
                << escapedStringValue << "\'";
            separator = ", ";
            dirtyFields++;
        }
        if (_pGameObjectElementDomainObject->getName().isDirty())
        {
            escapedStringValue = escapeString(_pGameObjectElementDomainObject->getName().getStringValue());
            query << separator << "name = \'" 
                << escapedStringValue << "\'";
            separator = ", ";
            dirtyFields++;
        }
        if (_pGameObjectElementDomainObject->getDataType().isDirty())
        {
            escapedStringValue = escapeString(_pGameObjectElementDomainObject->getDataType().getStringValue());
            query << separator << "dataType = \'" 
                << escapedStringValue << "\'";
            separator = ", ";
            dirtyFields++;
        }
        if (_pGameObjectElementDomainObject->getDefaultValue().isDirty())
        {
            escapedStringValue = escapeString(_pGameObjectElementDomainObject->getDefaultValue().getStringValue());
            query << separator << "defaultValue = \'" 
                << escapedStringValue << "\'";
            separator = ", ";
            dirtyFields++;
        }
        if (_pGameObjectElementDomainObject->getPropertiesId().isDirty())
        {
            escapedStringValue = escapeString(_pGameObjectElementDomainObject->getPropertiesId().getStringValue());
            query << separator << "propertiesId = \'" 
                << escapedStringValue << "\'";
            separator = ", ";
            dirtyFields++;
        }

        // No need to proceed if none of the fields were dirty.
        if (dirtyFields > 0)
        {
            // WHERE clause to update the record
            query << " WHERE gameObjectElementId = " << 
                _pGameObjectElementDomainObject->getGameObjectElementId().getInt64Value();

            pQuery->setQuery(query.str());

            // Execute the query.  
            pTransaction->executeQuery(pQuery);

            // Done, so commit the transaction.
            pTransaction->commit();
        }
        
        Zen::Database::I_DatabaseTransaction::pFutureKey_type 
            pKey(new Zen::Database::I_DatabaseTransaction::FutureKey_type);

        pKey->setValue(_pGameObjectElementDomainObject->getGameObjectElementId().getInt64Value());

        return pKey;
    }

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GameObjectElementDataMap::pFutureGameObjectElementDataCollection_type
GameObjectElementDataMap::getByGameObjectTypeId(const boost::uint64_t _gameObjectTypeId)
{
    pFutureGameObjectElementDataCollection_type pFutureCollection(new FutureGameObjectElementDataCollection_type);
    pGameObjectElementDataCollection_type pCollection(new GameObjectElementDataCollection, &destroyCollection);

    // Start a transaction
    Zen::Database::I_DatabaseConnection::pDatabaseTransaction_type 
        pTransaction = getDatabaseConnection()->beginTransaction();

    // Create a query
    Zen::Database::I_DatabaseTransaction::pStaticQuery_type 
        pQuery = pTransaction->createStaticQuery();

    // Get a collection of GameObjectElements by gameObjectTypeId.
    std::stringstream query;
    query << "SELECT * FROM GameObjectElement WHERE " << "gameObjectTypeId = " << _gameObjectTypeId << " ORDER by sequenceNumber";

    pQuery->setQuery(query.str());
    
    // Create a result handler to handle the execution results
    Zen::Database::I_DatabaseTransaction::pQueryResultHandler_type pResultHandler(new CollectionResultHandler(pTransaction, pFutureCollection, pCollection));

    // Execute the query.  
    pTransaction->executeQuery(pQuery, pResultHandler);    
    
    return pFutureCollection;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GameObjectElementDataMap::pFutureGameObjectElementDataCollection_type
GameObjectElementDataMap::getAll()
{
    pFutureGameObjectElementDataCollection_type pFutureCollection(new FutureGameObjectElementDataCollection_type);
    pGameObjectElementDataCollection_type pCollection(new GameObjectElementDataCollection, &destroyCollection);

    // Start a transaction
    Zen::Database::I_DatabaseConnection::pDatabaseTransaction_type 
        pTransaction = getDatabaseConnection()->beginTransaction();

    // Create a query
    Zen::Database::I_DatabaseTransaction::pStaticQuery_type 
        pQuery = pTransaction->createStaticQuery();

    // Get all of the records from the GameObjectElement.
    pQuery->setQuery("SELECT * FROM GameObjectElement");

    // Create a result handler to handle the execution results
    Zen::Database::I_DatabaseTransaction::pQueryResultHandler_type pResultHandler(new CollectionResultHandler(pTransaction, pFutureCollection, pCollection));

    // Execute the query.  
    pTransaction->executeQuery(pQuery, pResultHandler);    
    
    return pFutureCollection;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
        
std::string
GameObjectElementDataMap::escapeString(const std::string& _value) const
{
	return getDatabaseConnection()->escapeString(_value);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static std::string sm_fieldNames = "gameObjectElementId, gameObjectTypeId, overrideParent, sequenceNumber, name, dataType, defaultValue, propertiesId";
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static std::string sm_nonPKFieldNames = "gameObjectTypeId, overrideParent, sequenceNumber, name, dataType, defaultValue, propertiesId";
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
GameObjectElementDataMap::getFieldNames()
{
    return sm_fieldNames;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
GameObjectElementDataMap::getNonPKFieldNames()
{
    return sm_nonPKFieldNames;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static std::string sm_tableName = "GameObjectElement";
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
GameObjectElementDataMap::getTableName()
{
    return sm_tableName;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GameObjectElementDataMap::pDatabaseConnection_type
GameObjectElementDataMap::getDatabaseConnection() const
{
	return m_pDatabaseConnection;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace GameBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

