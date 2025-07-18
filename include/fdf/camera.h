#ifndef CAMERA_H
# define CAMERA_H

typedef enum e_projection_mode {
    PROJECTION_ISO,
    PROJECTION_ORTHO,
    PROJECTION_CONIC,
    PROJECTION_PARALLEL
} t_projection_mode;

typedef struct s_camera {
    float               zoom;

    float               angle_x;
    float               angle_y;
    float               angle_z;

    float               offset_x;
    float               offset_y;
    float               offset_z;

    float               scale_z;

    float               eye_distance;

    float               rot_offset_x;
    float               rot_offset_y;
    float               rot_offset_z;
    t_projection_mode   mode;
} t_camera;

void    camera_init(t_camera *cam);
void    camera_project_point(int x, int y, int z, const t_camera *cam, int *out_x, int *out_y);

#endif
