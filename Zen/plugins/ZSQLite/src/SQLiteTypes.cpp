//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
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
#include "SQLiteTypes.hpp"
#include "DatabaseColumn.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

Zen::ZSQLite::DatabaseTypes gDatabaseTypes;

#include <boost/assign/list_of.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/cstdint.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZSQLite {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class DataConverter
{
public:
    virtual const std::type_info& getTypeInfo() = 0;
    virtual boost::any getAnyValue(const std::string& _value) = 0;
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class IntegerDataConverter
:   public DataConverter
{
public:
    virtual const std::type_info& getTypeInfo()
    {
        return typeid(boost::int64_t);
    }

    virtual boost::any getAnyValue(const std::string& _value)
    {
        return boost::any(boost::lexical_cast<boost::int64_t, std::string>(_value));
    }

};  // class IntegerData

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class RealDataConverter
:   public DataConverter
{
public:
    virtual const std::type_info& getTypeInfo()
    {
        return typeid(double);
    }

    virtual boost::any getAnyValue(const std::string& _value)
    {
        return boost::any(boost::lexical_cast<double, std::string>(_value));
    }

};  // class RealDataConverter

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class StringDataConverter
:   public DataConverter
{
public:
    virtual const std::type_info& getTypeInfo()
    {
        return typeid(std::string);
    }

    virtual boost::any getAnyValue(const std::string& _value)
    {
        return boost::any(_value);
    }

};  // class StringDataConverter

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const DatabaseTypes::ConversionMap_type DatabaseTypes::sm_dataConversionMap = boost::assign::map_list_of
    (std::string("INTEGER"), boost::shared_ptr<DataConverter>(new IntegerDataConverter))
    (std::string("FLOAT"), boost::shared_ptr<DataConverter>(new RealDataConverter))
    (std::string("VARCHAR"), boost::shared_ptr<DataConverter>(new StringDataConverter));

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
DatabaseTypes::DatabaseTypes()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
DatabaseTypes::~DatabaseTypes()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::type_info&
DatabaseTypes::getTypeInfo(const std::string& _fieldType) const
{
    ConversionMap_type::const_iterator iter = 
        sm_dataConversionMap.find(_fieldType);

    if(iter == sm_dataConversionMap.end())
    {
        // Default to a string
        return typeid(std::string);
    }
    else
    {
        return iter->second->getTypeInfo();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
boost::any
DatabaseTypes::getData(sqlite3_stmt* _pResult, const DatabaseColumn& _column, int _fieldNumber) const
{
    std::string stringValue(
        (const char*)sqlite3_column_text(_pResult, _fieldNumber)
    );

    ConversionMap_type::const_iterator iter = 
        sm_dataConversionMap.find(std::string(sqlite3_column_decltype(_pResult, _fieldNumber)));

    if(iter != sm_dataConversionMap.end())
    {
        return iter->second->getAnyValue(stringValue);
    }
    else
    {
        boost::any value(stringValue);
        return value;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZSQLite
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
