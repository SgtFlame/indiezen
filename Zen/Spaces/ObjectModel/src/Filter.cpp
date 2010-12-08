//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Spaces
//
// Copyright (C) 2001 - 2010 Tony Richards
// Copyright (C) 2008 - 2010 Matthew Alan Gray
//
// Licensed under the Games by Sarge Publishing License - See your licensing
// agreement for terms and conditions.
//
// Do not redistribute this source code.
//
//  Tony Richards trichards@gamesbysarge.com
//  Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#include "Filter.hpp"

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Spaces {
namespace ObjectModel {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class ScalarConstraint
:   public I_Filter::I_ScalarConstraint
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_Constraint implementation
    /// @{
public:
    virtual const std::string& getFieldName() const
    {
        return m_fieldName;
    }

    virtual void appendQuery(std::string& _queryString) const
    {
        std::stringstream query;
        query << _queryString;
        query << m_fieldName << "=\'" << m_pValue->getStringValue() << "\'";
        _queryString = query.str();
    }
    /// @}

    /// @name I_ScalarConstraint implementation
    /// @{
public:
    virtual ElementReference_type getValue() const
    {
        return *m_pValue.get();
    }
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class Filter;
    ScalarConstraint(const std::string _fieldName, boost::any& _value)
    :   m_fieldName(_fieldName)
    ,   m_pValue(Zen::Enterprise::DataModel::I_DataElement::create())
    {
        m_pValue->setAnyValue(_value);
    }

    virtual ~ScalarConstraint() {}
    /// @}

    /// @name Member Variables
    /// @{
private:
    const std::string       m_fieldName;
    pElement_type           m_pValue;
    /// @}

};  // class ScalarConstraint

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Filter::Filter()
:   m_constraints()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Filter::~Filter()
{
    ConstraintsMap_type::iterator iter = m_constraints.begin();
    while (iter != m_constraints.end())
    {
        delete iter->second;
        iter++;
    }
    m_constraints.clear();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Filter::addScalarConstraint(const std::string& _field, boost::any& _value)
{
    ConstraintsMap_type::iterator iter = m_constraints.find(_field);
    if (iter == m_constraints.end())
    {
        m_constraints[_field] = new ScalarConstraint(_field, _value);
    }

    /// TODO Throw exception?
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Filter::getConstraints(I_ConstraintVisitor& _visitor) const
{
    _visitor.begin();

    ConstraintsMap_type::const_iterator iter = m_constraints.begin();
    while (iter != m_constraints.end())
    {
        _visitor.visit(*iter->second);
        iter++;
    }

    _visitor.end();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ObjectModel
}   // namespace Spaces
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
