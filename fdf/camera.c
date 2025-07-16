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
    cam->rot_offset_x = 0.0f;
    cam->rot_offset_y = 0.0f;
    cam->rot_offset_z = 0.0f;
    cam->mode = PROJECTION_ISO;
}

// Helper: rotate point in 3D (basic, not optimized)
static void rotate(float *x, float *y, float *z, const t_camera *cam) {
    float tmp;
    float ax = cam->angle_x + cam->rot_offset_x;
    float ay = cam->angle_y + cam->rot_offset_y;
    float az = cam->angle_z + cam->rot_offset_z;

    // Rotate around X
    tmp = *y;
    *y = tmp * cosf(ax) - *z * sinf(ax);
    *z = tmp * sinf(ax) + *z * cosf(ax);

    // Rotate around Y
    tmp = *x;
    *x = tmp * cosf(ay) + *z * sinf(ay);
    *z = -tmp * sinf(ay) + *z * cosf(ay);

    // Rotate around Z
    tmp = *x;
    *x = tmp * cosf(az) - *y * sinf(az);
    *y = tmp * sinf(az) + *y * cosf(az);
}

void camera_project_point(int x, int y, int z, const t_camera *cam, int *out_x, int *out_y) {
    float fx = x * cam->zoom;
    float fy = y * cam->zoom;
    float fz = z * cam->offset_z * cam->zoom;

    rotate(&fx, &fy, &fz, cam);

    switch (cam->mode) {
        case PROJECTION_ISO:
            *out_x = (int)((fx - fy) * cosf(0.523599f)) + (int)cam->offset_x;
            *out_y = (int)((fx + fy) * sinf(0.523599f) - fz) + (int)cam->offset_y;
            break;

        case PROJECTION_ORTHO:
            *out_x = (int)(fx + cam->offset_x);
            *out_y = (int)(fy - fz + cam->offset_y);
            break;

        case PROJECTION_CONIC: {
            float d = cam->eye_distance > 0.01f ? cam->eye_distance : 1.0f;
            float perspective = d / (d + fz);
            *out_x = (int)(fx * perspective + cam->offset_x);
            *out_y = (int)(fy * perspective + cam->offset_y);
            break;
        }

        case PROJECTION_PARALLEL: {
            // shift lines diagonally based on z
            float shift_x = 0.5f;
            float shift_y = 0.5f;
            *out_x = (int)(fx + fz * shift_x + cam->offset_x);
            *out_y = (int)(fy + fz * shift_y + cam->offset_y);
            break;
        }

        default:
            *out_x = (int)(fx + cam->offset_x);
            *out_y = (int)(fy - fz + cam->offset_y);
            break;
    }
}

