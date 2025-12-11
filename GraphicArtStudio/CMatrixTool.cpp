#include "pch.h"
#include "CMatrixTool.h"
#include <cmath>

void CMatrixTool::Identity(double matrix[3][3])
{
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            matrix[i][j] = (i == j) ? 1.0 : 0.0;
        }
    }
}

void CMatrixTool::Multiply(double m1[3][3], double m2[3][3], double result[3][3])
{
    double temp[3][3];
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            temp[i][j] = 0;
            for (int k = 0; k < 3; k++) {
                temp[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result[i][j] = temp[i][j];
        }
    }
}

void CMatrixTool::TransformPoint(CPoint& pt, double matrix[3][3])
{
    int x = pt.x;
    int y = pt.y;
    TransformPoint(x, y, matrix);
    pt.x = x;
    pt.y = y;
}

void CMatrixTool::TransformPoint(int& x, int& y, double matrix[3][3])
{
    double newX = matrix[0][0] * x + matrix[0][1] * y + matrix[0][2];
    double newY = matrix[1][0] * x + matrix[1][1] * y + matrix[1][2];
    x = (int)(newX + 0.5);
    y = (int)(newY + 0.5);
}

void CMatrixTool::TransformPoint(double& x, double& y, double matrix[3][3])
{
    double newX = matrix[0][0] * x + matrix[0][1] * y + matrix[0][2];
    double newY = matrix[1][0] * x + matrix[1][1] * y + matrix[1][2];
    x = newX;
    y = newY;
}

void CMatrixTool::Translate(double dx, double dy, double matrix[3][3])
{
    Identity(matrix);
    matrix[0][2] = dx;
    matrix[1][2] = dy;
}

void CMatrixTool::Scale(double sx, double sy, double matrix[3][3])
{
    Identity(matrix);
    matrix[0][0] = sx;
    matrix[1][1] = sy;
}

void CMatrixTool::Rotate(double angle, double matrix[3][3])
{
    Identity(matrix);
    double rad = DegToRad(angle);
    double cosA = cos(rad);
    double sinA = sin(rad);
    
    matrix[0][0] = cosA;
    matrix[0][1] = -sinA;
    matrix[1][0] = sinA;
    matrix[1][1] = cosA;
}

void CMatrixTool::Shear(double shx, double shy, double matrix[3][3])
{
    Identity(matrix);
    matrix[0][1] = shy;
    matrix[1][0] = shx;
}

void CMatrixTool::SymmetryX(double matrix[3][3])
{
    Identity(matrix);
    matrix[1][1] = -1;
}

void CMatrixTool::SymmetryY(double matrix[3][3])
{
    Identity(matrix);
    matrix[0][0] = -1;
}

void CMatrixTool::SymmetryOrigin(double matrix[3][3])
{
    Identity(matrix);
    matrix[0][0] = -1;
    matrix[1][1] = -1;
}

void CMatrixTool::SymmetryLine(double a, double b, double c, double matrix[3][3])
{
    // Symmetry about line ax + by + c = 0
    double norm = sqrt(a * a + b * b);
    if (norm < 1e-6) {
        Identity(matrix);
        return;
    }
    
    a /= norm;
    b /= norm;
    c /= norm;
    
    matrix[0][0] = 1 - 2 * a * a;
    matrix[0][1] = -2 * a * b;
    matrix[0][2] = -2 * a * c;
    matrix[1][0] = -2 * a * b;
    matrix[1][1] = 1 - 2 * b * b;
    matrix[1][2] = -2 * b * c;
    matrix[2][0] = 0;
    matrix[2][1] = 0;
    matrix[2][2] = 1;
}

void CMatrixTool::ScaleAboutPoint(double sx, double sy, CPoint center, double matrix[3][3])
{
    double t1[3][3], s[3][3], t2[3][3], temp[3][3];
    
    Translate(-center.x, -center.y, t1);
    Scale(sx, sy, s);
    Translate(center.x, center.y, t2);
    
    // Correct order: T2 * S * T1
    Multiply(s, t1, temp);  // temp = S * T1
    Multiply(t2, temp, matrix);  // matrix = T2 * temp
}

void CMatrixTool::RotateAboutPoint(double angle, CPoint center, double matrix[3][3])
{
    double t1[3][3], r[3][3], t2[3][3], temp[3][3];
    
    Translate(-center.x, -center.y, t1);
    Rotate(angle, r);
    Translate(center.x, center.y, t2);
    
    // Correct order: T2 * R * T1
    Multiply(r, t1, temp);  // temp = R * T1
    Multiply(t2, temp, matrix);  // matrix = T2 * temp
}

void CMatrixTool::ShearAboutPoint(double shx, double shy, CPoint center, double matrix[3][3])
{
    double t1[3][3], s[3][3], t2[3][3], temp[3][3];
    
    Translate(-center.x, -center.y, t1);
    Shear(shx, shy, s);
    Translate(center.x, center.y, t2);
    
    // Correct order: T2 * S * T1
    Multiply(s, t1, temp);  // temp = S * T1
    Multiply(t2, temp, matrix);  // matrix = T2 * temp
}

double CMatrixTool::DegToRad(double degrees)
{
    return degrees * M_PI / 180.0;
}

void CMatrixTool::PrintMatrix(double matrix[3][3])
{
    TRACE(_T("Matrix:\n"));
    for (int i = 0; i < 3; i++) {
        TRACE(_T("[%.2f %.2f %.2f]\n"), matrix[i][0], matrix[i][1], matrix[i][2]);
    }
}
