#ifndef _GAE_GRID_H_
#define _GAE_GRID_H_

typedef struct gae_grid_cell_position_s {
	unsigned int x;
	unsigned int y;
} gae_grid_cell_position_t;

/* May want to actually put more metadata in the cell later, so encapsulate it now */
typedef struct gae_grid_cell_s {
	void* data;
} gae_grid_cell_t; 

typedef struct gae_grid_s {
	gae_grid_cell_t* cells;
	unsigned int columns;
	unsigned int rows;
} gae_grid_t;

/* Initialise a new Grid structure */
gae_grid_t* gae_grid_init(gae_grid_t* grid, unsigned int columns, unsigned int rows);

/* Get the cell at the given position of the grid */
gae_grid_cell_t* gae_grid_cell_at(gae_grid_t* const grid, gae_grid_cell_position_t* position);

/* Destroy the given grid */
gae_grid_t* gae_grid_destroy(gae_grid_t* grid);

#endif

