//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Community Framework
//
// Copyright (C) 2001 - 2010 Tony Richards
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
#ifndef ZEN_COMMUNITY_CHATCOMMON_CHANNEL_VIEW_HPP_INCLUDED
#define ZEN_COMMUNITY_CHATCOMMON_CHANNEL_VIEW_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
namespace Common {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_Channel;

/// Channel View.
/// Implement this interface and use I_ChannelModel::subscribe()
/// to be notified of updates to the Channel Model.
class CHATCOMMON_DLL_LINK I_ChannelView
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_ChannelView interface
    /// @{
public:
    /// Notification of when a channel is added to the model.
    virtual void onAddChannel(I_Channel& _channel) = 0;

    /// Notification of when a channel is removed from the model.
    virtual void onRemoveChannel(I_Channel& _channel) = 0;

    /// Notification of when a channel is updated in the model.
    virtual void onUpdateChannel(I_Channel& _channel) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ChannelView();
    virtual ~I_ChannelView();
    /// @}

};  // interface I_ChannelView

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Common
}   // namespace Community
namespace Memory {
    /// I_ChannelView is managed by factory
	template<>
	struct is_managed_by_factory<Community::Common::I_ChannelView> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_COMMUNITY_CHATCOMMON_CHANNEL_VIEW_HPP_INCLUDED
