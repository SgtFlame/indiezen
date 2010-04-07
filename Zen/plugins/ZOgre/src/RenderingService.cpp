//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2007 Tony Richards
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
#include "RenderingService.hpp"
#include "RenderingView.hpp"
#include "SceneService.hpp"
#include "SceneNode.hpp"
#include "AttachableObject.hpp"
#include "ParticleSystem.hpp"

#include "NullContext.hpp"

#include <Zen/Engine/Rendering/I_RenderingManager.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZOgre {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
RenderingService::RenderingService()
:   m_root(Ogre::Root::getSingleton())
,   m_pScriptObject(NULL)
,   m_pModule(NULL)
{
    std::cout << "new RenderingService" << std::endl;

    // TODO Where should this go?
    std::cout << "OGRE: m_root.restoreConfig()" << std::endl;
    if (m_root.restoreConfig())
    {
        // we found a valid ogre.cfg with our rendering configuration in it
        std::cout << "Rendering configuration loaded from file 'ogre.cfg'.  Delete it to show dialog at startup." << std::endl;
        m_root.initialise(false);
        Ogre::ResourceGroupManager::getSingleton().addResourceLocation("./materials/textures", "FileSystem", "General", false);
        return;
    }

    // there was no valid ogre.cfg file to read from, so let's show the dialog
    std::cout << "OGRE: m_root.showConfigDialog()" << std::endl;
    if (m_root.showConfigDialog())
    {
        std::cout << "Rendering configuration loaded from on-screen dialog." << std::endl;
        m_root.initialise(false);
        Ogre::ResourceGroupManager::getSingleton().addResourceLocation("./materials/textures", "FileSystem", "General", false);
        return;
    }

    // if we get to this point it means the user hit Cancel on the config dialog
    // TODO - we should shutdown - how do we do this from here?
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
RenderingService::~RenderingService()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Engine::Rendering::I_Context*
RenderingService::createContext(window_handle_type _pParent)
{
    return new NullContext(*m_pModule, _pParent ? _pParent :
#ifdef _WIN32
        NULL
#else
        ""
#endif
        );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
RenderingService::destroyContext(Zen::Engine::Rendering::I_Context* _pContext)
{
    // TODO Implement
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Engine::Rendering::I_View*
RenderingService::createView(Zen::Engine::Rendering::I_Context& _context, const std::string& _windowName, unsigned int _width, unsigned int _height)
{
    // TODO Pass _context to the constructor?
    RenderingView* pView = new RenderingView(*m_pModule, _context, _windowName, _width, _height);

    return pView;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
RenderingService::destroyView(Zen::Engine::Rendering::I_View* _pView)
{
    // TODO Implement
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Scripting::I_ObjectReference*
RenderingService::getScriptObject()
{
    // TODO Make thread safe?
    if (m_pScriptObject == NULL)
    {
        m_pScriptObject = new ScriptObjectReference_type(
            m_pModule->getScriptModule(),
            m_pModule->getScriptModule()->getScriptType(getScriptTypeName()),
            getSelfReference().lock()
        );
    }

    return m_pScriptObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
RenderingService::registerScriptModule(Zen::Scripting::script_module& _module)
{
    m_pModule = &_module;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
RenderingService::showConfigDialog()
{
    return m_root.showConfigDialog();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZOgre
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
