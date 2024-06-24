#include "../include/transform.h"
#include <math.h>
#include <stdio.h>

/// Vector Functions

void vector_set(Vector *v, double x, double y, double z) {
    v->val[0] = x;
    v->val[1] = y;
    v->val[2] = z;
    v->val[3] = 0.0;
}

void vector_print(Vector *v, FILE *fp) {
    if (fp != NULL) {
        fprintf(fp, "Vector: (%.3f, %.3f, %.3f, %.3f)\n",
                v->val[0], v->val[1], v->val[2], v->val[3]);
    }
}

void vector_copy(Vector *dest, Vector *src) {
    if (dest != NULL && src != NULL) {
        for (int i = 0; i < 4; i++) {
            dest->val[i] = src->val[i];
        }
    }
}

double vector_length(Vector *v) {
    if (v != NULL) {
        double x = v->val[0];
        double y = v->val[1];
        double z = v->val[2];
        return sqrt(x*x + y*y + z*z);
    }
    return 0.0;
}

void vector_normalize(Vector *v) {
    if (v != NULL) {
        double length = vector_length(v);
        // Check to avoid division by zero
        if (length != 0) {
            v->val[0] /= length;
            v->val[1] /= length;
            v->val[2] /= length;
            // v->val[3] remains unchanged
        }
    }
}

double vector_dot(Vector *a, Vector *b) {
    if (a != NULL && b != NULL) {
        double sum = 0.0;
        for (int i = 0; i < 3; i++) {
            sum += a->val[i] * b->val[i];
        }
        return sum;
    }
    return 0.0;
}

void vector_cross(Vector *a, Vector *b, Vector *c) {
    if (a != NULL && b != NULL && c != NULL) {
        c->val[0] = a->val[1] * b->val[2] - a->val[2] * b->val[1];
        c->val[1] = a->val[2] * b->val[0] - a->val[0] * b->val[2];
        c->val[2] = a->val[0] * b->val[1] - a->val[1] * b->val[0];
        c->val[3] = 0.0;
    }
}

/// 2D Transformations

void matrix_print(Matrix *m, FILE *fp) {
    if (fp != NULL) {
        for (int i = 0; i < 4; i++) {
            fprintf(fp, "| ");
            for (int j = 0; j < 4; j++) {
                fprintf(fp, "%.3f ", m->m[i][j]);
            }
            fprintf(fp, "|\n");
        }
        fprintf(fp, "\n");
    }
}

void matrix_clear(Matrix *m) {
    if (m != NULL) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                m->m[i][j] = 0.0;
            }
        }
    }
}

void matrix_identity(Matrix *m) {
    if (m != NULL) {
        matrix_clear(m);
        for (int i = 0; i < 4; i++) {
            m->m[i][i] = 1.0;
        }
    }
}

double matrix_get(Matrix *m, int r, int c) {
    if (m != NULL && r >= 0 && r < 4 && c >= 0 && c < 4) {
        return m->m[r][c];
    }
    return 0.0;
}

void matrix_set(Matrix *m, int r, int c, double v) {
    if (m != NULL && r >= 0 && r < 4 && c >= 0 && c < 4) {
        m->m[r][c] = v;
    }
}

void matrix_copy(Matrix *dest, Matrix *src) {
    if (dest != NULL && src != NULL) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                dest->m[i][j] = src->m[i][j];
            }
        }
    }
}

void matrix_transpose(Matrix *m) {
    if (m != NULL) {
        for (int i = 0; i < 4; i++) {
            for (int j = i + 1; j < 4; j++) {
                double temp = m->m[i][j];
                m->m[i][j] = m->m[j][i];
                m->m[j][i] = temp;
            }
        }
    }
}

void matrix_multiply(Matrix *left, Matrix *right, Matrix *m) {
    if (left != NULL && right != NULL && m != NULL) {
        Matrix temp;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                double sum = 0.0;
                for (int k = 0; k < 4; k++) {
                    sum += left->m[i][k] * right->m[k][j];
                }
                temp.m[i][j] = sum;
            }
        }
        matrix_copy(m, &temp);
    }
}

void matrix_xformPoint(Matrix *m, Point *p, Point *q) {
    if (m != NULL && p != NULL && q != NULL) {
        Point tmp = *p;  // Copy the input point p into tmp
        q->val[0] = m->m[0][0] * tmp.val[0] + m->m[0][1] * tmp.val[1] +
                    m->m[0][2] * tmp.val[2] + m->m[0][3] * tmp.val[3];
        q->val[1] = m->m[1][0] * tmp.val[0] + m->m[1][1] * tmp.val[1] +
                    m->m[1][2] * tmp.val[2] + m->m[1][3] * tmp.val[3];
        q->val[2] = m->m[2][0] * tmp.val[0] + m->m[2][1] * tmp.val[1] +
                    m->m[2][2] * tmp.val[2] + m->m[2][3] * tmp.val[3];
        q->val[3] = m->m[3][0] * tmp.val[0] + m->m[3][1] * tmp.val[1] +
                    m->m[3][2] * tmp.val[2] + m->m[3][3] * tmp.val[3];
    }
}


void matrix_xformVector(Matrix *m, Vector *p, Vector *q) {
    if (m != NULL && p != NULL && q != NULL) {
        Vector tmp = *p;  // Copy the input vector p into tmp
        q->val[0] = m->m[0][0] * tmp.val[0] + m->m[0][1] * tmp.val[1] +
                    m->m[0][2] * tmp.val[2] + m->m[0][3] * tmp.val[3];
        q->val[1] = m->m[1][0] * tmp.val[0] + m->m[1][1] * tmp.val[1] +
                    m->m[1][2] * tmp.val[2] + m->m[1][3] * tmp.val[3];
        q->val[2] = m->m[2][0] * tmp.val[0] + m->m[2][1] * tmp.val[1] +
                    m->m[2][2] * tmp.val[2] + m->m[2][3] * tmp.val[3];
        q->val[3] = m->m[3][0] * tmp.val[0] + m->m[3][1] * tmp.val[1] +
                    m->m[3][2] * tmp.val[2] + m->m[3][3] * tmp.val[3];
    }
}


void matrix_xformPolygon(Matrix *m, Polygon *p) {
    if (m != NULL && p != NULL) {
        for (int i = 0; i < p->nVertex; i++) {
            matrix_xformPoint(m, &p->vertex[i], &p->vertex[i]);
            point_normalize(&p->vertex[i]);
        }
    }
}

void matrix_xformPolyline(Matrix *m, Polyline *p) {
    if (m != NULL && p != NULL) {
        for (int i = 0; i < p->numVertex; i++) {
            matrix_xformPoint(m, &p->vertex[i], &p->vertex[i]);
            point_normalize(&p->vertex[i]);
        }
    }
}

void matrix_xformLine(Matrix *m, Line *p) {
    if (m != NULL && p != NULL) {
        matrix_xformPoint(m, &p->a, &p->a);
        matrix_xformPoint(m, &p->b, &p->b);
    }
}

void matrix_scale2D(Matrix *m, double sx, double sy) {
    Matrix scale = {0};
    matrix_identity(&scale);
    scale.m[0][0] = sx;
    scale.m[1][1] = sy;
    matrix_multiply(&scale, m, m);
}

void matrix_rotateZ(Matrix *m, double cth, double sth) {
    Matrix rotate = {0};
    matrix_identity(&rotate);
    rotate.m[0][0] = cth;
    rotate.m[0][1] = -sth;
    rotate.m[1][0] = sth;
    rotate.m[1][1] = cth;
    matrix_multiply(&rotate, m, m);
}

void matrix_translate2D(Matrix *m, double tx, double ty) {
    Matrix translate = {0};
    matrix_identity(&translate);
    translate.m[0][3] = tx;
    translate.m[1][3] = ty;
    matrix_multiply(&translate, m, m);
}

void matrix_shear2D(Matrix *m, double shx, double shy) {
    Matrix shear = {0};
    matrix_identity(&shear);
    shear.m[0][1] = shx;
    shear.m[1][0] = shy;
    matrix_multiply(&shear, m, m);
}

/// 3D Transformations

void matrix_translate(Matrix *m, double tx, double ty, double tz) {
    Matrix translate = {0};
    matrix_identity(&translate);
    translate.m[0][3] = tx;
    translate.m[1][3] = ty;
    translate.m[2][3] = tz;
    matrix_multiply(&translate, m, m);
}

void matrix_scale(Matrix *m, double sx, double sy, double sz) {
    Matrix scale = {0};
    matrix_identity(&scale);
    scale.m[0][0] = sx;
    scale.m[1][1] = sy;
    scale.m[2][2] = sz;
    matrix_multiply(&scale, m, m);
}

void matrix_rotateX(Matrix *m, double cth, double sth) {
    Matrix rotate = {0};
    matrix_identity(&rotate);
    rotate.m[1][1] = cth;
    rotate.m[1][2] = -sth;
    rotate.m[2][1] = sth;
    rotate.m[2][2] = cth;
    matrix_multiply(&rotate, m, m);
}

void matrix_rotateY(Matrix *m, double cth, double sth) {
    Matrix rotate = {0};
    matrix_identity(&rotate);
    rotate.m[0][0] = cth;
    rotate.m[0][2] = sth;
    rotate.m[2][0] = -sth;
    rotate.m[2][2] = cth;
}

void matrix_rotateXYZ(Matrix *m, Vector *u, Vector *v, Vector *w) {
    Matrix rotate = {0};
    matrix_identity(&rotate);
    for (int i = 0; i < 3; i++) {
        rotate.m[0][i] = u->val[i];
        rotate.m[1][i] = v->val[i];
        rotate.m[2][i] = w->val[i];
    }
    matrix_multiply(&rotate, m, m);
}

void matrix_shearZ(Matrix *m, double shx, double shy) {
    Matrix shear = {0};
    matrix_identity(&shear);
    shear.m[0][2] = shx;
    shear.m[1][2] = shy;
    matrix_multiply(&shear, m, m);
}

void matrix_perspective(Matrix *m, double d) {
    Matrix perspective = {0};
    matrix_identity(&perspective);
    perspective.m[3][2] = 1.0 / d;
    matrix_multiply(&perspective, m, m);
}