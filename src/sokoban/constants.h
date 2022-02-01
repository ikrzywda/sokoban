#ifndef CONSTANTS_H
#define CONSTANTS_H

#define TILE_SIZE 50
#define LEVEL_COUNT 20
#define ASSET_COUNT 8
#define LEVEL_BUFFER_SIZE 1000

static const char *const kAssetPath[] = {
    "assets/floor.png",
    "assets/wall.png",
    "assets/crate.png",
    "assets/crate_on_dest.png",
    "assets/worker.png",
    "assets/worker_on_dest.png",
    "assets/dest.png",
    "assets/error.png",
};

typedef enum AssetIndex {
    ASSET_FLOOR = 0,
    ASSET_WALL = 1,
    ASSET_CRATE = 2,
    ASSET_CRATE_ON_DEST = 3,
    ASSET_WORKER = 4,
    ASSET_WORKER_ON_DEST = 5,
    ASSET_DEST = 6,
    ASSET_ERROR = 7,
} AssetIndex;

static const char *const kLevelPath[] = {
    "levels/1.txt",
    "levels/2.txt",
    "levels/3.txt",
    "levels/4.txt",
    "levels/5.txt",
    "levels/6.txt",
    "levels/dest.png",
    "levels/error.png",
};

#endif