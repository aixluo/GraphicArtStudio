#pragma once
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class CMatrixTool
{
public:
    // Matrix operations
    static void Identity(double matrix[3][3]);
    static void Multiply(double m1[3][3], double m2[3][3], double result[3][3]);
    static void TransformPoint(CPoint& pt, double matrix[3][3]);
    static void TransformPoint(int& x, int& y, double matrix[3][3]);
    static void TransformPoint(double& x, double& y, double matrix[3][3]);
    
    // Basic transformations
    static void Translate(double dx, double dy, double matrix[3][3]);
    static void Scale(double sx, double sy, double matrix[3][3]);
    static void Rotate(double angle, double matrix[3][3]); // angle in degrees
    static void Shear(double shx, double shy, double matrix[3][3]);
    
    // Symmetry transformations
    static void SymmetryX(double matrix[3][3]);
    static void SymmetryY(double matrix[3][3]);
    static void SymmetryOrigin(double matrix[3][3]);
    static void SymmetryLine(double a, double b, double c, double matrix[3][3]);
    
    // Transformation about a point
    static void ScaleAboutPoint(double sx, double sy, CPoint center, double matrix[3][3]);
    static void RotateAboutPoint(double angle, CPoint center, double matrix[3][3]);
    static void ShearAboutPoint(double shx, double shy, CPoint center, double matrix[3][3]);
    
    // Helper functions
    static double DegToRad(double degrees);
    static void PrintMatrix(double matrix[3][3]); // For debugging
};
