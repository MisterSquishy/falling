#include "BlockHandler.h"

BlockHandler::BlockHandler(void)
{
	GRID_HEIGHT = get_Window().get_height() / (int) GRID_SIZE;
	GRID_WIDTH = (get_Window().get_width() / (int) GRID_SIZE) + 1;

    timeToWait = INITIAL_TIME_BETWEEN_BLOCKS;
    lastCleanup = UNDEFINED;

	lastBlockSpawn = UNDEFINED;
	startedRespawn = UNDEFINED;
	min_index = 0;

	rs.setCur(true);
    
    state = BH_NORMAL;
    
	for(int i = 0; i < GRID_WIDTH; i++)
    {
		blocks.push_back(vector<Block*>());
    }
    
	d = new Dude(Point2f(0, (get_Window().get_height() / 2) - DUDE_SIZE/2), Vector2f(DUDE_SIZE, DUDE_SIZE), Global::pi*2.0f, GRID_HEIGHT * 1.6f, GRID_WIDTH * 1.1f);
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
                cleanupIndex = -20;
            }
            
            if(current_time-lastCleanup >= TIME_TO_WAIT_BETWEEN_CLEANUP_DELETIONS)
            {
                if(cleanupIndex >= 0 && cleanupIndex < blocks.size())
				{
                    blocks[cleanupIndex].erase(blocks[cleanupIndex].begin());
					play_sound("crumble");
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
                float speed = (rand() % MAX_Y_SPEED) + 1;
                
                int idx = rand() % GRID_WIDTH;
				if(idx%2 == 0) idx++;
				if(idx>=GRID_WIDTH) idx-=2;
                while(blocks[idx].size() >= GRID_HEIGHT - 2) idx = rand() % GRID_WIDTH;

				int type = rand() % 10;
				BlockType bt = (type <= 2) ? DIRT : TITANIUM;

				//pos, size, theta, y speed, x speed, accel
                blocks[idx].push_back(new Block(bt, Point2f(GRID_SIZE * idx, 0.0f-GRID_SIZE), Vector2f(GRID_SIZE, GRID_SIZE), Global::pi*2.0f, (float)speed));
				rs.spawned();

                timeToWait = (rand() % 2) + 1;
                lastBlockSpawn = current_time;
            }

			//See if its Barney time!
			BlockHandlerState oldState = state;
			state = BH_CLEANUP;
			for(int i = 0; i < blocks.size(); i++)
			{
			    if(blocks[i].size() < 2)
			    {
			        state = oldState;
			        break;
			    }
			}

			updateBlocks(current_time, time_step);

			// Tell the dude to consider all of the blocks for collision
			for(unsigned int i = 0; i < blocks.size(); i++)
				d->colliding(blocks[i], i==0);
			if(d->perform_logic(current_time, time_step, cs))
			{
				state = BH_RESPAWNING_DUDE;
			}
			break;
		}
		case BH_RESPAWNING_DUDE:
			if(startedRespawn == UNDEFINED)
			{
				startedRespawn = current_time;
				for(unsigned int i = 1; i < blocks.size(); i++)
				{
					if(blocks[i].size() <= blocks[min_index].size()) min_index = i;
				}
			}
			else if(current_time - startedRespawn >= 2.0f)
			{
				startedRespawn = UNDEFINED;
				min_index = 0;
				d->spawn(Point2f(GRID_SIZE * min_index, 0));
				state = BH_NORMAL;
				lastBlockSpawn = current_time;
			}
			rs.setTime(current_time);
			updateBlocks(current_time, time_step);
			break;
		case BH_POST_DEATH:
			if(cs.retry) // Reset everything
			{
				for(unsigned int i = 0; i < blocks.size(); i++)
				{
					while(!blocks[i].empty())
					{
						Block* b = blocks[i].back();
						blocks[i].pop_back();
						delete b;
						lastCleanup = UNDEFINED;
					}
				}
				d->revive();

				timeToWait = INITIAL_TIME_BETWEEN_BLOCKS;
				lastCleanup = UNDEFINED;

				lastBlockSpawn = UNDEFINED;
				
				rs.reset();
	    
			    state = BH_NORMAL;

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

		char* scoreText = rs.getName(false);
		fr.render_text(scoreText, Point2f(10.0f, 00.0f), get_Colors()["black"], ZENI_LEFT);
		free(scoreText);
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
		fr.render_text(message, Point2f(10.0f, 10.0f), get_Colors()["black"], ZENI_LEFT);
		free(scoreText);


		fr.render_text("HIGH SCORES", Point2f(get_Window().get_width(), 10.0f), get_Colors()["black"], ZENI_RIGHT);
		char * name;
		for(unsigned int i = 0; i < scoresThatIRead.size(); i++)
		{
			Score s = scoresThatIRead[i];
			name = s.getName(false);
			fr.render_text(name, Point2f(get_Window().get_width()-10.0f, 50.0f+(20.0f*i)), get_Colors()[(s.isCur() ? "red" : "black")], ZENI_RIGHT);
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
          
			if(block->isDestroyed())
			{
				blocks[i].erase(blocks[i].begin() + j);
				rs.destroyed();
				delete block;
				play_sound("crumble");
				continue;
			}
            
			block->colliding(blocks[i], true);
			block->perform_logic(current_time, time_step);
		}
	}
}

void BlockHandler::readScores()
{
	scoresThatIRead.clear();
	ifstream infile;
	infile.open((get_File_Ops().get_appdata_path() + "high_scores.txt").c_str());
	
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

	ofstream outfile;
	outfile.open((get_File_Ops().get_appdata_path() + "/high_scores.txt").c_str());

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