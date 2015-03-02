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
// File: MainMenuUI.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements MainMenuUI class.
// ========================================================================= //

#include "MainMenuUI.hpp"

// ========================================================================= //

MainMenuUI::MainMenuUI(void)
{
    m_layers.resize(Layer::NumLayers);
}

// ========================================================================= //

MainMenuUI::~MainMenuUI(void)
{

}

// ========================================================================= //

void MainMenuUI::init(void)
{
    // Load each layer from layout files.
    CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();

    // Root layer.
    m_layers[Layer::Root] = wmgr.loadLayoutFromFile("MainMenu/root.layout");
    m_layers[Layer::Root]->getChild("ButtonCampaign")->subscribeEvent(
        CEGUI::PushButton::EventClicked,
        CEGUI::Event::Subscriber(&MainMenuUI::root_CampaignPressed, this));
    m_layers[Layer::Root]->getChild("ButtonMultiplayer")->subscribeEvent(
        CEGUI::PushButton::EventClicked,
        CEGUI::Event::Subscriber(&MainMenuUI::root_MultiplayerPressed, this));
    m_layers[Layer::Root]->getChild("ButtonExit")->subscribeEvent(
        CEGUI::PushButton::EventClicked,
        CEGUI::Event::Subscriber(&MainMenuUI::root_ExitPressed, this));

    // Campaign layer.
    m_layers[Layer::Campaign] = wmgr.loadLayoutFromFile(
        "MainMenu/campaign.layout");
    m_layers[Layer::Campaign]->getChild("ButtonBack")->subscribeEvent(
        CEGUI::PushButton::EventClicked,
        CEGUI::Event::Subscriber(&MainMenuUI::BackPressed, this));

    // Multiplayer layer.
    m_layers[Layer::Multiplayer] = wmgr.loadLayoutFromFile(
        "MainMenu/multiplayer.layout");
    m_layers[Layer::Multiplayer]->getChild("ButtonHost")->subscribeEvent(
        CEGUI::PushButton::EventClicked,
        CEGUI::Event::Subscriber(&MainMenuUI::multiplayer_HostPressed, this));
    m_layers[Layer::Multiplayer]->getChild("ButtonJoin")->subscribeEvent(
        CEGUI::PushButton::EventClicked,
        CEGUI::Event::Subscriber(&MainMenuUI::multiplayer_JoinPressed, this));
    m_layers[Layer::Multiplayer]->getChild("ButtonBack")->subscribeEvent(
        CEGUI::PushButton::EventClicked,
        CEGUI::Event::Subscriber(&MainMenuUI::BackPressed, this));

    // Host layer.
    m_layers[Layer::Host] = wmgr.loadLayoutFromFile("MainMenu/host.layout");
    m_layers[Layer::Host]->getChild("ButtonHost")->subscribeEvent(
        CEGUI::PushButton::EventClicked,
        CEGUI::Event::Subscriber(&MainMenuUI::host_HostPressed, this));
    m_layers[Layer::Host]->getChild("ButtonBack")->subscribeEvent(
        CEGUI::PushButton::EventClicked,
        CEGUI::Event::Subscriber(&MainMenuUI::BackPressed, this));

    // Join layer.
    m_layers[Layer::Join] = wmgr.loadLayoutFromFile("MainMenu/join.layout");
    m_layers[Layer::Join]->getChild("ButtonConnect")->subscribeEvent(
        CEGUI::PushButton::EventClicked,
        CEGUI::Event::Subscriber(&MainMenuUI::join_ConnectPressed, this));
    m_layers[Layer::Join]->getChild("ButtonBack")->subscribeEvent(
        CEGUI::PushButton::EventClicked,
        CEGUI::Event::Subscriber(&MainMenuUI::BackPressed, this));

    // Options layer.
    m_layers[Layer::Options] = wmgr.loadLayoutFromFile("MainMenu/options.layout");
    m_layers[Layer::Options]->getChild("ButtonBack")->subscribeEvent(
        CEGUI::PushButton::EventClicked,
        CEGUI::Event::Subscriber(&MainMenuUI::BackPressed, this));


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

void MainMenuUI::destroy(void)
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

bool MainMenuUI::update(void)
{
    return UI::update();
}

// ========================================================================= //

// Event handlers:

// ========================================================================= //

bool MainMenuUI::root_CampaignPressed(const CEGUI::EventArgs& args)
{
    printf("Campaign pressed!");

    return true;
}

// ========================================================================= //

bool MainMenuUI::root_MultiplayerPressed(const CEGUI::EventArgs& args)
{
    this->pushLayer(Layer::Multiplayer);
    return true;
}

// ========================================================================= //

bool MainMenuUI::root_ExitPressed(const CEGUI::EventArgs& args)
{
    this->pushEvent(UIEvent(Event::Exit));
    return true;
}

// ========================================================================= //

bool MainMenuUI::multiplayer_HostPressed(const CEGUI::EventArgs& args)
{
    this->pushLayer(Layer::Host);
    return true;
}

// ========================================================================= //

bool MainMenuUI::multiplayer_JoinPressed(const CEGUI::EventArgs& args)
{
    this->pushLayer(Layer::Join);
    return true;
}

// ========================================================================= //

bool MainMenuUI::host_HostPressed(const CEGUI::EventArgs& args)
{
    UIEvent e(Event::HostGame);

    // Get username and port.
    e.s1 = m_layers[Layer::Host]->getChild("EditUsername")->getText().c_str();
    e.field.x = std::stoi(
        m_layers[Layer::Host]->getChild("EditPort")->getText().c_str());
    
    this->pushEvent(e);
    return true;
}

// ========================================================================= //

bool MainMenuUI::join_ConnectPressed(const CEGUI::EventArgs& args)
{
    UIEvent e(Event::JoinGame);

    // Get username, server address, and port.
    e.s1 = m_layers[Layer::Join]->getChild("EditUsername")->getText().c_str();
    e.s2 = m_layers[Layer::Join]->getChild("EditServer")->getText().c_str();
    e.field.x = std::stoi(
        m_layers[Layer::Join]->getChild("EditPort")->getText().c_str());

    this->pushEvent(e);
    return true;
}

// ========================================================================= //

bool MainMenuUI::BackPressed(const CEGUI::EventArgs& args)
{
    this->popLayer();

    return true;
}

// ========================================================================= //