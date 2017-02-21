#include "Sound.h"
#include "Report.h"

void Sound::LoadWavFile(std::string directory)
{
	sound = Mix_LoadWAV(directory.c_str());
	if (sound==NULL)
	{
		ReportError(directory,"Loading failed");
	}
}

int Sound::Play(int times)
{
	return Mix_PlayChannel( -1, sound, times );
}


void Music::LoadMusicFile(std::string directory)
{
	music = Mix_LoadMUS(directory.c_str());
	if (music==NULL)
	{
		ReportError(directory,"Loading failed");
	}
}

void Music::Play()
{
	if (Mix_PlayingMusic()==0)  //if music is not playing
	{
		Mix_PlayMusic( music, -1 );
	}
	else if (Mix_PlayingMusic()==1) //if music is paused
	{
		Mix_ResumeMusic();
	}
}

void Music::Pause()
{
	Mix_PauseMusic();
}

void Music::Stop()
{
	Mix_HaltMusic();
}
