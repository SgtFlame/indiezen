//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2010 Tony Richards
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
#ifndef ZEN_SCRIPTING_HPP_INCLUDED
#define ZEN_SCRIPTING_HPP_INCLUDED

#include <Zen/Core/Scripting/forward_declarations.hpp>

// C++ framework API
#include <Zen/Core/Scripting/I_ScriptEngine.hpp>
#include <Zen/Core/Scripting/I_ScriptingManager.hpp>
#include <Zen/Core/Scripting/I_ScriptModule.hpp>
#include <Zen/Core/Scripting/I_ScriptMethod.hpp>
#include <Zen/Core/Scripting/I_ScriptType.hpp>
#include <Zen/Core/Scripting/I_ScriptableType.hpp>
#include <Zen/Core/Scripting/I_ScriptableService.hpp>
#include <Zen/Core/Scripting/ObjectReference.hpp>

// template declarations and some template implementation
#include <Zen/Core/Scripting/script_module.hpp>
#include <Zen/Core/Scripting/script_method.hpp>
#include <Zen/Core/Scripting/script_type_interface.hpp>
#include <Zen/Core/Scripting/script_type.hpp>
#include <Zen/Core/Scripting/script_convert_argument.hpp>
#include <Zen/Core/Scripting/script_dispatch_helper.hpp>

// template implementation
#include <Zen/Core/Scripting/script_module_impl.hpp>
#include <Zen/Core/Scripting/script_type_impl.hpp>

#endif
