//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
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
#include "PostgresTypes.hpp"
#include "DatabaseColumn.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

Zen::ZPostgres::DatabaseTypes gDatabaseTypes;

#include <boost/assign/list_of.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/cstdint.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <string>
#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZPostgres {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class DataConverter
{
public:
    virtual const std::type_info& getTypeInfo() = 0;
    virtual boost::any getAnyValue(const std::string& _value) = 0;
};  // class DataConverter

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
        if (_value == "")
        {
            return boost::any();
        }

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
        if (_value == "")
        {
            return boost::any();
        }

        return boost::any(boost::lexical_cast<double, std::string>(_value));
    }

};  // class RealDataConverter

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class BoolDataConverter
:	public DataConverter
{
public:
	virtual const std::type_info& getTypeInfo()
	{
		return typeid(bool);
	}

	virtual boost::any getAnyValue(const std::string& _value)
	{
		if (_value == "")
		{
			return boost::any();
		}

		return _value == "t" ? true : false;
		//return boost::any(boost::lexical_cast<bool, std::string>(_value));
	}
};	// class BoolDataConverter

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class TimestampDataConverter
:   public DataConverter
{
public:
    virtual const std::type_info& getTypeInfo()
    {
        return typeid(boost::posix_time::ptime);
    }

    virtual boost::any getAnyValue(const std::string& _value)
    {
        if (_value == "")
        {
            return boost::any();
        }

        return boost::any(boost::posix_time::time_from_string(_value));
    }

};  // class TimestampDataConverter

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
        if (_value == "")
        {
            return boost::any();
        }

        return boost::any(_value);
    }

};  // class StringDataConverter

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const DatabaseTypes::ConversionMap_type DatabaseTypes::sm_dataConversionMap = boost::assign::map_list_of
	(DatabaseTypes::BOOLOID, boost::shared_ptr<DataConverter>(new BoolDataConverter))
    (DatabaseTypes::INT8OID, boost::shared_ptr<DataConverter>(new IntegerDataConverter))
    (DatabaseTypes::INT2OID, boost::shared_ptr<DataConverter>(new IntegerDataConverter))
    (DatabaseTypes::INT4OID, boost::shared_ptr<DataConverter>(new IntegerDataConverter))
    (DatabaseTypes::FLOAT4OID, boost::shared_ptr<DataConverter>(new RealDataConverter))
    (DatabaseTypes::FLOAT8OID, boost::shared_ptr<DataConverter>(new RealDataConverter))
    (DatabaseTypes::TIMESTAMPOID, boost::shared_ptr<DataConverter>(new TimestampDataConverter))
    (DatabaseTypes::VARCHAROID, boost::shared_ptr<DataConverter>(new StringDataConverter));

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
DatabaseTypes::getTypeInfo(int _fieldType) const
{
    ConversionMap_type::const_iterator iter =
        sm_dataConversionMap.find(_fieldType);

    if (iter == sm_dataConversionMap.end())
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
DatabaseTypes::getData(PGresult* _pResult, const DatabaseColumn& _column, int _rowNumber, int _fieldNumber) const
{
    std::string stringValue((const char*)PQgetvalue(_pResult, _rowNumber, _fieldNumber));

    ConversionMap_type::const_iterator iter =
        sm_dataConversionMap.find(PQftype(_pResult, _fieldNumber));

    if (iter != sm_dataConversionMap.end())
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
}   // namespace ZPostgres
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
