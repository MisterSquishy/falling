#ifndef BLOCKHANDLER_H
#define BLOCKHANDLER_H

#include "Block.h"
#include "Dude.h"
#include <zenilib.h>
#include <vector>

enum BlockHandlerState {BH_NORMAL, BH_STARTUP, BH_SPLASHDOWN, BH_CLEANUP};

class BlockHandler
{
public:
	BlockHandler(void);
	
	~BlockHandler(void);
	void render();
	void perform_logic(float time_passed, float time_step, ControlState cs);

	float GRID_SIZE;
	int GRID_HEIGHT;
	int GRID_WIDTH;

private:
    BlockHandlerState state;
	Chronometer<Time> m_chrono;
	vector<vector<Block*> > blocks;
	Dude* d;
	float timeToWait;
    float lastBlockSpawn;
    
    unsigned int cleanupIndex;
    float lastCleanup;
};

#endif