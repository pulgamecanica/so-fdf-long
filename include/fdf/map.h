#ifndef MAP_H
# define MAP_H

#include "point.h"

typedef struct s_map {
    int     width;
    int     height;
    t_point **points;
} t_map;

int     map_load(const char *filename, t_map *out);
void    map_free(t_map *map);
const char *fdf_get_map_filename(int index);

#endif