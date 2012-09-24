#ifndef BLOCKHANDLER_H
#define BLOCKHANDLER_H

#include "Block.h"
#include "Dude.h"
#include <zenilib.h>
#include <vector>
#include <deque>

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
	Chronometer<Time> m_chrono;
	vector<vector<Block*> > blocks;
	Dude* d;
};

#endif