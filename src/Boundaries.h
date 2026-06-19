#pragma once

#include "Wall.h"

/**
 * Boundaries of the map. Contains walls, doors and ribs.
 */
struct Boundaries
{
    std::vector<Wall_coloured> walls;	// vector of walls
    std::vector<Wall_coloured> doors;	// vector of doors
    std::vector<Wall> ribs;				// vector of ribs
};