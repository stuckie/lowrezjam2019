#include "fishy_area.h"

#include <stdlib.h>
#include <time.h>
#include <math.h>

static void water_hole_fill_pos(gae_grid_t* grid, gae_grid_cell_position_t from, gae_grid_cell_position_t to)
{
	gae_grid_cell_t* cellA = gae_grid_cell_at(grid, &from);
	gae_grid_cell_t* cellB = gae_grid_cell_at(grid, &to);
	
	water_cell_t* tileA = cellA->data;
	water_cell_t* tileB = cellB->data;
	
	int depthA = tileA->depth;
	int depthB = tileB->depth;
	
	unsigned int dist = fabs(gae_point_distancef(from.x, from.y, to.x, to.y));
	int newDepth = depthA - dist;
	
	if (0 == depthB) depthB = newDepth;
	else depthB = gae_max(newDepth, depthB);
	
	tileB->depth = gae_clamp(depthB, 0, 255);
}

static void water_hole_fill(gae_grid_t* grid, gae_grid_cell_position_t position, int width)
{
	gae_grid_cell_t* cell = gae_grid_cell_at(grid, &position);
	water_cell_t* tile = cell->data;
	int depth = tile->depth - width;
	gae_grid_cell_position_t newpos;
	
	unsigned int left = position.x - width;
	unsigned int right = position.x + width;
	unsigned int top = position.y - width;
	unsigned int bottom = position.y + width;
	
	if (0 == depth) return;
	newpos.x = left;
	while (newpos.x < right + 1) {
		newpos.y = top;
		while (newpos.y < bottom + 1) {
			if (newpos.x < grid->columns && newpos.y < grid->rows)
				water_hole_fill_pos(grid, position, newpos);
			
			++newpos.y;
		}
		++newpos.x;
	}
	
	if (0 < depth) water_hole_fill(grid, position, ++width);
}

static void water_depth_create(gae_grid_t* grid, int holes)
{
    int i = 0;
    gae_grid_cell_position_t* positions = gae_alloc(sizeof(gae_grid_cell_position_t) * holes);
	gae_grid_cell_position_t* position = &positions[0];

    for (; i < holes; ++i) {
        gae_grid_cell_t* cell = 0;
        water_cell_t* tile = 0;

        unsigned char depth = rand() % 10;
        position->x = rand() % grid->columns;
        position->y = rand() % grid->rows;
        cell = gae_grid_cell_at(grid, position);
        tile = cell->data;
        tile->depth = depth;
        ++position;
    }
    
    for (i = 0; i < holes; ++i) {
		water_hole_fill(grid, positions[i], 1);
	}
	
	gae_free(positions);
}

water_area_t* water_area_init(water_area_t* area, unsigned int width, unsigned int height, gae_list_t* fish)
{
    int i = 0;
    int size = width * height;
    (void)(fish);

    srand(time(0));

    gae_grid_init(&area->area, width, height);
    for (; i < size; ++i) {
        gae_grid_cell_t* cell = area->area.cells + i;
        water_cell_t* tile = gae_alloc(sizeof(water_cell_t));
        tile->depth = 0;
        tile->fish = 0;
        cell->data = tile;
    }

    water_depth_create(&area->area, height + rand() % width);

    return area;
}

#include <stdio.h>
water_area_t* water_area_print(water_area_t* area)
{
    int i = 0;
    int size = area->area.columns * area->area.rows;

    for (; i < size; ++i) {
        gae_grid_cell_t* cell = area->area.cells + i;
        water_cell_t* tile = cell->data;
        if (0 == i % area->area.columns) printf("\n");
        printf("%d", tile->depth);
    }
    printf("\n");

    return area;
}

water_area_t* water_area_destroy(water_area_t* area)
{
    int i = 0;
    int size = area->area.columns * area->area.rows;

    for (; i < size; ++i) {
        gae_grid_cell_t* cell = area->area.cells + i;
        water_cell_t* tile = cell->data;
        gae_free(tile);
        cell->data = 0;
    }

    gae_grid_destroy(&area->area);

    return area;
}
