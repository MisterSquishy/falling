#include "BlockHandler.h"

BlockHandler::BlockHandler(void) :
	GRID_SIZE(64.0f), 
	GRID_HEIGHT(get_Window().get_height() / (int) GRID_SIZE),
	GRID_WIDTH((get_Window().get_width() / (int) GRID_SIZE) + 1)
{
	for(int i = 0; i < GRID_WIDTH; i++)
	{
		blocks.push_back(vector<Block*>());
	}

	GRID_WIDTH = 7;

	d = new Dude(Point2f(512.0f, GRID_HEIGHT-32.0f), Vector2f(32.0f, 32.0f), Global::pi*2.0f);

	for(int i = 0; i < GRID_WIDTH; i++)
	{
		blocks.push_back(vector<Block*>());
		Block* b = new Block(Point2f((0.0f+GRID_SIZE)*i, 0.0f), Vector2f(GRID_SIZE, GRID_SIZE), Global::pi*2.0f);
		blocks[i].push_back(b);
	}
}

BlockHandler::~BlockHandler(void)
{
	for(unsigned int i = 0; i < blocks.size(); i++)
	{
		while(!blocks[i].empty())
		{
			Block* b = blocks[i].back();
			blocks[i].pop_back();
			delete b;
		}
	}
	delete d;
}

void BlockHandler::perform_logic(float time_passed, float time_step, ControlState cs)
{
	static bool d0, d1, d2, d3 = false;
	static bool dd2 = false;
	if (!d0)
	{
		if(!d1 && time_passed >= 10.0f){ d1=true; Block* b = new Block(Point2f((0.0f+GRID_SIZE)*2, 0.0f), Vector2f(GRID_SIZE, GRID_SIZE), Global::pi*2.0f); blocks[2].push_back(b); }
		if(!d2 && time_passed >= 20.0f){ d2=true; Block* b = new Block(Point2f((0.0f+GRID_SIZE)*2, 0.0f), Vector2f(GRID_SIZE, GRID_SIZE), Global::pi*2.0f); blocks[2].push_back(b); }
		if(!d3 && time_passed >= 30.0f){ d0=true; d3=true; Block* b = new Block(Point2f((0.0f+GRID_SIZE)*6, 0.0f), Vector2f(GRID_SIZE, GRID_SIZE), Global::pi*2.0f); blocks[6].push_back(b); }
	}

	if (time_passed >= 40.0f && !dd2 && d0)
	{
		dd2 = true;
		blocks[2].erase(blocks[2].begin());
	}

	for(int i = 0; i < (int) blocks.size(); i++)
	{
		for(int j = 0; (int) j < blocks[i].size(); j++)
		{
			Block* block = blocks[i][j];
			vector<GameObject*> potentialCollissions;
			if(i >= 0 && i < blocks.size() && j+1 >= 0 && j+1 < blocks[i].size()) // Above
				potentialCollissions.push_back(blocks[i][j+1]);
			if(i >= 0 && i < blocks.size() && j-1 >=0 && j-1 < blocks[i].size()) // Below
				potentialCollissions.push_back(blocks[i][j-1]);
			if(i-1 >= 0 && i-1 < blocks.size() && j >=0 && j < blocks[i-1].size()) // Left
				potentialCollissions.push_back(blocks[i-1][j]);
			if(i+1 >= 0 && i+1 < blocks.size() && j >=0 && j < blocks[i+1].size()) // Right
				potentialCollissions.push_back(blocks[i+1][j]);
			potentialCollissions.push_back(d);
			block->colliding(potentialCollissions, true);
			block->perform_logic(time_passed, time_step);
		}
	}

	for(unsigned int i = 0; i < blocks.size(); i++)
	{
		d->colliding(blocks[i], i==0);
	}
	d->perform_logic(time_passed, time_step, cs);
}

void BlockHandler::render()
{
	for(unsigned int i = 0; i < blocks.size(); i++)
	{
		for(unsigned int j = 0; j < blocks[i].size(); j++)
			blocks[i][j]->render();
	}

	d->render();
}


