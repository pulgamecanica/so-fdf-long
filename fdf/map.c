#include "fdf/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define LINE_MAX_LEN 4096

static int parse_token(const char *token, t_point *point) {
    char *comma = strchr(token, ',');
    if (comma) {
        *comma = '\0';
        point->z = atoi(token);

        if (strncmp(comma + 1, "0x", 2) == 0 || strncmp(comma + 1, "0X", 2) == 0) {
            point->color = (int)strtol(comma + 2, NULL, 16);
        } else {
            return 0;
        }
        *comma = ',';
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

    FILE *file = fopen(filename, "r");
    if (!file) return -2;

    char line[LINE_MAX_LEN];
    int width = -1;
    int height = 0;
    t_point **temp_rows = NULL;

    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '\n' || line[0] == '\0')
            continue;

        int current_width = count_tokens(line);
        if (width == -1) {
            width = current_width;
        } else if (current_width != width) {
            fclose(file);
            map_free(out);
            return -3;
        }

        t_point *row = malloc(sizeof(t_point) * width);
        if (!row) {
            fclose(file);
            map_free(out);
            return -4;
        }

        char *token = strtok(line, " \t\n");
        for (int i = 0; i < width; ++i) {
            if (!token || !parse_token(token, &row[i])) {
                free(row);
                fclose(file);
                map_free(out);
                return -5; // parse error
            }
            token = strtok(NULL, " \t\n");
        }

        t_point **new_rows = realloc(temp_rows, sizeof(t_point *) * (height + 1));
        if (!new_rows) {
            free(row);
            fclose(file);
            map_free(out);
            return -6;
        }
        temp_rows = new_rows;
        temp_rows[height++] = row;
    }

    fclose(file);

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
        "assets/fdf/42.fdf",
        "assets/fdf/t1.fdf",
        "assets/fdf/t2.fdf"
    };
    int count = sizeof(filenames) / sizeof(*filenames);
    if (index < 0 || index >= count)
        return ""; // empty in case of error
    return filenames[index];
}

