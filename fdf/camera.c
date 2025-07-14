#include "fdf/camera.h"
#include <math.h>

void camera_init(t_camera *cam) {
    cam->zoom = 20.0f;
    cam->angle_x = 0.0f;
    cam->angle_y = 0.0f;
    cam->angle_z = 0.0f;
    cam->offset_x = 0.0f;
    cam->offset_y = 0.0f;
    cam->offset_z = 1.0f;
    cam->mode = PROJECTION_ISO;
}

// Helper: rotate point in 3D (basic, not optimized)
static void rotate(float *x, float *y, float *z, const t_camera *cam) {
    float tmp;

    // Rotate around X
    tmp = *y;
    *y = tmp * cosf(cam->angle_x) - *z * sinf(cam->angle_x);
    *z = tmp * sinf(cam->angle_x) + *z * cosf(cam->angle_x);

    // Rotate around Y
    tmp = *x;
    *x = tmp * cosf(cam->angle_y) + *z * sinf(cam->angle_y);
    *z = -tmp * sinf(cam->angle_y) + *z * cosf(cam->angle_y);

    // Rotate around Z
    tmp = *x;
    *x = tmp * cosf(cam->angle_z) - *y * sinf(cam->angle_z);
    *y = tmp * sinf(cam->angle_z) + *y * cosf(cam->angle_z);
}

void camera_project_point(int x, int y, int z, const t_camera *cam, int *out_x, int *out_y) {
    float fx = x * cam->zoom;
    float fy = y * cam->zoom;
    float fz = z * cam->offset_z * cam->zoom;

    rotate(&fx, &fy, &fz, cam);

    if (cam->mode == PROJECTION_ISO) {
        *out_x = (int)((fx - fy) * cosf(0.523599)) + (int)cam->offset_x;
        *out_y = (int)((fx + fy) * sinf(0.523599) - fz) + (int)cam->offset_y;
    } else { // PROJECTION_ORTHO
        *out_x = (int)(fx + cam->offset_x);
        *out_y = (int)(fy - fz + cam->offset_y);
    }
}
