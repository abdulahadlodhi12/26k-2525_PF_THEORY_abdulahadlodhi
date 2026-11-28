#include <math.h>
#include <stdio.h>
#include <stdlib.h>

enum {
    shape_dot = 0,
    shape_line = 1,
    shape_triangle = 2,
    shape_square = 3,
    shape_rectangle = 4
};

enum {
    prop_area = 1,
    prop_perimeter = 2,
    prop_origin_dist = 3
};

typedef struct {
    int type;
    float x;
    float y;
} dot_t;

typedef struct {
    int type;
    float x1;
    float y1;
    float x2;
    float y2;
} line_t;

typedef struct {
    int type;
    float x1;
    float y1;
    float x2;
    float y2;
    float x3;
    float y3;
} triangle_t;

typedef struct {
    int type;
    float x;
    float y;
    float side;
} square_t;

typedef struct {
    int type;
    float x;
    float y;
    float width;
    float height;
} rectangle_t;

static float dist2d(float ax, float ay, float bx, float by) {
    float dx = ax - bx;
    float dy = ay - by;
    return sqrtf(dx * dx + dy * dy);
}

float calculateProperty(void *shapePtr, int propertyType) {
    if (!shapePtr) {
        return 0.0f;
    }

    int kind = *(int *)shapePtr;

    switch (kind) {
        case shape_dot: {
            dot_t *d = (dot_t *)shapePtr;
            if (propertyType == prop_area) {
                return 0.0f;
            }
            if (propertyType == prop_perimeter) {
                return 0.0f;
            }
            if (propertyType == prop_origin_dist) {
                return dist2d(d->x, d->y, 0.0f, 0.0f);
            }
            break;
        }
        case shape_line: {
            line_t *l = (line_t *)shapePtr;
            if (propertyType == prop_area) {
                return 0.0f;
            }
            if (propertyType == prop_perimeter) {
                return dist2d(l->x1, l->y1, l->x2, l->y2);
            }
            if (propertyType == prop_origin_dist) {
                float d1 = dist2d(l->x1, l->y1, 0.0f, 0.0f);
                float d2 = dist2d(l->x2, l->y2, 0.0f, 0.0f);
                return (d1 < d2) ? d1 : d2;
            }
            break;
        }
        case shape_triangle: {
            triangle_t *t = (triangle_t *)shapePtr;
            float a = dist2d(t->x1, t->y1, t->x2, t->y2);
            float b = dist2d(t->x2, t->y2, t->x3, t->y3);
            float c = dist2d(t->x3, t->y3, t->x1, t->y1);
            if (propertyType == prop_area) {
                float area2 = fabsf(t->x1 * (t->y2 - t->y3) +
                                    t->x2 * (t->y3 - t->y1) +
                                    t->x3 * (t->y1 - t->y2));
                return 0.5f * area2;
            }
            if (propertyType == prop_perimeter) {
                return a + b + c;
            }
            if (propertyType == prop_origin_dist) {
                float cx = (t->x1 + t->x2 + t->x3) / 3.0f;
                float cy = (t->y1 + t->y2 + t->y3) / 3.0f;
                return dist2d(cx, cy, 0.0f, 0.0f);
            }
            break;
        }
        case shape_square: {
            square_t *s = (square_t *)shapePtr;
            if (propertyType == prop_area) {
                return s->side * s->side;
            }
            if (propertyType == prop_perimeter) {
                return 4.0f * s->side;
            }
            if (propertyType == prop_origin_dist) {
                return dist2d(s->x, s->y, 0.0f, 0.0f);
            }
            break;
        }
        case shape_rectangle: {
            rectangle_t *r = (rectangle_t *)shapePtr;
            if (propertyType == prop_area) {
                return r->width * r->height;
            }
            if (propertyType == prop_perimeter) {
                return 2.0f * (r->width + r->height);
            }
            if (propertyType == prop_origin_dist) {
                return dist2d(r->x, r->y, 0.0f, 0.0f);
            }
            break;
        }
        default:
            printf("unkown shape type %d, returning 0\n", kind);
            break;
    }

    return 0.0f;
}

int main(void) {
    dot_t *d = (dot_t*)malloc(sizeof(dot_t));
    *d = (dot_t){shape_dot, 3.0f, 4.0f};

    line_t *l = (line_t*)malloc(sizeof(line_t));
    *l = (line_t){shape_line, 0.0f, 0.0f, 6.0f, 8.0f};

    triangle_t *t = (triangle_t*)malloc(sizeof(triangle_t));
    *t = (triangle_t){shape_triangle, 0.0f, 0.0f, 4.0f, 0.0f, 0.0f, 3.0f};

    square_t *s = (square_t*)malloc(sizeof(square_t));
    *s = (square_t){shape_square, 2.0f, 2.0f, 5.0f};

    rectangle_t *r = (rectangle_t*)malloc(sizeof(rectangle_t));
    *r = (rectangle_t){shape_rectangle, 1.0f, 1.0f, 4.0f, 6.0f};

    void *shapes[5] = {d, l, t, s, r};
    const char *names[5] = {"dot", "line", "triangle", "square", "rectagle"};

    for (int i = 0; i < 5; i++) {
        float ar = calculateProperty(shapes[i], prop_area);
        float pe = calculateProperty(shapes[i], prop_perimeter);
        float di = calculateProperty(shapes[i], prop_origin_dist);
        printf("%s -> area: %.2f | perim/len: %.2f | dist from origin: %.2f\n",
               names[i], ar, pe, di);
    }

    for (int i = 0; i < 5; i++) {
        free(shapes[i]);
    }
    return 0;
}
