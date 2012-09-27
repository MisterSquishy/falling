#include "BlockHandler.h"

BlockHandler::BlockHandler(void) :
GRID_SIZE(64.0f),
GRID_HEIGHT(get_Window().get_height() / (int) GRID_SIZE),
GRID_WIDTH((get_Window().get_width() / (int) GRID_SIZE) + 1)
{
    m_chrono.start();
    timeToWait = 5.0f;
    lastCleanup = 0.0f;
    
    state = BH_NORMAL;
    
	for(int i = 0; i < GRID_WIDTH; i++)
    {
		blocks.push_back(vector<Block*>());
        //if(i == 0) blocks[i].push_back(new Block(Point2f(64.0f * i, 64.0f), Vector2f(64.0f, 64.0f), Global::pi*2.0f));
    }
    
    int blkidx = 3;
    blocks[blkidx].push_back(new Block(Point2f(64.0f * blkidx, 64.0f), Vector2f(64.0f, 64.0f), Global::pi*2.0f, 10.0f));
    blocks[blkidx].push_back(new Block(Point2f(64.0f * blkidx, 64.0f - (2*64.0f)), Vector2f(64.0f, 64.0f), Global::pi*2.0f, 10.0f));
    blocks[blkidx].push_back(new Block(Point2f(64.0f * blkidx, 64.0f - (3*64.0f)), Vector2f(64.0f, 64.0f), Global::pi*2.0f, 10.0f));
    blocks[blkidx].push_back(new Block(Point2f(64.0f * blkidx, 64.0f - (4*64.0f)), Vector2f(64.0f, 64.0f), Global::pi*2.0f, 10.0f));
    blkidx = 1;
    blocks[blkidx].push_back(new Block(Point2f(64.0f * blkidx, 64.0f), Vector2f(64.0f, 64.0f), Global::pi*2.0f, 10.0f));
    blocks[blkidx].push_back(new Block(Point2f(64.0f * blkidx, 64.0f - (2*64.0f)), Vector2f(64.0f, 64.0f), Global::pi*2.0f, 10.0f));
    blocks[blkidx].push_back(new Block(Point2f(64.0f * blkidx, 64.0f - (3*64.0f)), Vector2f(64.0f, 64.0f), Global::pi*2.0f, 10.0f));
    blocks[blkidx].push_back(new Block(Point2f(64.0f * blkidx, 64.0f - (4*64.0f)), Vector2f(64.0f, 64.0f), Global::pi*2.0f, 10.0f));
    
    
    
    
	d = new Dude(Point2f((get_Window().get_width() / 2) - 32.0f, 0.0f), Vector2f(64.0f, 64.0f), Global::pi*2.0f);
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
    float curSecs = m_chrono.seconds();
    switch(state)
    {
        case BH_CLEANUP:
            if(lastCleanup == 0.0f)
            {
                lastCleanup = m_chrono.seconds();
                cleanupIndex = 0;
            }
            
            if(curSecs-lastCleanup >= 0.156f)
            {
                blocks[cleanupIndex].erase(blocks[cleanupIndex].begin());
                lastCleanup = m_chrono.seconds();
                cleanupIndex++;
                
                if(cleanupIndex >= blocks.size())
                {
                    state = BH_NORMAL;
                    lastCleanup = 0.0f;
                }
            }
        case BH_NORMAL:
            if(curSecs-lastBlockSpawn >= timeToWait && curSecs != lastBlockSpawn)
            {
                int speed = (rand() % 10) + 1;
                
                int idx = rand() % GRID_WIDTH;
                while(blocks[idx].size() >= GRID_HEIGHT - 4) idx = rand() % GRID_WIDTH;
                    
                blocks[idx].push_back(new Block(Point2f(64.0f * idx, 0.0f), Vector2f(64.0f, 64.0f), Global::pi*2.0f, (float)speed));
                
                timeToWait = (rand() % 5) + 1;
                lastBlockSpawn = curSecs;
            }
            
            if(state != BH_CLEANUP)
            {
                bool itsTime = true;
                for(int i = 0; i < blocks.size(); i++)
                {
                    if(blocks[i].size() < 4)
                    {
                        itsTime = false;
                        break;
                    }
                }
                if(itsTime)
                    state = BH_CLEANUP;
            }
            break;
    }
    
    
    // Loop through each block, and tell it to consider its surrounding blocks for collision
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
			//potentialCollissions.push_back(d);
			block->colliding(potentialCollissions, true);
			block->perform_logic(time_passed, time_step);
		}
	}
    
    // Tell the dude to consider all of the blocks for collision
	for(unsigned int i = 0; i < blocks.size(); i++)
	{
		d->colliding(blocks[i], i==0);
	}
	d->perform_logic(m_chrono.seconds(), time_passed, time_step, cs);
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


