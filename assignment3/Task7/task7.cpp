#include <math.h>
#include <stdio.h>
#include <stdlib.h>

enum {
    type_dot = 0,
    type_line = 1
};

enum {
    prop_length = 2
};

typedef struct {
    int type;
    double x;
    double y;
} Dot;

typedef struct {
    int type;
    double x1;
    double y1;
    double x2;
    double y2;
} Line;

float calculateProperty(void *shapePtr, int propertyType) {
    if (!shapePtr) {
        return 0.0f;
    }
    int kind = *(int *)shapePtr;
    if (kind == type_line && propertyType == prop_length) {
        Line *ln = (Line *)shapePtr;
        double dx = ln->x2 - ln->x1;
        double dy = ln->y2 - ln->y1;
        return (float)sqrt(dx * dx + dy * dy);
    }
    return 0.0f;
}

static double cross_val(Dot p, Dot q, Dot r) {
    return (q.x - p.x) * (r.y - q.y) - (q.y - p.y) * (r.x - q.x);
}

static double dist_sq(Dot a, Dot b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return dx * dx + dy * dy;
}

Line *computeConvexHull(const Dot *points, int n, int *hullEdgeCount) {
    if (!points || n < 3 || !hullEdgeCount) {
        if (hullEdgeCount) {
            *hullEdgeCount = 0;
        }
        return NULL;
    }

    int *hull_idx = (int*)malloc((size_t)n * sizeof(int*));
    if (!hull_idx) {
        *hullEdgeCount = 0;
        return NULL;
    }

    int leftmost = 0;
    for (int i = 1; i < n; i++) {
        if (points[i].x < points[leftmost].x ||
            (points[i].x == points[leftmost].x && points[i].y < points[leftmost].y)) {
            leftmost = i;
        }
    }

    int p = leftmost;
    int count = 0;

    do {
        hull_idx[count++] = p;
        int q = (p + 1) % n;
        for (int i = 0; i < n; i++) {
            double cr = cross_val(points[p], points[i], points[q]);
            if (cr < 0.0 ||
                (fabs(cr) < 1e-12 && dist_sq(points[p], points[i]) > dist_sq(points[p], points[q]))) {
                q = i;
            }
        }
        p = q;
    } while (p != leftmost && count < n);

    Line *edges = (Line*)malloc((size_t)count * sizeof(Line));
    if (!edges) {
        free(hull_idx);
        *hullEdgeCount = 0;
        return NULL;
    }

    for (int i = 0; i < count; i++) {
        Dot a = points[hull_idx[i]];
        Dot b = points[hull_idx[(i + 1) % count]];
        edges[i].type = type_line;
        edges[i].x1 = a.x;
        edges[i].y1 = a.y;
        edges[i].x2 = b.x;
        edges[i].y2 = b.y;
    }

    free(hull_idx);
    *hullEdgeCount = count;
    return edges;
}

int main(void) {
    FILE *fp = fopen("points.txt", "r");
    if (!fp) {
        printf("couldnt open points.txt, make sure its there\n");
        return 1;
    }

    int n = 0;
    if (fscanf(fp, "%d", &n) != 1 || n < 3) {
        printf("need at least 3 points in points.txt\n");
        fclose(fp);
        return 1;
    }

    Dot *pts = (Dot*)malloc((size_t)n * sizeof(Dot));
    if (!pts) {
        printf("no memory left for points array\n");
        fclose(fp);
        return 1;
    }

    for (int i = 0; i < n; i++) {
        double x = 0.0;
        double y = 0.0;
        if (fscanf(fp, " %lf , %lf", &x, &y) != 2) {
            printf("trouble reading line %d in points.txt\n", i + 2);
            free(pts);
            fclose(fp);
            return 1;
        }
        pts[i].type = type_dot;
        pts[i].x = x;
        pts[i].y = y;
    }
    fclose(fp);

    int edge_count = 0;
    Line *hull_edges = computeConvexHull(pts, n, &edge_count);
    if (!hull_edges || edge_count == 0) {
        printf("couldnt build convex hull\n");
        free(pts);
        return 1;
    }

    double total_perim = 0.0;
    printf("convex hull edges:\n");
    for (int i = 0; i < edge_count; i++) {
        float len = calculateProperty(&hull_edges[i], prop_length);
        total_perim += (double)len;
        printf("edge %d: (%.2f, %.2f) (%.2f, %.2f) | len = %.2f\n",
               i + 1,
               hull_edges[i].x1,
               hull_edges[i].y1,
               hull_edges[i].x2,
               hull_edges[i].y2,
               len);
    }
    printf("total convex hull perimter: %.2f\n", total_perim);

    free(hull_edges);
    free(pts);
    return 0;
}
