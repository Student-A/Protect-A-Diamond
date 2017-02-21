//==========Sound.h===========
#ifndef SOUND_H_INCLUDED
#define SOUND_H_INCLUDED

//=================================
// included dependencies
#include <SDL_mixer.h>
#include <string>
//=================================

class Sound
{
		
	public:
		Mix_Chunk *sound = NULL;
		void LoadWavFile(std::string directory);
		int Play(int times);
		void Stop();
};

class Music
{
	private:
		enum State
		{
			NOMUSIC=0,
			PUASED=1
		}state;
		Mix_Music *music;
	public:
		void LoadMusicFile(std::string directory);
		void Play();
		void Pause();
		void Stop();
};

#endif  // SOUND_H_INCLUDED
