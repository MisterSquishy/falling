#ifndef BLOCKHANDLER_H
#define BLOCKHANDLER_H

#include "Block.h"
#include "Dude.h"
#include "Powerup.h"
#include <vector>
#include <fstream>

class Score
{
public:

	Score(float time_, int blocksSpawned_, int blocksDestroyed_, bool cur_ = false):time(time_), numSpawned(blocksSpawned_), numDestroyed(blocksDestroyed_), curScore(cur_)
	{
	}

	Score(bool cur_ = false):time(0), numSpawned(0), numDestroyed(0), curScore(cur_){}

	Zeni::String getTimeAsStr()
	 {
		Zeni::String time_str = ftoa(time);
		time_str = time_str.substr(0, time_str.find('.'));
		return time_str;
	 }

	void destroyed()
	{
		numDestroyed++;
	}

	void spawned()
	{
		numSpawned++;
	}

	void setTime(float time_)
	{
		time = time_;
	}

	bool isCur()
	{
		return curScore;
	}

	int getNumSpawned()
	{
		return numSpawned;
	}

	
	void setCur(bool cur_)
	{
		curScore = cur_;
	}

	void reset()
	{
		time = 0;
		numSpawned = 0;
		numDestroyed = 0;
	}

	const float getTime()
	{
		return time;
	}

	bool operator< (const Score &other) const;

	char* getName(bool multi_line)
	{
		char* name = (char*) malloc(sizeof(char) * 512);
		if(multi_line) sprintf(name, "Time Survived:            %s\nBlocks Spawned:        %d\nBlocks Destroyed:   %d", getTimeAsStr().c_str(), numSpawned, numDestroyed);
		else sprintf(name, "Time: %s   Spawned: %d   Destroyed: %d", getTimeAsStr().c_str(), numSpawned, numDestroyed);
		return name;
	}

private:
	float time;
	bool curScore;
	int numSpawned;
	int numDestroyed;
	friend istream& operator>>(istream &in, Score &s);
	friend ostream& operator<<(ostream &out, Score &s);
};

enum BlockHandlerState {BH_NORMAL, BH_CLEANUP, BH_POST_DEATH, BH_RESPAWNING_DUDE};

class BlockHandler
{
public:
	BlockHandler(void);
	
	~BlockHandler(void);
	void render();
	void perform_logic(float current_time, float time_step, ControlState cs);

	BlockHandlerState getState();

private:
    BlockHandlerState state;
	vector<vector<Block*> > blocks;
	Dude* d;
	float timeToWait;
    float lastBlockSpawn;
    
    int cleanupIndex;
    float lastCleanup;

	float startedRespawn;
	int min_index;
    
    void emptyBlocks();

	void readScores();
	void writeScores();
	Score rs;
	vector<Score> scoresThatIRead;
    
    vector<Powerup*> powerups;

	void updateBlocks(float current_time, float time_step);
    void updatePowerups(float current_time, float time_step);

	int spawnPerLoop;
	void init();
};



#endif