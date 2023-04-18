#include <core/matrix.h>
#include <core/assert.h>
#include <core/vector.h>
#include <core/point.h>

namespace rt {

    Matrix::Matrix(const Float4& r1, const Float4& r2, const Float4& r3, const Float4& r4) {
        row1 = r1;
        row2 = r2;
        row3 = r3;
        row4 = r4;
    }

    Float4& Matrix::operator[](int idx) {
        switch (idx) {
        case 0:
            return row1;
        case 1:
            return row2;
        case 2:
            return row3;
        default:
            return row4;
        }
    }

    Float4 Matrix::operator[](int idx) const {
        switch (idx) {
        case 0:
            return row1;
        case 1:
            return row2;
        case 2:
            return row3;
        default:
            return row4;
        }
    }

    Matrix Matrix::operator+(const Matrix& b) const {
        Float4 r1 = row1 + b[0];
        Float4 r2 = row2 + b[1];
        Float4 r3 = row3 + b[2];
        Float4 r4 = row4 + b[3];
        return (Matrix(r1, r2, r3, r4));
    }

    Matrix Matrix::operator-(const Matrix& b) const {
        Float4 r1 = row1 - b[0];
        Float4 r2 = row2 - b[1];
        Float4 r3 = row3 - b[2];
        Float4 r4 = row4 - b[3];
        return (Matrix(r1, r2, r3, r4));
    }

    Matrix Matrix::transpose() const {
        Float4 r1 = Float4(row1[0], row2[0], row3[0], row4[0]);
        Float4 r2 = Float4(row1[1], row2[1], row3[1], row4[1]);
        Float4 r3 = Float4(row1[2], row2[2], row3[2], row4[2]);
        Float4 r4 = Float4(row1[3], row2[3], row3[3], row4[3]);
        return (Matrix(r1, r2, r3, r4));
    }

    Matrix Matrix::invert() const {
        Matrix result;
        const Matrix& m = *this;

        // Taken and modified from http://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
        result[0][0] = m[1][1] * m[2][2] * m[3][3] - m[1][1] * m[2][3] * m[3][2] - m[2][1] * m[1][2] * m[3][3] + m[2][1] * m[1][3] * m[3][2] + m[3][1] * m[1][2] * m[2][3] - m[3][1] * m[1][3] * m[2][2];
        result[1][0] = -m[1][0] * m[2][2] * m[3][3] + m[1][0] * m[2][3] * m[3][2] + m[2][0] * m[1][2] * m[3][3] - m[2][0] * m[1][3] * m[3][2] - m[3][0] * m[1][2] * m[2][3] + m[3][0] * m[1][3] * m[2][2];
        result[2][0] = m[1][0] * m[2][1] * m[3][3] - m[1][0] * m[2][3] * m[3][1] - m[2][0] * m[1][1] * m[3][3] + m[2][0] * m[1][3] * m[3][1] + m[3][0] * m[1][1] * m[2][3] - m[3][0] * m[1][3] * m[2][1];
        result[3][0] = -m[1][0] * m[2][1] * m[3][2] + m[1][0] * m[2][2] * m[3][1] + m[2][0] * m[1][1] * m[3][2] - m[2][0] * m[1][2] * m[3][1] - m[3][0] * m[1][1] * m[2][2] + m[3][0] * m[1][2] * m[2][1];

        float det = m[0][0] * result[0][0] + m[0][1] * result[1][0] + m[0][2] * result[2][0] + m[0][3] * result[3][0];
        if (det == 0)
            return Matrix::zero();

        result[0][1] = -m[0][1] * m[2][2] * m[3][3] + m[0][1] * m[2][3] * m[3][2] + m[2][1] * m[0][2] * m[3][3] - m[2][1] * m[0][3] * m[3][2] - m[3][1] * m[0][2] * m[2][3] + m[3][1] * m[0][3] * m[2][2];
        result[1][1] = m[0][0] * m[2][2] * m[3][3] - m[0][0] * m[2][3] * m[3][2] - m[2][0] * m[0][2] * m[3][3] + m[2][0] * m[0][3] * m[3][2] + m[3][0] * m[0][2] * m[2][3] - m[3][0] * m[0][3] * m[2][2];
        result[2][1] = -m[0][0] * m[2][1] * m[3][3] + m[0][0] * m[2][3] * m[3][1] + m[2][0] * m[0][1] * m[3][3] - m[2][0] * m[0][3] * m[3][1] - m[3][0] * m[0][1] * m[2][3] + m[3][0] * m[0][3] * m[2][1];
        result[3][1] = m[0][0] * m[2][1] * m[3][2] - m[0][0] * m[2][2] * m[3][1] - m[2][0] * m[0][1] * m[3][2] + m[2][0] * m[0][2] * m[3][1] + m[3][0] * m[0][1] * m[2][2] - m[3][0] * m[0][2] * m[2][1];
        result[0][2] = m[0][1] * m[1][2] * m[3][3] - m[0][1] * m[1][3] * m[3][2] - m[1][1] * m[0][2] * m[3][3] + m[1][1] * m[0][3] * m[3][2] + m[3][1] * m[0][2] * m[1][3] - m[3][1] * m[0][3] * m[1][2];
        result[1][2] = -m[0][0] * m[1][2] * m[3][3] + m[0][0] * m[1][3] * m[3][2] + m[1][0] * m[0][2] * m[3][3] - m[1][0] * m[0][3] * m[3][2] - m[3][0] * m[0][2] * m[1][3] + m[3][0] * m[0][3] * m[1][2];
        result[2][2] = m[0][0] * m[1][1] * m[3][3] - m[0][0] * m[1][3] * m[3][1] - m[1][0] * m[0][1] * m[3][3] + m[1][0] * m[0][3] * m[3][1] + m[3][0] * m[0][1] * m[1][3] - m[3][0] * m[0][3] * m[1][1];
        result[3][2] = -m[0][0] * m[1][1] * m[3][2] + m[0][0] * m[1][2] * m[3][1] + m[1][0] * m[0][1] * m[3][2] - m[1][0] * m[0][2] * m[3][1] - m[3][0] * m[0][1] * m[1][2] + m[3][0] * m[0][2] * m[1][1];
        result[0][3] = -m[0][1] * m[1][2] * m[2][3] + m[0][1] * m[1][3] * m[2][2] + m[1][1] * m[0][2] * m[2][3] - m[1][1] * m[0][3] * m[2][2] - m[2][1] * m[0][2] * m[1][3] + m[2][1] * m[0][3] * m[1][2];
        result[1][3] = m[0][0] * m[1][2] * m[2][3] - m[0][0] * m[1][3] * m[2][2] - m[1][0] * m[0][2] * m[2][3] + m[1][0] * m[0][3] * m[2][2] + m[2][0] * m[0][2] * m[1][3] - m[2][0] * m[0][3] * m[1][2];
        result[2][3] = -m[0][0] * m[1][1] * m[2][3] + m[0][0] * m[1][3] * m[2][1] + m[1][0] * m[0][1] * m[2][3] - m[1][0] * m[0][3] * m[2][1] - m[2][0] * m[0][1] * m[1][3] + m[2][0] * m[0][3] * m[1][1];
        result[3][3] = m[0][0] * m[1][1] * m[2][2] - m[0][0] * m[1][2] * m[2][1] - m[1][0] * m[0][1] * m[2][2] + m[1][0] * m[0][2] * m[2][1] + m[2][0] * m[0][1] * m[1][2] - m[2][0] * m[0][2] * m[1][1];

        result = result * (1.0f / det);
        return result;
    }

    bool Matrix::operator==(const Matrix& b) const {
        //Matrix m = Matrix();
        Matrix m = *this;
        for (int r = 0; r < 4; r++) {
            for (int c = 0; c < 4; c++) {
                if (m[r][c] != b[r][c]) {
                    return false;

                }
            }
        }

        return true;
    }

    bool Matrix::operator!=(const Matrix& b) const {
        //Matrix m = Matrix();
        Matrix m = *this;
        for (int r = 0; r < 4; r++) {
            for (int c = 0; c < 4; c++) {
                if (m[r][c] != b[r][c]) {
                    return true;

                }
            }
        }

        return false;
    }

    Matrix product(const Matrix& a, const Matrix& b) {
        Matrix prod = Matrix();
        prod[0][0] = a[0][0] * b[0][0] + a[0][1] * b[1][0] + a[0][2] * b[2][0] + a[0][3] * b[3][0];
        prod[0][1] = a[0][0] * b[0][1] + a[0][1] * b[1][1] + a[0][2] * b[2][1] + a[0][3] * b[3][1];
        prod[0][2] = a[0][0] * b[0][2] + a[0][1] * b[1][2] + a[0][2] * b[2][2] + a[0][3] * b[3][2];
        prod[0][3] = a[0][0] * b[0][3] + a[0][1] * b[1][3] + a[0][2] * b[2][3] + a[0][3] * b[3][3];



        prod[1][0] = a[1][0] * b[0][0] + a[1][1] * b[1][0] + a[1][2] * b[2][0] + a[1][3] * b[3][0];
        prod[1][1] = a[1][0] * b[0][1] + a[1][1] * b[1][1] + a[1][2] * b[2][1] + a[1][3] * b[3][1];
        prod[1][2] = a[1][0] * b[0][2] + a[1][1] * b[1][2] + a[1][2] * b[2][2] + a[1][3] * b[3][2];
        prod[1][3] = a[1][0] * b[0][3] + a[1][1] * b[1][3] + a[1][2] * b[2][3] + a[1][3] * b[3][3];



        prod[2][0] = a[2][0] * b[0][0] + a[2][1] * b[1][0] + a[2][2] * b[2][0] + a[2][3] * b[3][0];
        prod[2][1] = a[2][0] * b[0][1] + a[2][1] * b[1][1] + a[2][2] * b[2][1] + a[2][3] * b[3][1];
        prod[2][2] = a[2][0] * b[0][2] + a[2][1] * b[1][2] + a[2][2] * b[2][2] + a[2][3] * b[3][2];
        prod[2][3] = a[2][0] * b[0][3] + a[2][1] * b[1][3] + a[2][2] * b[2][3] + a[2][3] * b[3][3];



        prod[3][0] = a[3][0] * b[0][0] + a[3][1] * b[1][0] + a[3][2] * b[2][0] + a[3][3] * b[3][0];
        prod[3][1] = a[3][0] * b[0][1] + a[3][1] * b[1][1] + a[3][2] * b[2][1] + a[3][3] * b[3][1];
        prod[3][2] = a[3][0] * b[0][2] + a[3][1] * b[1][2] + a[3][2] * b[2][2] + a[3][3] * b[3][2];
        prod[3][3] = a[3][0] * b[0][3] + a[3][1] * b[1][3] + a[3][2] * b[2][3] + a[3][3] * b[3][3];



        return prod;
    }

    Matrix operator*(const Matrix& a, float scalar) {
        Float4 r1 = scalar * a[0];
        Float4 r2 = scalar * a[1];
        Float4 r3 = scalar * a[2];
        Float4 r4 = scalar * a[3];
        return (Matrix(r1, r2, r3, r4));
    }

    Matrix operator*(float scalar, const Matrix& a) {
        Float4 r1 = scalar * a[0];
        Float4 r2 = scalar * a[1];
        Float4 r3 = scalar * a[2];
        Float4 r4 = scalar * a[3];
        return (Matrix(r1, r2, r3, r4));
    }

    Float4 Matrix::operator*(const Float4& b) const {
        float f1 = row1[0] * b[0] + row1[1] * b[1] + row1[2] * b[2] + row1[3] * b[3];
        float f2 = row2[0] * b[0] + row2[1] * b[1] + row2[2] * b[2] + row2[3] * b[3];
        float f3 = row3[0] * b[0] + row3[1] * b[1] + row3[2] * b[2] + row3[3] * b[3];
        float f4 = row4[0] * b[0] + row4[1] * b[1] + row4[2] * b[2] + row4[3] * b[3];
        return (Float4(f1, f2, f3, f4));
    }

    Vector Matrix::operator*(const Vector& b) const {
        float f1 = row1[0] * b.x + row1[1] * b.y + row1[2] * b.z;
        float f2 = row2[0] * b.x + row2[1] * b.y + row2[2] * b.z;
        float f3 = row3[0] * b.x + row3[1] * b.y + row3[2] * b.z;
        return (Vector(f1, f2, f3));
    }

    Point Matrix::operator*(const Point& b) const {
        float p1 = row1[0] * b.x + row1[1] * b.y + row1[2] * b.z + row1[3];
        float p2 = row2[0] * b.x + row2[1] * b.y + row2[2] * b.z + row2[3];
        float p3 = row3[0] * b.x + row3[1] * b.y + row3[2] * b.z + row3[3];
        float p4 = row4[0] * b.x + row4[1] * b.y + row4[2] * b.z + row4[3];
        return (Point(p1 / p4, p2 / p4, p3 / p4)); // normalize cords.
    }

    float Matrix::det() const {
        Matrix result;
        const Matrix& m = *this;
        //copied from the inverse function above.
        result[0][0] = m[1][1] * m[2][2] * m[3][3] - m[1][1] * m[2][3] * m[3][2] - m[2][1] * m[1][2] * m[3][3] + m[2][1] * m[1][3] * m[3][2] + m[3][1] * m[1][2] * m[2][3] - m[3][1] * m[1][3] * m[2][2];
        result[1][0] = -m[1][0] * m[2][2] * m[3][3] + m[1][0] * m[2][3] * m[3][2] + m[2][0] * m[1][2] * m[3][3] - m[2][0] * m[1][3] * m[3][2] - m[3][0] * m[1][2] * m[2][3] + m[3][0] * m[1][3] * m[2][2];
        result[2][0] = m[1][0] * m[2][1] * m[3][3] - m[1][0] * m[2][3] * m[3][1] - m[2][0] * m[1][1] * m[3][3] + m[2][0] * m[1][3] * m[3][1] + m[3][0] * m[1][1] * m[2][3] - m[3][0] * m[1][3] * m[2][1];
        result[3][0] = -m[1][0] * m[2][1] * m[3][2] + m[1][0] * m[2][2] * m[3][1] + m[2][0] * m[1][1] * m[3][2] - m[2][0] * m[1][2] * m[3][1] - m[3][0] * m[1][1] * m[2][2] + m[3][0] * m[1][2] * m[2][1];

        float det = m[0][0] * result[0][0] + m[0][1] * result[1][0] + m[0][2] * result[2][0] + m[0][3] * result[3][0];
        return det;
    }

    Matrix Matrix::zero() {
        Float4 f = Float4::rep(0.f);
        return (Matrix(f, f, f, f));
    }

    Matrix Matrix::identity() {
        Float4 r1(1.f, 0.f, 0.f, 0.f);
        Float4 r2(0.f, 1.f, 0.f, 0.f);
        Float4 r3(0.f, 0.f, 1.f, 0.f);
        Float4 r4(0.f, 0.f, 0.f, 1.f);
        return (Matrix(r1, r2, r3, r4));
    }

    Matrix Matrix::system(const Vector& e1, const Vector& e2, const Vector& e3) {
        Matrix m = {
              Float4(e1.x, e2.x, e3.x, 0.f),
              Float4(e1.y, e2.y, e3.y, 0.f),
              Float4(e1.z, e2.z, e3.z, 0.f),
              Float4(0.f , 0.f , 0.f , 1.f)
        };

        return m;
    }

}