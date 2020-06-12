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

#include <Client/Audio/Source.hpp>
#include <Client/Audio/Audio.hpp>

#include <AL/al.h>
#include <AL/alext.h>

using namespace phx::audio;

Source::Source()
{
	m_source = { new unsigned int{}, Deleter() };
	
	// creates an OpenAL source, ready for setting a buffer and playing back.
	alGenSources(1, &*m_source);
	alSourcei(*m_source, AL_SOURCE_RELATIVE, false);
}

Source::Source(const AudioData& data)
{
	m_source = {new unsigned int {}, Deleter()};

	// generates an OpenAL source and sets the buffer to be used during
	// playback.
	alGenSources(1, &*m_source);
	alSourcei(*m_source, AL_SOURCE_RELATIVE, false);
	alSourcei(*m_source, AL_BUFFER, data.buffer);
	m_duration = data.duration;
}

void Source::enableSpatial(bool enable)
{
	alSourcei(*m_source, AL_SOURCE_SPATIALIZE_SOFT,
	          enable ? AL_TRUE : AL_FALSE);
}

void Source::enableLoop(bool enable)
{
	alSourcei(*m_source, AL_LOOPING, enable ? AL_TRUE : AL_FALSE);
}

void Source::setPos(const phx::math::vec3& position)
{
	alSourcefv(*m_source, AL_POSITION, &position.x);
}

void Source::setDirection(const phx::math::vec3& direction)
{
	alSourcefv(*m_source, AL_DIRECTION, &direction.x);
}

void Source::setVelocity(const phx::math::vec3& velocity)
{
	alSourcefv(*m_source, AL_VELOCITY, &velocity.x);
}

void Source::setGain(float gain)
{
	alSourcef(*m_source, AL_GAIN, gain);
}

void Source::setPitch(float pitch)
{
	alSourcef(*m_source, AL_PITCH, pitch);
}

Duration Source::getDuration() const { return m_duration; }

Source::State Source::status() const
{
	int state;
	alGetSourcei(*m_source, AL_SOURCE_STATE, &state);

	return static_cast<State>(state);
}

void Source::setAudioData(const AudioData& data)
{
	m_duration = data.duration;
	alSourcei(*m_source, AL_BUFFER, data.buffer);
}

void Source::play() const { alSourcePlay(*m_source); }

void Source::pause() const { alSourcePause(*m_source); }

void Source::stop() const { alSourceStop(*m_source); }
