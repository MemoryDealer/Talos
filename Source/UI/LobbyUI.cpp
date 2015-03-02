// ========================================================================= //
// Talos - A 3D game engine with network multiplayer.
// Copyright(C) 2015 Jordan Sparks <unixunited@live.com>
//
// This program is free software; you can redistribute it and / or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or(at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
// ========================================================================= //
// File: LobbyUI.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements LobbyUI class.
// ========================================================================= //

#include "LobbyUI.hpp"

// ========================================================================= //

LobbyUI::LobbyUI(void)
{
    m_layers.resize(Layer::NumLayers);
}

// ========================================================================= //

LobbyUI::~LobbyUI(void)
{

}

// ========================================================================= //

void LobbyUI::init(void)
{
    CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();

    m_layers[Layer::Root] = wmgr.loadLayoutFromFile("Lobby/root.layout");
    // Disable manual editing of multiline edit boxes.
    m_layers[Layer::Root]->getChild("PlayerList")->disable();
    m_layers[Layer::Root]->getChild("Chat")->disable();
    // Subscribe event handlers.
    m_layers[Layer::Root]->getChild("EditSend")->subscribeEvent(
        CEGUI::Editbox::EventTextAccepted,
        CEGUI::Event::Subscriber(&LobbyUI::root_SendPressed, this));
    m_layers[Layer::Root]->getChild("ButtonSend")->subscribeEvent(
        CEGUI::PushButton::EventClicked,
        CEGUI::Event::Subscriber(&LobbyUI::root_SendPressed, this));

    // Add each layer to root window.
    for (int i = 0; i < Layer::NumLayers; ++i){
        CEGUI::System::getSingleton().getDefaultGUIContext().
            getRootWindow()->addChild(m_layers[i]);
        m_layers[i]->setVisible(false);
    }

    // Push first layer onto stack.
    this->pushLayer(Layer::Root);
}

// ========================================================================= //

void LobbyUI::destroy(void)
{
    UI::destroy();

    CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
    for (int i = 0; i < Layer::NumLayers; ++i){
        CEGUI::System::getSingleton().getDefaultGUIContext().
            getRootWindow()->removeChild(m_layers[i]);
        wmgr.destroyWindow(m_layers[i]);
    }
}

// ========================================================================= //

bool LobbyUI::update(void)
{
    return UI::update();
}

// ========================================================================= //

bool LobbyUI::root_SendPressed(const CEGUI::EventArgs& args)
{
    // Get message.
    CEGUI::String str = m_layers[Layer::Root]->getChild("EditSend")->getText();
    if (str.empty() == true){
        return false;
    }

    // Add text to chat box.
    CEGUI::MultiLineEditbox* chat = static_cast<CEGUI::MultiLineEditbox*>(
        m_layers[Layer::Root]->getChild("Chat"));
    chat->appendText(str + "\n");
    // Scroll to bottom of chat box.
    chat->getVertScrollbar()->setScrollPosition(999999.f);

    // Clear edit box.
    m_layers[Layer::Root]->getChild("EditSend")->setText("");

    // Notify LobbyState.
    UIEvent e(Event::Chat);
    e.s1 = str.c_str();
    this->pushEvent(e);

    return true;
}

// ========================================================================= //