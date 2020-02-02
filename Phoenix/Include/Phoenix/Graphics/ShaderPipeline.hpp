// Copyright 2019 Genten Studios
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

/**
 * @file ShaderPipeline.hpp
 * @brief The rendering pipeline paired with shaders.
 *
 * @copyright Copyright (c) 2019-20 Genten Studios
 */

#pragma once

#include <Phoenix/Math/Math.hpp>

#include <glad/glad.h>

#include <string>
#include <vector>

namespace phx
{
	namespace gfx
	{
		/**
		 * @brief The layout for shader vertex locations.
		 *
		 * A shader has a layout in which vertices and data in general is sent
		 * to it in, this makes sure that all renderer objects can specify a
		 * their own layout to allow better compatability.
		 *
		 * @paragraph Usage
		 * @code
		 * std::vector<ShaderLayout> attributes;
		 * attributes.emplace_back("a_Vertex", 0);
		 * attributes.emplace_back("a_UV", 1);
		 *
		 * ShaderPipeline pipeline;
		 * pipeline.prepare("myvert.shader", "myfrag.shader", attributes);
		 * @endcode
		 */
		struct ShaderLayout
		{
			ShaderLayout(const std::string& attribName, int desiredIndex)
			    : attribName(attribName), desiredIndex(desiredIndex)
			{
			}

			std::string attribName;
			int         desiredIndex = -1;
		};

		/**
		 * @brief The Pipeline through which basic rendering occurs.
		 *
		 * This class provides the ability to use shaders while rendering - and
		 * you need shaders to render anything more than a triangle so this is a
		 * necessity. Shaders must be written with ambiguous location parameters
		 * on inputs but will be set through this class, to allow for maximum
		 * compatibility.
		 *
		 * The activate method must be called before rendering the system
		 * associated with this pipeline, otherwise you may render without or
		 * with the wrong shaders.
		 *
		 * The set* methods are to set uniform data, for example the MVP
		 * matrices from the camera.
		 *
		 * @paragraph Usage
		 * @code
		 * // using the chunk renderer as an example.
		 * ShaderPipeline pipeline;
		 * pipeline.prepare("myvert.shader", "myfrag.shader", ChunkRenderer::getRequiredShaderLayout());
		 *
		 * mainGameLoop()
		 * {
		 *     // individually activate since we have another shader that could be active.
		 *     pipeline.activate();
		 *     pipeline.setMatrix(myViewMatrix);
		 *     pipeline.setMatrix(myProjectionMatrix);
		 *     world.render();
		 *
		 *     // lets say we made this for a ui element
		 *     pipeline2.activate();
		 *     ui.render();
		 * }
		 * @endcode
		 */
		class ShaderPipeline
		{
		public:
			ShaderPipeline()  = default;
			~ShaderPipeline() = default;

			/**
			 * @brief Prepares a pipeline with the provided shaders and layout.
			 * @param vertShaderPath Path to vertex shader.
			 * @param fragShaderPath Path to fragment/pixel shader.
			 * @param layout The required layout for the shaders to adhere to.
			 */
			void prepare(std::string vertShaderPath, std::string fragShaderPath,
			             std::vector<ShaderLayout> layout);

			/**
			 * @brief Activates the pipeline, prepared shaders are activated.
			 *
			 * This must be called before calling the render method for
			 * associated objects, otherwise you may render with the wrong
			 * shader, or just without an active shader. If you don't plan on
			 * changing shaders at any point, this can be called once beforehand
			 * and never again.
			 */
			void activate();

			/**
			 * @brief Sets a uniform location to a 2 component vector.
			 * @param location The location being set.
			 * @param value The value to set provided location.
			 */
			void setVector2(std::string location, math::vec2 value);

			/**
			 * @brief Sets a uniform location to a 3 component vector.
			 * @param location The location being set.
			 * @param value The value to set provided location.
			 */
			void setVector3(std::string location, math::vec3 value);

			/**
			 * @brief Sets a uniform location to a 4x4 matrix.
			 * @param location The location being set.
			 * @param value The value to set provided location.
			 */
			void setMatrix(std::string location, math::mat4 value);

			/**
			 * @brief Queries the location set for a specific attribute.
			 * @param attr The attribute location to query
			 * @return The set location for the attribute.
			 */
			int queryLayoutOfAttribute(std::string attr);

		private:
			unsigned int m_program;
		};
	} // namespace gfx
} // namespace phx
