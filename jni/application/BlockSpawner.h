#ifndef BLOCKSPAWNER_H
#define BLOCKSPAWNER_H

#include "Block.h"
#include "Dude.h"
#include <vector>

class BlockSpawner
{
public:
    BlockSpawner();
    ~BlockSpawner();
    
    
    void perform_logic(float time_passed, float time_step);
    void render();
};

#endif
