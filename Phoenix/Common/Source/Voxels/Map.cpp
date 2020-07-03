// Copyright 2019-20 Genten Studios
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors
// may be used to endorse or promote products derived from this software without
// specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include <Common/Voxels/Map.hpp>
#include <fstream>
#include <iostream>
#include <utility>

using namespace phx::voxels;

Map::Map(const std::string& save, const std::string& name)
    : m_save(save), m_mapName(name)
{
}

Chunk Map::getChunk(const phx::math::vec3& pos)
{
	if (m_chunks.find(pos) != m_chunks.end())
	{
		return m_chunks.at(pos);
	}
	else
	{
		std::ifstream saveFile;
		std::string   position = "." + std::to_string(int(pos.x)) + "_" +
		                       std::to_string(int(pos.y)) + "_" +
		                       std::to_string(int(pos.z));
		saveFile.open("Saves/" + m_save + "/" + m_mapName + position + ".save");

		if (saveFile)
		{
			std::string saveString;
			std::getline(saveFile, saveString);
			auto win = m_chunks.emplace(pos, Chunk(pos, saveString));
		}
		else
		{
			auto win = m_chunks.emplace(pos, Chunk(pos));
			m_chunks.at(pos).autoTestFill();
			save(pos);
		}
		return m_chunks.at(pos);
	}
}

void Map::setBlockAt(phx::math::vec3 position, BlockType* block)
{
	int posX = static_cast<int>(position.x / Chunk::CHUNK_WIDTH);
	int posY = static_cast<int>(position.y / Chunk::CHUNK_HEIGHT);
	int posZ = static_cast<int>(position.z / Chunk::CHUNK_DEPTH);

	position.x =
	    static_cast<float>(static_cast<int>(position.x) % Chunk::CHUNK_WIDTH);
	if (position.x < 0)
	{
		posX -= 1;
		position.x += Chunk::CHUNK_WIDTH;
	}

	position.y =
	    static_cast<float>(static_cast<int>(position.y) % Chunk::CHUNK_HEIGHT);
	if (position.y < 0)
	{
		posY -= 1;
		position.y += Chunk::CHUNK_HEIGHT;
	}

	position.z =
	    static_cast<float>(static_cast<int>(position.z) % Chunk::CHUNK_DEPTH);
	if (position.z < 0)
	{
		posZ -= 1;
		position.z += Chunk::CHUNK_DEPTH;
	}

	const math::vec3 chunkPosition =
	    math::vec3(static_cast<float>(posX * Chunk::CHUNK_WIDTH),
	               static_cast<float>(posY * Chunk::CHUNK_HEIGHT),
	               static_cast<float>(posZ * Chunk::CHUNK_DEPTH));

	m_chunks.at(chunkPosition)
	    .setBlockAt(
	        {
	            // "INLINE" VECTOR 3 DECLARATION
	            position.x, // x position IN the chunk, not overall
	            position.y, // y position IN the chunk, not overall
	            position.z  // z position IN the chunk, not overall
	        },
	        block);

	save(chunkPosition);
}

void Map::save(const phx::math::vec3& pos)
{
	std::ofstream saveFile;
	std::string   position = "." + std::to_string(int(pos.x)) + "_" +
	                       std::to_string(int(pos.y)) + "_" +
	                       std::to_string(int(pos.z));
	saveFile.open("Saves/" + m_save + "/" + m_mapName + position + ".save");
	saveFile << m_chunks.at(pos).save();

	saveFile.close();
}

