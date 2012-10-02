#include "BlockHandler.h"

BlockHandler::BlockHandler(void)
{
    d = new Dude(Point2f(0, (get_Window().get_height() / 2) - DUDE_SIZE/2), Vector2f(DUDE_SIZE, DUDE_SIZE), Global::pi*2.0f, GRID_HEIGHT * 1.6f, GRID_WIDTH * 1.1f);
    init();
}

BlockHandler::~BlockHandler(void)
{
	emptyBlocks();
	delete d;
}

void BlockHandler::init()
{
    GRID_HEIGHT = get_Window().get_height() / (int) GRID_SIZE;
	GRID_WIDTH = (get_Window().get_width() / (int) GRID_SIZE) + 1;
    
	timeToWait = INITIAL_TIME_BETWEEN_BLOCKS;
    lastCleanup = UNDEFINED;
    
    d->reset();
    
	lastBlockSpawn = UNDEFINED;
	startedRespawn = UNDEFINED;
	min_index = 0;
	spawnPerLoop = GRID_WIDTH/5;
    
    
	for(int i = 0; i < GRID_WIDTH; i++)
    {
		blocks.push_back(vector<Block*>());
    }
    
    rs.setCur(true);
    
    state = BH_NORMAL;
}

void BlockHandler::emptyBlocks()
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
    
    while(!powerups.empty())
    {
        Powerup* p = powerups.back();
        powerups.pop_back();
        delete p;
    }
}

void BlockHandler::perform_logic(float current_time, float time_step, ControlState cs)
{
	if(d->isDead() && state != BH_POST_DEATH)
	{
		state = BH_POST_DEATH;
		readScores();
		scoresThatIRead.push_back(rs);
		sort(scoresThatIRead.begin(), scoresThatIRead.end());
		writeScores();
	}
    
    switch(state)
    {
        case BH_CLEANUP:
            if(lastCleanup == UNDEFINED)
            {
                lastCleanup = current_time;
                cleanupIndex = -10;
            }
            
            if(current_time-lastCleanup >= TIME_TO_WAIT_BETWEEN_CLEANUP_DELETIONS)
            {
                if(cleanupIndex >= 0 && cleanupIndex < blocks.size())
				{
					if(blocks[cleanupIndex].size() && blocks[cleanupIndex][0]->m_speed_y == 0)
					{
						blocks[cleanupIndex].erase(blocks[cleanupIndex].begin());
						if(cleanupIndex%3==0) play_sound("crumble");
					}
				}
                
                cleanupIndex++;
                lastCleanup = current_time;
            }
			if(cleanupIndex >= 0 && cleanupIndex >= blocks.size())
			{
				state = BH_NORMAL;
				lastCleanup = UNDEFINED;
			}
            
        case BH_NORMAL:
		{
			rs.setTime(current_time);
            
            if(current_time-lastBlockSpawn >= timeToWait && current_time != lastBlockSpawn)
            {
				for(int i = 0; i < spawnPerLoop; i++)
				{
					float speed = (rand() % MAX_Y_SPEED) + 1;
                    
					int idx = rand() % GRID_WIDTH;
					unsigned int numChecked = 0;
					while(blocks[idx].size() >= GRID_HEIGHT - 2 && numChecked++ < blocks.size())
					{
						idx = rand() % GRID_WIDTH;
					}
                    
					if(numChecked <= blocks.size()) blocks[idx].push_back(new Block(Point2f(GRID_SIZE * idx, 0.0f-GRID_SIZE), Vector2f(GRID_SIZE, GRID_SIZE), Global::pi*2.0f, (float)speed));
					rs.spawned();
				}
                lastBlockSpawn = current_time;
				timeToWait = max(timeToWait-0.3f, MIN_TIME_BETWEEN_BLOCKS);
				if(rs.getNumSpawned() % 20 == 0) spawnPerLoop = min(spawnPerLoop+1, GRID_WIDTH/2);
            }
            
			//See if its Barney time!
			int numAtMax = 0;
			for(int i = 0; i < blocks.size(); i++)
			{
			    if(blocks[i].size() >= GRID_HEIGHT - 2)
			        numAtMax++;
			}
			if(numAtMax >= .2*GRID_WIDTH) state = BH_CLEANUP;
            
			updateBlocks(current_time, time_step);
            updatePowerups(current_time, time_step);
            
			// Tell the dude to consider all of the blocks for collision
			for(unsigned int i = 0; i < blocks.size(); i++)
				d->colliding(blocks[i], i==0);
			if(d->perform_logic(current_time, time_step, cs))
			{
				state = BH_RESPAWNING_DUDE;
                while(!powerups.empty())
                {
                    Powerup* p = powerups.back();
                    powerups.pop_back();
                    delete p;
                }
			}
			break;
		}
		case BH_RESPAWNING_DUDE:
			if(startedRespawn == UNDEFINED)
			{
				startedRespawn = current_time;
				for(unsigned int i = 1; i < blocks.size()-1; i++)
				{
					if(blocks[i].size() <= blocks[min_index].size())
						min_index = i;
				}
			}
			else if(current_time - startedRespawn >= 3.0f)
			{
				startedRespawn = UNDEFINED;
				d->spawn(Point2f(GRID_SIZE * min_index, 0));
				min_index = 0;
				state = BH_NORMAL;
				lastBlockSpawn = current_time;
			}
			rs.setTime(current_time);
			updateBlocks(current_time, time_step);
			break;
		case BH_POST_DEATH:
			if(cs.retry) // Reset everything
			{
                emptyBlocks();
                init();
				d->revive();
				rs.reset();
			}
			else if(cs.main_menu)
			{
				get_Game().pop_state();
			}
			break;
    }
}

void BlockHandler::render()
{
	Zeni::Font &fr = get_Fonts()["clock"];
	char* message = (char*) malloc(512 * sizeof(char));\
    
	switch(state)
	{
        case BH_RESPAWNING_DUDE:
        case BH_CLEANUP:
        case BH_NORMAL:
        {
            for(unsigned int i = 0; i < blocks.size(); i++)
            {
                for(unsigned int j = 0; j < blocks[i].size(); j++)
                    blocks[i][j]->render();
            }
            d->render();
            for(unsigned int i = 0; i < powerups.size(); i++)
                powerups[i]->render();
            
            
            char* scoreText = rs.getName(false);
            fr.render_text(scoreText, Point2f(10.0f, 00.0f), get_Colors()["white"], ZENI_LEFT);
            free(scoreText);
            
            if(state == BH_RESPAWNING_DUDE)
            {
                Zeni::Font &fd = get_Fonts()["respawn"];
                fd.render_text("RESPAWNING THE DUDE...", Point2f(get_Window().get_width()/2, (get_Window().get_height()/2)-fd.get_text_height()/2), get_Colors()["white"], ZENI_CENTER);
            }
        }
            break;
        case BH_POST_DEATH:
            
            for(unsigned int i = 0; i < blocks.size(); i++)
            {
                for(unsigned int j = 0; j < blocks[i].size(); j++)
                    blocks[i][j]->render();
            }
            d->render();
            
            char* scoreText = rs.getName(true);
            sprintf(message, "GAME OVER\n\n%s\n\n[enter to retry]\n[backspace to quit]", scoreText);
            fr.render_text(message, Point2f(10.0f, 10.0f), get_Colors()["white"], ZENI_LEFT);
            free(scoreText);
            
            char highheader[512] = "";
            sprintf(&highheader[0], "HIGH SCORES: %d tall, %d wide", GRID_HEIGHT, GRID_WIDTH);
            fr.render_text(highheader, Point2f(get_Window().get_width()-10.0f, 10.0f), get_Colors()["white"], ZENI_RIGHT);
            char * name;
            for(unsigned int i = 0; i < scoresThatIRead.size(); i++)
            {
                Score s = scoresThatIRead[i];
                name = s.getName(false);
                fr.render_text(name, Point2f(get_Window().get_width()-10.0f, 50.0f+(20.0f*i)), get_Colors()[(s.isCur() ? "red" : "white")], ZENI_RIGHT);
                free(name);
            }
            
            break;
	}
	free(message);
}


BlockHandlerState BlockHandler::getState()
{
	return state;
}

void BlockHandler::updateBlocks(float current_time, float time_step)
{
	// Loop through each block, and tell it to consider its surrounding blocks for collision
	for(int i = 0; i < (int) blocks.size(); i++)
	{
		for(int j = 0; (int) j < blocks[i].size(); j++)
		{
			Block* block = blocks[i][j];
            
            if(block->getState() == BL_DESTROYED)
			{
				blocks[i].erase(blocks[i].begin() + j);
				rs.destroyed();
                
                PowerupType pt = block->getPowerup();
                if(pt != NO_POWERUP)
                    powerups.push_back(new Powerup(pt, block->m_position+(block->m_size/4), Point2f(POWERUP_SIZE, POWERUP_SIZE), 0, GRID_HEIGHT*5.0f, DORMANT));
				
                delete block;
				play_sound("crumble");
				continue;
			}
            
			block->colliding(blocks[i], true);
			block->perform_logic(current_time, time_step);
		}
	}
}

void BlockHandler::updatePowerups(float current_time, float time_step)
{
    for(unsigned int i = 0; i < powerups.size(); i++)
    {
        if(powerups[i]->getState() == P_DONE)
        {
            Powerup* p = powerups[i];
            powerups.erase(powerups.begin()+i);
            delete p;
        }
        else
        {
            for(unsigned int j = 0; j < blocks.size(); j++)
            {
                powerups[i]->colliding_p(blocks[j], j==0);
            }
            if(powerups[i]->considerDude(d))
            {
                switch(powerups[i]->getType())
                {
                    case EXTRA_LIFE:
                        d->newLife();
                        break;
                    case SUPER_STRENGTH:
                        d->bulkUp();
                        break;
                    case SUPER_JUMP:
                        d->jumpHigher();
                        break;
                }
            }
            powerups[i]->perform_logic(current_time, time_step);
        }
    }
}


void BlockHandler::readScores()
{
	scoresThatIRead.clear();

	char filename[256] = "";
    sprintf(&filename[0], "%s/high_scores_%d_%d.txt", get_File_Ops().get_appdata_path().c_str(), GRID_HEIGHT, GRID_WIDTH);
    
    ifstream infile;
	infile.open(filename);
	
	vector<Score> v;
	do
	{
		Score s;
		infile >> s;
		if(s.getTime() > 0) scoresThatIRead.push_back(s);
	} while(infile.good());
	infile.close();
	sort(scoresThatIRead.begin(), scoresThatIRead.end());
}

void BlockHandler::writeScores()
{
	sort(scoresThatIRead.begin(), scoresThatIRead.end());
    
    char filename[512] = "";
    sprintf(&filename[0], "%s/high_scores_%d_%d.txt", get_File_Ops().get_appdata_path().c_str(), GRID_HEIGHT, GRID_WIDTH);
    
	ofstream outfile;
	outfile.open(filename);
    
	for(unsigned int i = 0; i < scoresThatIRead.size() && i < 10; i++)
	{
		if(scoresThatIRead[i].getTime() > 0.0f) outfile << scoresThatIRead[i];
	}
    
	outfile.close();
}

bool Score::operator< (const Score &other) const
{
    return other.time < time;
}

istream& operator>>(istream &in, Score &s)
{
	in >> s.time >> s.numSpawned >> s.numDestroyed;
	return in;
}

ostream& operator<<(ostream &out, Score &s)
{
	out << s.time << ' ' << s.numSpawned << ' ' << s.numDestroyed << endl;
	return out;
}