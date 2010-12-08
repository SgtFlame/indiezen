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
#ifndef ZEN_OBJECTMODEL_FILTER_HPP_INCLUDED
#define ZEN_OBJECTMODEL_FILTER_HPP_INCLUDED

#include "../Configuration.hpp"

#include <Zen/Spaces/ObjectModel/I_Filter.hpp>

#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Spaces {
namespace ObjectModel {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class Filter
:   public I_Filter
{
    /// @name Types
    /// @{
public:
    typedef std::map<std::string, I_Constraint*> ConstraintsMap_type;
    /// @}

    /// @name I_Filter implementation
    /// @{
public:
    virtual void addScalarConstraint(const std::string& _field, boost::any& _value);
    virtual void getConstraints(I_ConstraintVisitor& _visitor) const;
    /// @}

    /// @name Filter implementation
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class I_Filter;
             Filter();
public:
    virtual ~Filter();
    /// @}

    /// @name Member Variables
    /// @{
private:
    ConstraintsMap_type         m_constraints;
    /// @}

};  // class Filter

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ObjectModel
}   // namespace Spaces
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_OBJECTMODEL_FILTER_HPP_INCLUDED
