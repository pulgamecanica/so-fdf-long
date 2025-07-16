#include "fdf/map.h"
#include "gnl.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

static int parse_token(const char *token, t_point *point) {
    char *comma = strchr(token, ',');
    if (comma) {
        // Parse z part
        char *z_str = strndup(token, comma - token);
        if (!z_str) return 0;
        point->z = atoi(z_str);
        free(z_str);

        const char *color_str = comma + 1;
        if (strncmp(color_str, "0x", 2) != 0 && strncmp(color_str, "0X", 2) != 0)
            return 0;

        const char *hex = color_str + 2;
        size_t len = strlen(hex);
        unsigned int value = (unsigned int)strtoul(hex, NULL, 16);

        if (len < 8) {
            // RRGGBB → force alpha
            point->color = 0xFF000000 | value;
        } else {
            // RRGGBBAA → convert to AARRGGBB
            point->color = ((value & 0xFF) << 24) | (value >> 8);
        }
    } else {
        point->z = atoi(token);
        point->color = -1;
    }
    return 1;
}

static int count_tokens(const char *line) {
    int count = 0;
    const char *p = line;
    while (*p) {
        while (*p && isspace(*p)) p++;
        if (*p) count++;
        while (*p && !isspace(*p)) p++;
    }
    return count;
}

int map_load(const char *filename, t_map *out) {
    if (!filename || !out) return -1;

    int fd = open(filename, O_RDONLY);
    if (fd == -1) return -2;

    int width = -1;
    int height = 0;
    t_point **temp_rows = NULL;

    char *line = NULL;
    while ((line = get_next_line_no_nl(fd))) {
        if (!line || !strlen(line) || line[0] == '\n')
            continue;
        int current_width = count_tokens(line);
        if (width == -1) {
            width = current_width;
        } else if (current_width != width) {
            close(fd);
            map_free(out);
            return -3;
        }

        t_point *row = malloc(sizeof(t_point) * width);
        if (!row) {
            close(fd);
            map_free(out);
            return -4;
        }

        char *token = strtok(line, " \t\n");
        for (int i = 0; i < width; ++i) {
            if (!token || !parse_token(token, &row[i])) {
                free(row);
                close(fd);
                map_free(out);
                return -5; // parse error
            }
            token = strtok(NULL, " \t\n");
        }

        t_point **new_rows = realloc(temp_rows, sizeof(t_point *) * (height + 1));
        if (!new_rows) {
            free(row);
            close(fd);
            map_free(out);
            return -6;
        }
        temp_rows = new_rows;
        temp_rows[height++] = row;
    }

    close(fd);

    out->width = width;
    out->height = height;
    out->points = temp_rows;
    return 0;
}

void map_free(t_map *map) {
    if (!map || !map->points) return;
    for (int i = 0; i < map->height; ++i)
        free(map->points[i]);
    free(map->points);
    map->points = NULL;
    map->width = 0;
    map->height = 0;
}

const char *fdf_get_map_filename(int index) {
    static const char *filenames[] = {
        "assets/fdf/t1.fdf",
        "assets/fdf/t2.fdf",
        "assets/fdf/100-6.fdf",
        "assets/fdf/10-2.fdf",
        "assets/fdf/20-60.fdf",
        "assets/fdf/42.fdf",
        "assets/fdf/elem2.fdf",
        "assets/fdf/elem-fract.fdf",
        "assets/fdf/julia.fdf",
        "assets/fdf/mars.fdf",
        "assets/fdf/pentenegpos.fdf",
        "assets/fdf/pyra.fdf",
        "assets/fdf/pyramide.fdf"
    };
    int count = sizeof(filenames) / sizeof(*filenames);
    if (index < 0 || index >= count)
        return ""; // empty string in case of invalid index
    return filenames[index];
}
