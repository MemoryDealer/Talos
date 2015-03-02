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
// File: UI.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements UI class.
// ========================================================================= //

#include "UI.hpp"

// ========================================================================= //

UI::UI(void) :
m_layers(),
m_layerStack(),
m_currentLayer(nullptr),
m_events()
{

}

// ========================================================================= //

UI::~UI(void)
{

}

// ========================================================================= //

void UI::destroy(void)
{
    while (m_layerStack.empty() == false){
        CEGUI::Window* current = m_layers[m_layerStack.top()];
        current->setVisible(false);
        current->deactivate();

        m_layerStack.pop();
    }
}

// ========================================================================= //

bool UI::update(void)
{
    CEGUI::System::getSingleton().injectTimePulse(1.f / 16.f);

    return (m_events.empty() == false);
}

// ========================================================================= //

void UI::pushLayer(const unsigned int n)
{
    Assert(n < m_layers.size(), "Invalid layer");

    // Hide current active layer.
    if (m_layerStack.empty() == false){
        CEGUI::Window* current = m_layers[m_layerStack.top()];
        current->setVisible(false);
        current->deactivate();
    }

    // Activate new layer.
    m_layerStack.push(n);
    m_layers[n]->setVisible(true);
    m_layers[n]->activate();
    m_currentLayer = m_layers[n];
}

// ========================================================================= //

void UI::popLayer(void)
{
    // Hide and deactivate active layer and pop index.
    CEGUI::Window* current = m_layers[m_layerStack.top()];
    current->setVisible(false);
    current->deactivate();

    m_layerStack.pop();

    // Get next layer and activate it.
    if (m_layerStack.empty() == false){
        m_currentLayer = m_layers[m_layerStack.top()];
        m_currentLayer->setVisible(true);
        m_currentLayer->activate();
    }
    else{
        m_currentLayer = nullptr;
    }
}

// ========================================================================= //

void UI::setVisible(const bool visible)
{
    m_currentLayer->setVisible(visible);

    if (visible == true){
        m_currentLayer->activate();
    }
    else{
        m_currentLayer->deactivate();
    }
}

// ========================================================================= //

void UI::pushEvent(const UIEvent e)
{
    m_events.push(e);
}

// ========================================================================= //

const UIEvent UI::getNextEvent(void)
{
    if (m_events.empty() == true){
        return UIEvent(UIEvent::None);
    }

    UIEvent e = m_events.front();
    m_events.pop();
    return e;
}

// ========================================================================= //

CEGUI::Window* UI::getWindow(const int layer, const std::string& name)
{
    Assert(layer <= m_layers.size(), "Invalid layer");

    return m_layers[layer]->getChild(CEGUI::String(name));
}

// ========================================================================= //

// Helper functions:

// ========================================================================= //

void UI::insertListboxItem(const std::string& window, const std::string& text)
{
    CEGUI::Listbox* listbox = static_cast<CEGUI::Listbox*>(
        m_currentLayer->getChild(window));

    CEGUI::ListboxTextItem* item = new CEGUI::ListboxTextItem(text);
    listbox->addItem(item);
}

// ========================================================================= //

void UI::removeListboxItem(const std::string& window,
                           const std::string& text)
{
    CEGUI::Listbox* listbox = static_cast<CEGUI::Listbox*>(
        m_currentLayer->getChild(window));

    CEGUI::ListboxItem* item = listbox->findItemWithText(text, nullptr);
    if (item != nullptr){
        listbox->removeItem(item);
    }
}

// ========================================================================= //