//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C) 2008 - 2009 Matthew Alan Gray
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
#ifndef ZEN_ENGINE_MOVE_MANAGER_I_CONTROLLABLE_HPP_INCLUDED
#define ZEN_ENGINE_MOVE_MANAGER_I_CONTROLLABLE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Engine/Core/I_GameObject.hpp>
#include <Zen/Core/Scripting/I_ScriptableType.hpp>
#include <Zen/Core/Memory/managed_ptr.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace MoveManager {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_Move;

/// Controllable object implement this interface
class MOVEMANAGER_DLL_LINK I_Controllable
:   public virtual Zen::Scripting::I_ScriptableType
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<I_Move>  pMove_type;

    /// @name I_Controllable interface
    /// @{
protected:
    friend class I_MoveService;
    /// This should only be called directly by the I_MoveService 
    virtual void processMove(const I_Move& _move) = 0;
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    /// This is implemented to return "Controllable"
    /// Override this method if you create a derived type
    const std::string& getScriptTypeName();
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_Controllable();
    virtual ~I_Controllable();
    /// @}

};  // interface I_Controllable

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace MoveManager
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_MOVE_MANAGER_I_CONTROLLABLE_HPP_INCLUDED
