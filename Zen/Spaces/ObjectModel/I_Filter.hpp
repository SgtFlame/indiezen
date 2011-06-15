//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Spaces
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C) 2008 - 2009 Matthew Alan Gray
//
// Licensed under the Games by Sarge Publishing License - See your licensing
// agreement for terms and conditions.
//
// Do not redistribute this source code.
//
//  Tony Richards trichards@gamesbysarge.com
//  Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_OBJECTMODEL_I_FILTER_HPP_INCLUDED
#define ZEN_OBJECTMODEL_I_FILTER_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Enterprise/DataModel/I_DataElement.hpp>

#include <boost/noncopyable.hpp>
#include <boost/any.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Spaces {
namespace ObjectModel {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

class OBJECTMODEL_DLL_LINK I_Filter
:   boost::noncopyable
{
    /// @name Forward Declarations
    /// @{
public:
    struct I_ConstraintVisitor;
    /// @}

    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<I_Filter>                                  pFilter_type;
    /// @}

    /// @name I_Filter interface.
    /// @{
public:
    /// Add scalar constraint.
    virtual void addScalarConstraint(const std::string& _field, boost::any& _value) = 0;

    /// Add exclusion constraint.
    virtual void addExclusionConstraint(const std::string& _field, boost::any& _value) = 0;

    /// Get the constraints represented by this filter via a visitor pattern.
    virtual void getConstraints(I_ConstraintVisitor& _visitor) const = 0;
    /// @}

    /// @name Static methods
    /// @{
public:
    static pFilter_type create();
    /// @}

    /// @name Inner structures
    /// @{
public:
    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    class I_Constraint
    {
        /// @name Types
        /// @{
    public:
        typedef Zen::Memory::managed_ptr<Zen::Enterprise::DataModel::I_DataElement> pElement_type;
        typedef Zen::Enterprise::DataModel::I_DataElement&                          ElementReference_type;
        /// @}

        /// @name I_Constraint interface.
        /// @{
    public:
        /// Get the field name against which to apply this constraint.
        virtual const std::string& getFieldName() const = 0;

        /// Append this constraint to the query.
        virtual void appendQuery(std::string& _queryString) const = 0;
        /// @}

        /// @name 'Structors
        /// @{
    public:
                 I_Constraint();
        virtual ~I_Constraint();
        /// @}

    };  // interface I_Constraint
    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    class I_ScalarConstraint
    :   public I_Constraint
    {
        /// @name Types
        /// @{
    public:
        /// @}

        /// @name I_ScalarConstraint interface.
        /// @{
    public:
        virtual ElementReference_type getValue() const = 0;
        /// @}

        /// @name 'Structors
        /// @{
    public:
                 I_ScalarConstraint();
        virtual ~I_ScalarConstraint();
        /// @}

    };  // interface I_ScalarConstraint
    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    class I_ExclusionConstraint
    :   public I_ScalarConstraint
    {
        /// @name Types
        /// @{
    public:
        /// @}

        /// @name I_ExclusionConstraint interface.
        /// @{
    public:
        /// @}

        /// @name 'Structors
        /// @{
    public:
                 I_ExclusionConstraint();
        virtual ~I_ExclusionConstraint();
        /// @}

    };  // interface I_ExclusionConstraint
    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    struct I_ConstraintVisitor
    {
        virtual void begin() = 0;
        virtual void visit(const I_Constraint& _constraint) = 0;
        virtual void end() = 0;

    };  // interface I_ConstraintVisitor
    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_Filter();
    virtual ~I_Filter();
    /// @}

};  // interface I_Filter

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ObjectModel
}   // namespace Spaces
namespace Memory {
    // I_Filter is managed by factory
    template<>
    struct is_managed_by_factory<Zen::Spaces::ObjectModel::I_Filter> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_OBJECTMODEL_I_FILTER_HPP_INCLUDED
