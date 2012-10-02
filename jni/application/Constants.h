#ifndef CONSTANTS_H
#define CONSTANTS_H

static const float DUDE_SIZE = 64.0f;
static const float GRID_SIZE = 90.0f;
static const float POWERUP_SIZE = GRID_SIZE/2;
static int GRID_HEIGHT; // set by BlockHandler
static int GRID_WIDTH; // set by BlockHandler

static const float UNDEFINED = 0.0f;
static const float DORMANT = 0.0f;

static const float FRAME_RATE = 0.005f;
static const float SCALE = 200.0f;

static const float Y_ACCELERATION = 0.59875f;
static const float MOVE_ACC_INC = 0.48f;

static const float MAX_X_SPEED = 9.0f;
static const int MAX_Y_SPEED = 10; // for falling blocks

static const float COF = 0.55f;
static const float IN_AIR_COF = 0.05f;

static const float JUMP_VERTICAL_SPEED = 13.0f;
static const float WALL_JUMP_VERTICAL_SPEED = 11.0f;
static const float JUMP_HORIZONTAL_SPEED = 5.5f;
static const float SPIN_SPEED = 0.68f;

static const float TIME_TO_WAIT_BETWEEN_CLEANUP_DELETIONS = 0.156f;

static const int DUDE_INITIAL_LIFE_COUNT = 3;

static const float INITIAL_TIME_BETWEEN_BLOCKS = 8.0f;
static const float MIN_TIME_BETWEEN_BLOCKS = 2.0f;
static const float BLOCK_PAUSE_BEFORE_FALL = MIN_TIME_BETWEEN_BLOCKS - 0.5f;

static const float SUPER_STRENGTH_ACTIVE_TIME = 10.0f;
static const float SUPER_JUMP_ACTIVE_TIME = 10.0f;
static const float SUPER_JUMP_BOOST = 5.0f;

#endif