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

#include <Common/Voxels/BlockRegistry.hpp>

#include <algorithm>
#include <cstring>

using namespace phx::voxels;

BlockRegistry::BlockRegistry()
{
	BlockType unknownBlock;
	unknownBlock.displayName = "Unknown Block";
	unknownBlock.id          = "core.unknown";
	unknownBlock.category    = BlockCategory::SOLID;
	unknownBlock.setAllTextures("Assets/unknown.png");
	registerBlock(unknownBlock);

	BlockType outOfBoundsBlock;
	outOfBoundsBlock.displayName = "Out Of Bounds";
	outOfBoundsBlock.id          = "core.out_of_bounds";
	outOfBoundsBlock.category    = BlockCategory::AIR;
	registerBlock(outOfBoundsBlock);
}

void BlockRegistry::registerAPI(phx::cms::ModManager* manager)
{
	manager->registerFunction(
	    "voxel.block.register", [manager](sol::table luaBlock) {
		    BlockType block;
		    {
			    block.displayName = luaBlock["name"];
			    block.id          = luaBlock["id"];

			    std::string category = luaBlock["category"];
			    if (category == "Air")
			    {
				    block.category = BlockCategory::AIR;
			    }
			    else if (category == "Liquid")
			    {
				    block.category = BlockCategory::LIQUID;
			    }
			    else
			    {
				    block.category = BlockCategory::SOLID;
			    }

			    if (luaBlock["onPlace"].valid())
			    {
				    block.onPlace = luaBlock["onPlace"];
			    }

			    if (luaBlock["onBreak"].valid())
			    {
				    block.onBreak = luaBlock["onBreak"];
			    }

			    if (luaBlock["onInteract"].valid())
			    {
				    block.onInteract = luaBlock["onInteract"];
			    }

			    if (luaBlock["textures"].valid())
			    {
				    std::array<std::string, 6> textures;
				    for (int i = 0; i < 6; i++)
				    {
					    std::string texture = luaBlock["textures"][i + 1];
					    if (texture.empty())
					    {
						    // If a texture is not supplied, we use the first
						    // texture in its place
						    texture = luaBlock["textures"][1];
					    }
					    textures[i] = manager->getCurrentModPath() + texture;
				    }

			    	block.textures = textures;
			    }
		    }
		    BlockRegistry::get()->registerBlock(block);
	    });
}

void BlockRegistry::registerBlock(BlockType blockInfo)
{
	if (std::find(m_blocks.begin(), m_blocks.end(), blockInfo) ==
	    m_blocks.end())
	{
		blockInfo.m_registryID = m_blocks.size();
		m_blocks.push_back(blockInfo);

		if (blockInfo.category != BlockCategory::AIR)
		{
			for (const std::string& tex : blockInfo.textures)
			{
				m_textures.addTexture(tex);
			}
		}
	}
}

BlockType* BlockRegistry::getFromID(const std::string& id)
{
	auto it = std::find_if(
	    m_blocks.begin(), m_blocks.end(), [id](const BlockType& block) {
		    return std::strcmp(block.id.c_str(), id.c_str()) == 0;
	    });

	return it == m_blocks.end() ? getFromRegistryID(0)
	                            : &(*it); // 0 is always unknown.
}

BlockType* BlockRegistry::getFromRegistryID(std::size_t registryID)
{
	if (registryID >= m_blocks.size())
	{
		return &m_blocks[0];
	}

	return &m_blocks[registryID];
}

TextureRegistry* BlockRegistry::getTextures() { return &m_textures; }
