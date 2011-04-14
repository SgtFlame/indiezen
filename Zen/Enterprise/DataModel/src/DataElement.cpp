//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Enterprise Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C) 2008 - 2010 Matthew Alan Gray
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
#include "DataElement.hpp"

#include <Zen/Core/Math/Math.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <boost/assign/list_of.hpp>
#include <boost/cstdint.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace DataModel {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
typedef std::map<
    std::string,
    DataElement::UnderlyingType
>   TypeMap_type;

static const TypeMap_type sm_typeMap = boost::assign::map_list_of
    (std::string(typeid(boost::uint8_t).name()), DataElement::INTEGER)
    (std::string(typeid(boost::uint16_t).name()), DataElement::INTEGER)
    (std::string(typeid(boost::uint32_t).name()), DataElement::INTEGER)
    (std::string(typeid(boost::uint64_t).name()), DataElement::INTEGER)
    (std::string(typeid(boost::int8_t).name()), DataElement::INTEGER)
    (std::string(typeid(boost::int16_t).name()), DataElement::INTEGER)
    (std::string(typeid(boost::int32_t).name()), DataElement::INTEGER)
    (std::string(typeid(boost::int64_t).name()), DataElement::INTEGER)
    (std::string(typeid(float).name()), DataElement::REAL)
    (std::string(typeid(double).name()), DataElement::REAL)
    (std::string(typeid(boost::posix_time::ptime).name()), DataElement::DATETIME)
    (std::string(typeid(std::string).name()), DataElement::STRING);
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
DataElement::DataElement()
:   m_type(NULL_TYPE)
,   m_dirty(false)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
DataElement::~DataElement()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
DataElement::UnderlyingType
DataElement::getUnderlyingType() const
{
    return m_type;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
DataElement::isNull()
{
    return m_type == NULL_TYPE;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
DataElement::isDirty()
{
    return m_dirty;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
DataElement::setDirty(bool _dirty)
{
    m_dirty = _dirty;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
boost::any
DataElement::getAnyValue() const
{
    return m_value;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
DataElement::setAnyValue(const boost::any& _value)
{
    if (!_value.empty())
    {
        TypeMap_type::const_iterator iter = sm_typeMap.find(std::string(_value.type().name()));
        if (iter != sm_typeMap.end())
        {
            m_type = iter->second;
        }
        else
        {
            m_type = UNKNOWN;
        }
    }
    else
    {
        m_type = NULL_TYPE;
    }

    m_value = _value;
    setDirty(true);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_DataElement&
DataElement::operator=(const boost::any& _value)
{
    setAnyValue(_value);
    setDirty(true);
    return *this;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
std::string
DataElement::getStringValue() const
{
    if (m_type == STRING)
    {
        return boost::any_cast<std::string>(m_value);
    }

    std::stringstream value;
    boost::posix_time::time_facet* time_facet(
        new boost::posix_time::time_facet("%Y-%m-%d %H:%M:%S")
    );

    switch(m_type)
    {
    case NULL_TYPE:
        break;
    case INTEGER:
        value << boost::any_cast<boost::int64_t>(m_value);
        break;
    case REAL:
        value << boost::any_cast<Math::Real>(m_value);
        break;
    case DATETIME:
        value.imbue(std::locale(value.getloc(), time_facet));
        value << boost::any_cast<boost::posix_time::ptime>(m_value);
        break;
    case UNKNOWN:
        if (m_value.type() == typeid(std::string))
        {
            return boost::any_cast<std::string>(m_value);
        }
        else if (m_value.type() == typeid(boost::int64_t))
        {
            value << boost::any_cast<boost::int64_t>(m_value);
            return value.str();
        }
        else if (m_value.type() == typeid(boost::uint64_t))
        {
            value << boost::any_cast<boost::uint64_t>(m_value);
            return value.str();
        }
        else if (m_value.type() == typeid(boost::posix_time::ptime))
        {
            /// TODO Implement
            return value.str();
        }
        // No break here.  Error if type wasn't found.
    default:
        // TODO Implement type coercion
        throw Utility::runtime_exception("DataElement::getStringValue(): Error, not implemented");
    }

    return value.str();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_DataElement&
DataElement::operator=(const std::string& _value)
{
    m_value = _value;
    m_type = STRING;
    setDirty(true);
    return *this;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
DataElement::operator std::string()
{
    return getStringValue();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Math::Real
DataElement::getRealValue() const
{
    if (m_type == REAL)
    {
        return boost::any_cast<Math::Real>(m_value);
    }

    throw Utility::runtime_exception("DataElement::getRealValue(): Error, not implemented");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
boost::int64_t
DataElement::getInt64Value() const
{
    if (m_type == INTEGER)
    {
        return boost::any_cast<boost::int64_t>(m_value);
    }

    switch(m_type)
    {
    case UNKNOWN:
        if (m_value.type() == typeid(std::string))
        {
            std::string value = boost::any_cast<std::string>(m_value);
            return strtol(value.c_str(), NULL, 10);
        }
        else if (m_value.type() == typeid(boost::int64_t))
        {
            return boost::any_cast<boost::int64_t>(m_value);
        }
        else if (m_value.type() == typeid(boost::uint64_t))
        {
            return boost::any_cast<boost::uint64_t>(m_value);
        }
        break;
    }

    throw Utility::runtime_exception("DataElement::getInt64Value(): Error, not implemented");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
DataElement::setInt64Value(boost::int64_t _value)
{
    m_value = _value;
    m_type = INTEGER;
    setDirty(true);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_DataElement&
DataElement::operator=(boost::int64_t _value)
{
    setInt64Value(_value);
    return *this;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
DataElement::operator boost::int64_t()
{
    return getInt64Value();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
boost::posix_time::ptime
DataElement::getDateTimeValue() const
{
    if (m_type == DATETIME)
    {
        return boost::any_cast<boost::posix_time::ptime>(m_value);
    }

    throw Utility::runtime_exception("DataElement::getDateTimeValue(): Error, not implemented");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
DataElement::setDateTimeValue(const boost::posix_time::ptime& _value)
{
    m_value = _value;
    m_type = DATETIME;
    setDirty(true);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_DataElement&
DataElement::operator=(const boost::posix_time::ptime& _value)
{
    setDateTimeValue(_value);
    return *this;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
DataElement::operator boost::posix_time::ptime()
{
    return getDateTimeValue();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
std::string
DataElement::getDBString() const
{
    std::stringstream stream;

    if (m_type == STRING || m_type == DATETIME)
    {
        stream << "\'";
        stream << escapeString(getStringValue());
        stream << "\'";
    }
    else
    {
        stream << getStringValue();
    }

    return stream.str();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
std::string
DataElement::escapeString(const std::string& _string)
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
}   // namespace DataModel
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
