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
#ifndef ZEN_ZPOSTGRES_DATA_ELEMENT_HPP_INCLUDED
#define ZEN_ZPOSTGRES_DATA_ELEMENT_HPP_INCLUDED

#include <Zen/Enterprise/DataModel/I_DataElement.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZPostgres {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class DataElement
:   public Zen::Enterprise::DataModel::I_DataElement
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_DataElement implementation
    /// @{
public:
    virtual UnderlyingType getUnderlyingType() const;
    virtual bool isNull();
    virtual bool isDirty();
    virtual void setDirty(bool _dirty);
    virtual boost::any getAnyValue() const;
    virtual void setAnyValue(const boost::any& _value);
    virtual I_DataElement& operator=(const boost::any& _value);
    virtual std::string getStringValue() const;
    virtual I_DataElement& operator=(const std::string& _value);
    virtual operator std::string();
    virtual Math::Real getRealValue() const;
    virtual boost::int64_t getInt64Value() const;
    virtual void setInt64Value(boost::int64_t _value);
    virtual I_DataElement& operator=(boost::int64_t _value);
    virtual operator boost::int64_t();
    virtual boost::posix_time::ptime getDateTimeValue() const;
    virtual I_DataElement& operator=(const boost::posix_time::ptime& _value);
    virtual operator boost::posix_time::ptime();
    virtual std::string getDBString() const;
    /// @}

    /// @name 'Structors
    /// @{
public:
             DataElement();
    virtual ~DataElement();
    /// @}

    /// @name Member Variables
    /// @{
private:
    typedef Zen::Memory::managed_ptr<Zen::Enterprise::DataModel::I_DataElement> pElement_type;
    pElement_type       m_pElement;
    /// @}

};  // class I_DataElement

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZPostgres
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZPOSTGRES_DATA_ELEMENT_HPP_INCLUDED
