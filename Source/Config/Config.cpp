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
// File: Config.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements Config class.
// ========================================================================= //

#include "Config.hpp"

// ========================================================================= //

namespace Talos
{;

// ========================================================================= //

Config::Config(void) :
m_file(),
m_buffer(),
m_loaded(false)
{

}

// ========================================================================= //

Config::Config(const std::string& file) :
m_file(),
m_buffer(),
m_loaded(false)
{
    this->load(file);
}

// ========================================================================= //

Config::~Config(void)
{

}

// ========================================================================= //

bool Config::load(const std::string& file)
{
    if (m_loaded){
        m_file.close();
        m_loaded = false;
    }

    m_file.open(file);
    if (m_file.is_open()){
        m_loaded = true;
    }
    
    return m_loaded;
}

// ========================================================================= //

std::string& Config::parseValue(const std::string& section,
                                const std::string& key)
{
    if (!m_loaded){
        m_buffer.clear();
        return m_buffer;
    }

    // Always start scanning from the first line.
    m_file.clear();
    m_file.seekg(0, m_file.beg);

    while (!m_file.eof()){
        std::getline(m_file, m_buffer);
        // Find the right section first.
        if (m_buffer[0] == '[' && m_buffer[m_buffer.size() - 1] == ']'){
            std::string compare(section);
            if (m_buffer.compare(1, m_buffer.size() - 2, compare) == 0){
                // Section found, now find the value.
                std::getline(m_file, m_buffer);
                // @TODO: There must always be a \n at the end of a config
                // file with !m_file.eof().
                while (m_buffer[0] != '[' && !m_file.eof()){ 

                    // Skip comments.
                    if (m_buffer[0] != '#'){
                        // Find location of assignment operator.
                        size_t assign = m_buffer.find_first_of('=');

                        // See if pos 0 to the assignment operator matches
                        // value name.
                        if (m_buffer.compare(0, assign, key) == 0){
                            // Trim string to only contain the value.
                            /*if (quotations){
                                m_buffer = m_buffer.substr(
                                    assign + 2, 
                                    m_buffer.size() - key.size() - 3);
                            }*/
                            //else{
                                m_buffer = m_buffer.substr(
                                    assign + 1, 
                                    m_buffer.size() - key.size() - 1);
                            //}

                            return m_buffer;
                        }
                    }

                    std::getline(m_file, m_buffer);
                }
            }
        }
    }

    // Value not found.
    m_buffer.clear();
    return m_buffer;
}

// ========================================================================= //

const Ogre::Real Config::parseReal(const std::string& section,
                                   const std::string& key)
{
    std::string str = this->parseValue(section, key);
    if (!str.empty()){
        return std::stof(str);
    }
}

// ========================================================================= //

const int Config::parseInt(const std::string& section,
                           const std::string& key)
{
    std::string str = this->parseValue(section, key);
    if (!str.empty()){
        return std::stoi(str);
    }

    return 0;
}

// ========================================================================= //

}

// ========================================================================= //