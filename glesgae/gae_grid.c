#include "gae_grid.h"
#include "gae_memory.h"

gae_grid_t* gae_grid_init(gae_grid_t* grid, unsigned int columns, unsigned int rows)
{
	grid->columns = columns;
	grid->rows = rows;
	grid->cells = gae_alloc(sizeof(gae_grid_cell_t) * columns * rows);

	if (0 == grid->cells) { /* out of memory */
		return 0;
	}

	return grid;
}

gae_grid_cell_t* gae_grid_cell_at(gae_grid_t* const grid, gae_grid_cell_position_t* position)
{
	return grid->cells + (position->y * grid->columns + position->x);
}

gae_grid_t* gae_grid_destroy(gae_grid_t* grid)
{
	gae_free(grid->cells);
	
	return grid;
}

