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

#include "RenderingView.hpp"
#include "NullContext.hpp"

#include <Zen/Engine/Rendering/I_SceneService.hpp>

#include <iostream>
#include <sstream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZOgre {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
RenderingView::RenderingView(Zen::Scripting::script_module& _module, Zen::Engine::Rendering::I_Context& _context, const std::string& _windowName, unsigned int _width, unsigned int _height)
:   m_root(Ogre::Root::getSingleton())
,   m_context(_context)
,   m_pRenderWindow(NULL)
,   m_pCanvas(NULL)
,   m_module(_module)
,   m_pScriptObject(NULL)
{
    std::cout << "new RenderingView" << std::endl;

    Ogre::NameValuePairList nvpl;
    bool useParentWindow = false;

#ifdef WIN32
    // TODO Do something like the way the not WIN32 works
    if (m_context.getParentWindow() != NULL)
    {
        useParentWindow = true;
        nvpl["externalWindowHandle"] = Ogre::StringConverter::toString((int)m_context.getParentWindow());
        std::cout << "ZOgre::RenderingView is using parent window handle" << std::endl;
    }
    else
    {
        std::cout << "ZOgre::RenderingView is not using parent window handle" << std::endl;
    }

    //nvpl["parentWindowHandle"] = Ogre::StringConverter::toString((int)m_context.getParentWindow());
#else
    if (*m_context.getParentWindow())
    {
        useParentWindow = true;
        nvpl["externalWindowHandle"] = m_context.getParentWindow();
        std::cout << "ZOgre::RenderingView using " << m_context.getParentWindow() << " as the external window handle." << std::endl;
    }
    else
    {
        std::cout << "ZOgre::RenderingView not using parent window handle" << std::endl;
    }
#endif
    nvpl["title"] = _windowName.c_str();

    if (!useParentWindow)
    {
        Ogre::Root::getSingleton().getRenderSystem()->setConfigOption("Full Screen", "No");

        std::stringstream str;
        str << _width << " x " << _height;
        Ogre::Root::getSingleton().getRenderSystem()->setConfigOption("Video Mode", str.str());
    }

    std::cout << "OGRE: Ogre::Root::getSingleton().initialise(!useParentWindow, _windowName.c_str());" << std::endl;
    Ogre::Root::getSingleton().initialise(!useParentWindow, _windowName.c_str());

    if (useParentWindow)
    {
        std::cout << "OGRE: m_root.createRenderWindow(_windowName.c_str(), _width, _height, false, &nvpl);" << std::endl;
        m_pRenderWindow = m_root.createRenderWindow(_windowName.c_str(), _width, _height, false, &nvpl);
    }
    else
    {
        std::cout << "OGRE: m_root.getAutoCreatedWindow();" << std::endl;
        m_pRenderWindow = m_root.getAutoCreatedWindow();
    }

    // Dunno why this is done.
    std::cout << "OGRE: Ogre::RenderWindow::removeAllViewports()" << std::endl;
    m_pRenderWindow->removeAllViewports();

    // Set the window value in the _context
    NullContext* pContext = dynamic_cast<NullContext*>(&_context);

    assert(pContext);

    if (pContext != NULL)
    {
        // TODO This might be different depending on the OS.
        std::cout << "OGRE: Ogre::RenderWindow::getCustomAttribute(\"WINDOW\", &pContext->m_pHandle);" << std::endl;
        m_pRenderWindow->getCustomAttribute("WINDOW", &pContext->m_pHandle);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
RenderingView::~RenderingView()
{
    std::cout << "RenderingView::~RenderingView()" << std::endl;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
RenderingView::setCamera(Zen::Engine::Camera::I_Camera* _pCamera)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Engine::Rendering::I_View*
RenderingView::createSubView(int _x, int _y, int _width, int _height)
{
    return NULL;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Engine::Rendering::I_Canvas&
RenderingView::getCanvas()
{
    return *m_pCanvas;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
RenderingView::initCanvas()
{
    m_pCanvas = new RenderingCanvas(m_module, this);
    return true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Ogre::RenderWindow&
RenderingView::getRenderWindow()
{
    return *m_pRenderWindow;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Scripting::I_ObjectReference*
RenderingView::getScriptObject()
{
    // TODO Make thread safe?
    if (m_pScriptObject == NULL)
    {
        m_pScriptObject = new ScriptObjectReference_type(
            m_module.getScriptModule(),
            m_module.getScriptModule()->getScriptType(getScriptTypeName()),
            this
        );
    }
    return m_pScriptObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZOgre
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
