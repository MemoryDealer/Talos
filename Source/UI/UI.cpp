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
m_events()
{

}

// ========================================================================= //

UI::~UI(void)
{

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
        current = m_layers[m_layerStack.top()];
        current->setVisible(true);
        current->activate();
    }
    else{

    }
}

// ========================================================================= //

void UI::setVisible(const bool visible)
{
    CEGUI::Window* currentLayer = m_layers[m_layerStack.top()];
    currentLayer->setVisible(visible);

    if (visible == true){
        currentLayer->activate();
    }
    else{
        currentLayer->deactivate();
    }
}

// ========================================================================= //

int UI::getNextEvent(void)
{
    if (m_events.empty() == true){
        return 0;
    }

    int e = m_events.front();
    m_events.pop();
    return e;
}

// ========================================================================= //