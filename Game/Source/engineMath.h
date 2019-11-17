// engineMath.h
// Math library mostly for vectors, matrices, and quaternions

#pragma once


namespace Math
{
    const float Pi = 3.1415926535f;
    const float TwoPi = Pi * 2.0f;
    const float PiOver2 = Pi / 2.0f;

    inline float ToRadians(float degrees)
    {
        return degrees * Pi / 180.0f;
    }

    inline float ToDegrees(float radians)
    {
        return radians * 180.0f / Pi;
    }

    template <typename T>
    T Max(const T& a, const T& b)
    {
        return (a < b ? b : a);
    }

    template <typename T>
    T Min(const T& a, const T& b)
    {
        return (a < b ? a : b);
    }

    template <typename T>
    T Clamp(const T& value, const T& lower, const T& upper)
    {
        return Min(upper, Max(lower, value));
    }

    inline float Lerp(float a, float b, float f)
    {
        return a + f * (b - a);
    }

    //----------------------------------------------------------------------------------------
    // returns true if val is "close enough" to be considered the zero
    //----------------------------------------------------------------------------------------
    inline bool IsZero(float val, float epsilon = 0.001f)
    {
        if (fabs(val) <= epsilon)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    //----------------------------------------------------------------------------------------
    // returns true if a and b are "close enough" to be considered the same value
    //----------------------------------------------------------------------------------------
    inline bool IsCloseEnuf(float a, float b, float epsilon = 0.001f)
    {
        return fabsf(a - b) <= epsilon * Max(Max(1.0f, fabsf(a)), fabsf(b));
    }
}   //namespace Math

class Quaternion;

//------------------------------------------------------------------------------------
// 2D Vector
//------------------------------------------------------------------------------------
class Vector2
{
public:
    float x;
    float y;

    Vector2()
        : x(0.0f)
        , y(0.0f)
    {}

    explicit Vector2(float inX, float inY)
        : x(inX)
        , y(inY)
    {}

    // Set both components in one line
    void Set(float inX, float inY)
    {
        x = inX;
        y = inY;
    }

    // Vector addition (a + b)
    friend Vector2 operator+(const Vector2& a, const Vector2& b)
    {
        return Vector2(a.x + b.x, a.y + b.y);
    }

    // Vector +=
    Vector2& operator+=(const Vector2& right)
    {
        x += right.x;
        y += right.y;
        return *this;
    }

    // Vector subtraction (a - b)
    friend Vector2 operator-(const Vector2& a, const Vector2& b)
    {
        return Vector2(a.x - b.x, a.y - b.y);
    }

    // Vector -=
    Vector2& operator-=(const Vector2& right)
    {
        x -= right.x;
        y -= right.y;
        return *this;
    }

    // Component-wise multiplication
    // (a.x * b.x, ...)
    friend Vector2 operator*(const Vector2& a, const Vector2& b)
    {
        return Vector2(a.x * b.x, a.y * b.y);
    }

    // Scalar multiplication
    friend Vector2 operator*(const Vector2& vec, float scalar)
    {
        return Vector2(vec.x * scalar, vec.y * scalar);
    }

    // Scalar multiplication
    friend Vector2 operator*(float scalar, const Vector2& vec)
    {
        return Vector2(vec.x * scalar, vec.y * scalar);
    }

    // Scalar *=
    Vector2& operator*=(float scalar)
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    // Scalar division
    friend Vector2 operator/(const Vector2& vec, float scalar)
    {
        return Vector2(vec.x / scalar, vec.y / scalar);
    }

    // Scalar /=
    Vector2& operator/=(float scalar)
    {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    // Length squared of vector
    float LengthSq() const
    {
        return (x*x + y*y);
    }

    // Length of vector
    float Length() const
    {
        return (sqrtf(LengthSq()));
    }

    // Normalize this vector
    void Normalize()
    {
        float length = Length();
        x /= length;
        y /= length;
    }

    // Normalize the provided vector
    static Vector2 Normalize(const Vector2& vec)
    {
        Vector2 temp = vec;
        temp.Normalize();
        return temp;
    }

    // Dot product between two vectors (a dot b)
    static float Dot(const Vector2& a, const Vector2& b)
    {
		return (a.x * b.x + a.y * b.y);
    }

    // Lerp from A to B by f
    static Vector2 Lerp(const Vector2& a, const Vector2& b, float f)
    {
		return Vector2(a + f * (b - a));
    }

    static const Vector2 Zero;
    static const Vector2 One;
    static const Vector2 UnitX;
    static const Vector2 UnitY;

    static bool UnitTest();
};

// 3D Vector
class Vector3
{
public:
    float x;
    float y;
    float z;

    Vector3()
        : x(0.0f)
        , y(0.0f)
        , z(0.0f)
    {}

    explicit Vector3(float inX, float inY, float inZ)
        : x(inX)
        , y(inY)
        , z(inZ)
    {}

    explicit Vector3(__m128 value)
    {
        memcpy(this, &value, sizeof(float) * 3);
    }

    // Set all three components in one line
    void Set(float inX, float inY, float inZ)
    {
        x = inX;
        y = inY;
        z = inZ;
    }

    // Vector addition (a + b)
    friend Vector3 operator+(const Vector3& a, const Vector3& b)
    {
        return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
    }

    // Vector subtraction (a - b)
    friend Vector3 operator-(const Vector3& a, const Vector3& b)
    {
        return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
    }

    // Component-wise multiplication
    friend Vector3 operator*(const Vector3& left, const Vector3& right)
    {
        return Vector3(left.x * right.x, left.y * right.y, left.z * right.z);
    }

    // Scalar multiplication
    friend Vector3 operator*(const Vector3& vec, float scalar)
    {
        return Vector3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
    }

    // Scalar multiplication
    friend Vector3 operator*(float scalar, const Vector3& vec)
    {
        return Vector3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
    }

    // Scalar *=
    Vector3& operator*=(float scalar)
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    // Scalar division
    friend Vector3 operator/(const Vector3& vec, float scalar)
    {
        return Vector3(vec.x / scalar, vec.y / scalar, vec.z / scalar);
    }

    // Scalar /=
    Vector3& operator/=(float scalar)
    {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }

    // Vector +=
    Vector3& operator+=(const Vector3& right)
    {
        x += right.x;
        y += right.y;
        z += right.z;
        return *this;
    }

	// Vector -=
	Vector3& operator-=(const Vector3& right)
	{
		x -= right.x;
		y -= right.y;
		z -= right.z;
		return *this;
	}

	// Vector ==
	bool operator==(const Vector3& right)
	{
		return (x == right.x && y == right.y && z == right.z);
	}


    // Length squared of vector
    float LengthSq() const
    {
        return (x*x + y*y + z*z);
    }

    // Length of vector
    float Length() const
    {
        return (sqrtf(LengthSq()));
    }

    // Normalize this vector
    void Normalize()
    {
		float len = Length();
		x /= len;
		y /= len;
		z /= len;
    }

    // Normalize the provided vector
    friend Vector3 Normalize(const Vector3& vec)
    {
		Vector3 veccopy = vec;
		veccopy.Normalize();
		return veccopy;
    }

    // Dot product between two vectors (a dot b)
    friend float Dot(const Vector3& a, const Vector3& b)
    {
		return (a.x * b.x + a.y * b.y + a.z * b.z);
    }

    // Cross product between two vectors (a cross b)
    friend Vector3 Cross(const Vector3& a, const Vector3& b)
    {
		float x = a.y * b.z - a.z * b.y;
		float y = a.z * b.x - a.x * b.z;
		float z = a.x * b.y - a.y * b.x;
		return Vector3(x, y, z);
    }

    // Lerp from A to B by f
    friend Vector3 Lerp(const Vector3& a, const Vector3& b, float f)
    {
		return Vector3(a + f * (b - a));
    }

    static const Vector3 Zero;
    static const Vector3 One;
    static const Vector3 UnitX;
    static const Vector3 UnitY;
    static const Vector3 UnitZ;

    static bool UnitTest();
};

// 4D Vector
class Vector4
{
public:
    float x;
    float y;
    float z;
    float w;

    Vector4()
        : x(0.0f)
        , y(0.0f)
        , z(0.0f)
        , w(0.0f)
    {}

    explicit Vector4(float inX, float inY, float inZ, float inW)
        : x(inX)
        , y(inY)
        , z(inZ)
        , w(inW)
    {}

    explicit Vector4(__m128 value)
    {
        memcpy(this, &value, sizeof(float) * 4);
    }

    // Set all four components in one line
    void Set(float inX, float inY, float inZ, float inW)
    {
        x = inX;
        y = inY;
        z = inZ;
        w = inW;
    }

    // Vector addition (a + b)
    friend Vector4 operator+(const Vector4& a, const Vector4& b)
    {
        return Vector4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
    }

    // Vector subtraction (a - b)
    friend Vector4 operator-(const Vector4& a, const Vector4& b)
    {
        return Vector4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
    }

    // Component-wise multiplication
    friend Vector4 operator*(const Vector4& left, const Vector4& right)
    {
        return Vector4(left.x * right.x, left.y * right.y, left.z * right.z, left.w * right.w);
    }

    // Scalar multiplication
    friend Vector4 operator*(const Vector4& vec, float scalar)
    {
        return Vector4(vec.x * scalar, vec.y * scalar, vec.z * scalar, vec.w * scalar);
    }

    // Scalar multiplication
    friend Vector4 operator*(float scalar, const Vector4& vec)
    {
        return Vector4(vec.x * scalar, vec.y * scalar, vec.z * scalar, vec.w * scalar);
    }

    // Scalar *=
    Vector4& operator*=(float scalar)
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        w *= scalar;
        return *this;
    }

    // Scalar division
    friend Vector4 operator/(const Vector4& vec, float scalar)
    {
        return Vector4(vec.x / scalar, vec.y / scalar, vec.z / scalar, vec.w / scalar);
    }

    // Scalar /=
    Vector4& operator/=(float scalar)
    {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        w /= scalar;
        return *this;
    }

    // Vector +=
    Vector4& operator+=(const Vector4& right)
    {
        x += right.x;
        y += right.y;
        z += right.z;
        w += right.w;
        return *this;
    }

    // Vector -=
    Vector4& operator-=(const Vector4& right)
    {
        x -= right.x;
        y -= right.y;
        z -= right.z;
        w -= right.w;
        return *this;
    }

    // Length squared of vector
    float LengthSq() const
    {
        return (x*x + y*y + z*z + w*w);
    }

    // Length of vector
    float Length() const
    {
        return (sqrtf(LengthSq()));
    }

    // Normalize this vector
    void Normalize()
    {
		float len = Length();
		x /= len;
		y /= len;
		z /= len;
		w /= len;
    }

    // Normalize the provided vector
    friend Vector4 Normalize(const Vector4& vec)
    {
		Vector4 normalized = vec;
		normalized.Normalize();
		return normalized;
    }

    // Lerp from A to B by f
    friend Vector4 Lerp(const Vector4& a, const Vector4& b, float f)
    {
		return Vector4(a + f * (b - a));
    }

    static const Vector4 Zero;
    static const Vector4 One;
    static const Vector4 UnitX;
    static const Vector4 UnitY;
    static const Vector4 UnitZ;
    static const Vector4 UnitW;

    static bool UnitTest();
};

// 4x4 Matrix
class Matrix4
{
public:
    float mat[4][4];

    Matrix4()
    {
        *this = Matrix4::Identity;
    }

    explicit Matrix4(const float inMat[4][4])
    {
        memcpy(mat, inMat, 16 * sizeof(float));
    }

    explicit Matrix4(const __m128 rows[4])
    {
        memcpy(mat, rows, 16 * sizeof(float));
    }

    // Cast to a const float pointer
    const float* GetAsFloatPtr() const
    {
        return reinterpret_cast<const float*>(&mat[0][0]);
    }

    // Matrix multiplication (a * b)
    friend Matrix4 operator*(const Matrix4& a, const Matrix4& b)
    {
        Matrix4 retVal;
        // row 0
        retVal.mat[0][0] =
            a.mat[0][0] * b.mat[0][0] +
            a.mat[0][1] * b.mat[1][0] +
            a.mat[0][2] * b.mat[2][0] +
            a.mat[0][3] * b.mat[3][0];

        retVal.mat[0][1] =
            a.mat[0][0] * b.mat[0][1] +
            a.mat[0][1] * b.mat[1][1] +
            a.mat[0][2] * b.mat[2][1] +
            a.mat[0][3] * b.mat[3][1];

        retVal.mat[0][2] =
            a.mat[0][0] * b.mat[0][2] +
            a.mat[0][1] * b.mat[1][2] +
            a.mat[0][2] * b.mat[2][2] +
            a.mat[0][3] * b.mat[3][2];

        retVal.mat[0][3] =
            a.mat[0][0] * b.mat[0][3] +
            a.mat[0][1] * b.mat[1][3] +
            a.mat[0][2] * b.mat[2][3] +
            a.mat[0][3] * b.mat[3][3];

        // row 1
        retVal.mat[1][0] =
            a.mat[1][0] * b.mat[0][0] +
            a.mat[1][1] * b.mat[1][0] +
            a.mat[1][2] * b.mat[2][0] +
            a.mat[1][3] * b.mat[3][0];

        retVal.mat[1][1] =
            a.mat[1][0] * b.mat[0][1] +
            a.mat[1][1] * b.mat[1][1] +
            a.mat[1][2] * b.mat[2][1] +
            a.mat[1][3] * b.mat[3][1];

        retVal.mat[1][2] =
            a.mat[1][0] * b.mat[0][2] +
            a.mat[1][1] * b.mat[1][2] +
            a.mat[1][2] * b.mat[2][2] +
            a.mat[1][3] * b.mat[3][2];

        retVal.mat[1][3] =
            a.mat[1][0] * b.mat[0][3] +
            a.mat[1][1] * b.mat[1][3] +
            a.mat[1][2] * b.mat[2][3] +
            a.mat[1][3] * b.mat[3][3];

        // row 2
        retVal.mat[2][0] =
            a.mat[2][0] * b.mat[0][0] +
            a.mat[2][1] * b.mat[1][0] +
            a.mat[2][2] * b.mat[2][0] +
            a.mat[2][3] * b.mat[3][0];

        retVal.mat[2][1] =
            a.mat[2][0] * b.mat[0][1] +
            a.mat[2][1] * b.mat[1][1] +
            a.mat[2][2] * b.mat[2][1] +
            a.mat[2][3] * b.mat[3][1];

        retVal.mat[2][2] =
            a.mat[2][0] * b.mat[0][2] +
            a.mat[2][1] * b.mat[1][2] +
            a.mat[2][2] * b.mat[2][2] +
            a.mat[2][3] * b.mat[3][2];

        retVal.mat[2][3] =
            a.mat[2][0] * b.mat[0][3] +
            a.mat[2][1] * b.mat[1][3] +
            a.mat[2][2] * b.mat[2][3] +
            a.mat[2][3] * b.mat[3][3];

        // row 3
        retVal.mat[3][0] =
            a.mat[3][0] * b.mat[0][0] +
            a.mat[3][1] * b.mat[1][0] +
            a.mat[3][2] * b.mat[2][0] +
            a.mat[3][3] * b.mat[3][0];

        retVal.mat[3][1] =
            a.mat[3][0] * b.mat[0][1] +
            a.mat[3][1] * b.mat[1][1] +
            a.mat[3][2] * b.mat[2][1] +
            a.mat[3][3] * b.mat[3][1];

        retVal.mat[3][2] =
            a.mat[3][0] * b.mat[0][2] +
            a.mat[3][1] * b.mat[1][2] +
            a.mat[3][2] * b.mat[2][2] +
            a.mat[3][3] * b.mat[3][2];

        retVal.mat[3][3] =
            a.mat[3][0] * b.mat[0][3] +
            a.mat[3][1] * b.mat[1][3] +
            a.mat[3][2] * b.mat[2][3] +
            a.mat[3][3] * b.mat[3][3];

        return retVal;
    }

    Matrix4& operator*=(const Matrix4& right)
    {
        *this = *this * right;
        return *this;
    }

    // Invert the matrix - super slow
    void Invert();

    // Get the translation component of the matrix
    Vector3 GetTranslation() const
    {
		return Vector3(mat[3][0], mat[3][1], mat[3][2]);
    }

    // Get the X axis of the matrix (forward)
    Vector3 GetXAxis() const
    {
		return Normalize(Vector3(mat[0][0], mat[0][1], mat[0][2]));
    }

    // Get the Y axis of the matrix (left)
    Vector3 GetYAxis() const
    {
		return Normalize(Vector3(mat[1][0], mat[1][1], mat[1][2]));
    }

    // Get the Z axis of the matrix (up)
    Vector3 GetZAxis() const
    {
		return Normalize(Vector3(mat[2][0], mat[2][1], mat[2][2]));
    }

    // Extract the scale component from the matrix
    Vector3 GetScale() const
    {
        Vector3 retVal;
        retVal.x = Vector3(mat[0][0], mat[0][1], mat[0][2]).Length();
        retVal.y = Vector3(mat[1][0], mat[1][1], mat[1][2]).Length();
        retVal.z = Vector3(mat[2][0], mat[2][1], mat[2][2]).Length();
        return retVal;
    }

    // Transpose this matrix
    void Transpose()
    {
        float temp[4][4];

        temp[0][0] = mat[0][0];
        temp[0][1] = mat[1][0];
        temp[0][2] = mat[2][0];
        temp[0][3] = mat[3][0];

        temp[1][0] = mat[0][1];
        temp[1][1] = mat[1][1];
        temp[1][2] = mat[2][1];
        temp[1][3] = mat[3][1];

        temp[2][0] = mat[0][2];
        temp[2][1] = mat[1][2];
        temp[2][2] = mat[2][2];
        temp[2][3] = mat[3][2];

        temp[3][0] = mat[0][3];
        temp[3][1] = mat[1][3];
        temp[3][2] = mat[2][3];
        temp[3][3] = mat[3][3];

        memcpy(mat, temp, 16 * sizeof(float));
    }

    // Transpose the provided matrix
    friend Matrix4 Transpose(const Matrix4& inMat)
    {
        Matrix4 retVal = inMat;
        retVal.Transpose();
        return retVal;
    }

    // Create a scale matrix with x, y, and z scales
    static Matrix4 CreateScale(float xScale, float yScale, float zScale)
    {
		float temp[4][4] =
        {
            { xScale, 0.0f, 0.0f, 0.0f },
            { 0.0f, yScale, 0.0f, 0.0f },
            { 0.0f, 0.0f, zScale, 0.0f },
            { 0.0f, 0.0f, 0.0f, 1.0f }
        };
        return Matrix4(temp);
    }

    static Matrix4 CreateScale(const Vector3& scaleVector)
    {
        return CreateScale(scaleVector.x, scaleVector.y, scaleVector.z);
    }

    // Create a scale matrix with a uniform factor
    static Matrix4 CreateScale(float scale)
    {
        return CreateScale(scale, scale, scale);
    }

    // Rotation about x-axis
    static Matrix4 CreateRotationX(float theta)
    {
		float temp[4][4] =
		{
			{ 1.0f, 0.0f, 0.0f , 0.0f },
			{ 0.0f, cosf(theta), sinf(theta), 0.0f },
			{ 0.0f, -sinf(theta), cosf(theta), 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f },
		};
        return Matrix4(temp);
    }

    // Rotation about y-axis
    static Matrix4 CreateRotationY(float theta)
    {
		float temp[4][4] =
		{
			{ cosf(theta), 0.0f, -sinf(theta), 0.0f },
			{ 0.0f, 1.0f, 0.0f, 0.0f },
			{ sinf(theta), 0.0f, cosf(theta), 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f },
		};
        return Matrix4(temp);
    }

    // Rotation about z-axis
    static Matrix4 CreateRotationZ(float theta)
    {
		float temp[4][4] =
        {
            { cosf(theta), sinf(theta), 0.0f, 0.0f },
            { -sinf(theta), cosf(theta), 0.0f, 0.0f },
            { 0.0f, 0.0f, 1.0f, 0.0f },
            { 0.0f, 0.0f, 0.0f, 1.0f },
        };
        return Matrix4(temp);
    }

    static Matrix4 CreateYawPitchRoll(float yaw, float pitch, float roll)
    {
        return Matrix4::CreateRotationZ(roll)
            * Matrix4::CreateRotationX(pitch)
            * Matrix4::CreateRotationY(yaw);
    }

    // Create a rotation matrix from a quaternion
    static Matrix4 CreateFromQuaternion(const Quaternion& q);

    static Matrix4 CreateTranslation(const Vector3& trans)
    {
		float temp[4][4] =
        {
            { 1.0f, 0.0f, 0.0f, 0.0f },
            { 0.0f, 1.0f, 0.0f, 0.0f },
            { 0.0f, 0.0f, 1.0f, 0.0f },
            { trans.x, trans.y, trans.z, 1.0f },
        };
        return Matrix4(temp);
    }

    static Matrix4 CreateLookAt(const Vector3& eye, const Vector3& at, const Vector3& up)
    {
        Vector3 forward = Normalize(at - eye);
        Vector3 left = Normalize(Cross(up, forward));
        Vector3 newUp = Normalize(Cross(forward, left));

        float temp[4][4] =
        {
            { left.x, left.y, left.z, 0.0f },
            { newUp.x, newUp.y, newUp.z, 0.0f },
            { forward.x, forward.y, forward.z, 0.0f },
            { eye.x, eye.y, eye.z, 1.0f }
        };
        return Matrix4(temp);
    }

    static Matrix4 CreateOrtho(float width, float height, float nearZ, float farZ)
    {
        float temp[4][4] =
        {
            { 2.0f / width, 0.0f, 0.0f, 0.0f },
            { 0.0f, 2.0f / height, 0.0f, 0.0f },
            { 0.0f, 0.0f, 1.0f / (farZ - nearZ), 0.0f },
            { 0.0f, 0.0f, nearZ / (nearZ - farZ), 1.0f }
        };
        return Matrix4(temp);
    }

    static Matrix4 CreatePerspectiveFOV(float fovY, float width, float height, float nearZ, float farZ)
    {
        float yScale = 1.0f / tanf(fovY / 2.0f);
        float xScale = yScale * height / width;
        float temp[4][4] =
        {
            { xScale, 0.0f, 0.0f, 0.0f },
            { 0.0f, yScale, 0.0f, 0.0f },
            { 0.0f, 0.0f, farZ / (farZ - nearZ), 1.0f },
            { 0.0f, 0.0f, -nearZ * farZ / (farZ - nearZ), 0.0f }
        };
        return Matrix4(temp);
    }

    static const Matrix4 Identity;

    static bool UnitTest();
};

// 3x3 Matrix
class Matrix3
{
public:
    float mat[3][3];

    Matrix3()
    {
        *this = Matrix3::Identity;
    }

    explicit Matrix3(const float inMat[3][3])
    {
        memcpy(mat, inMat, 9 * sizeof(float));
    }

    // Cast to a const float pointer
    const float* GetAsFloatPtr() const
    {
        return reinterpret_cast<const float*>(&mat[0][0]);
    }

    // Matrix multiplication
    friend Matrix3 operator*(const Matrix3& left, const Matrix3& right)
    {
        Matrix3 retVal;
        // row 0
        retVal.mat[0][0] =
            left.mat[0][0] * right.mat[0][0] +
            left.mat[0][1] * right.mat[1][0] +
            left.mat[0][2] * right.mat[2][0];

        retVal.mat[0][1] =
            left.mat[0][0] * right.mat[0][1] +
            left.mat[0][1] * right.mat[1][1] +
            left.mat[0][2] * right.mat[2][1];

        retVal.mat[0][2] =
            left.mat[0][0] * right.mat[0][2] +
            left.mat[0][1] * right.mat[1][2] +
            left.mat[0][2] * right.mat[2][2];

        // row 1
        retVal.mat[1][0] =
            left.mat[1][0] * right.mat[0][0] +
            left.mat[1][1] * right.mat[1][0] +
            left.mat[1][2] * right.mat[2][0];

        retVal.mat[1][1] =
            left.mat[1][0] * right.mat[0][1] +
            left.mat[1][1] * right.mat[1][1] +
            left.mat[1][2] * right.mat[2][1];

        retVal.mat[1][2] =
            left.mat[1][0] * right.mat[0][2] +
            left.mat[1][1] * right.mat[1][2] +
            left.mat[1][2] * right.mat[2][2];

        // row 2
        retVal.mat[2][0] =
            left.mat[2][0] * right.mat[0][0] +
            left.mat[2][1] * right.mat[1][0] +
            left.mat[2][2] * right.mat[2][0];

        retVal.mat[2][1] =
            left.mat[2][0] * right.mat[0][1] +
            left.mat[2][1] * right.mat[1][1] +
            left.mat[2][2] * right.mat[2][1];

        retVal.mat[2][2] =
            left.mat[2][0] * right.mat[0][2] +
            left.mat[2][1] * right.mat[1][2] +
            left.mat[2][2] * right.mat[2][2];

        return retVal;
    }

    Matrix3& operator*=(const Matrix3& right)
    {
        *this = *this * right;
        return *this;
    }

    void Transpose()
    {
        float temp[3][3];

        temp[0][0] = mat[0][0];
        temp[0][1] = mat[1][0];
        temp[0][2] = mat[2][0];

        temp[1][0] = mat[0][1];
        temp[1][1] = mat[1][1];
        temp[1][2] = mat[2][1];

        temp[2][0] = mat[0][2];
        temp[2][1] = mat[1][2];
        temp[2][2] = mat[2][2];

        memcpy(mat, temp, 9 * sizeof(float));
    }

    // Transpose the provided matrix
    friend Matrix3 Transpose(const Matrix3& inMat)
    {
        Matrix3 retVal = inMat;
        retVal.Transpose();
        return retVal;
    }

    // Create a scale matrix with x and y scales
    static Matrix3 CreateScale(float xScale, float yScale)
    {
        float temp[3][3] =
        {
            { xScale, 0.0f, 0.0f },
            { 0.0f, yScale, 0.0f },
            { 0.0f, 0.0f, 1.0f },
        };
        return Matrix3(temp);
    }

    static Matrix3 CreateScale(const Vector2& scaleVector)
    {
        return CreateScale(scaleVector.x, scaleVector.y);
    }

    // Create a scale matrix with a uniform factor
    static Matrix3 CreateScale(float scale)
    {
        return CreateScale(scale, scale);
    }

    // Create a rotation matrix about the Z axis
    // theta is in radians
    static Matrix3 CreateRotation(float theta)
    {
        float temp[3][3] =
        {
            { cosf(theta), sinf(theta), 0.0f },
            { -sinf(theta), cosf(theta), 0.0f },
            { 0.0f, 0.0f, 1.0f },
        };
        return Matrix3(temp);
    }

    // Create a translation matrix (on the xy-plane)
    static Matrix3 CreateTranslation(const Vector2& trans)
    {
        float temp[3][3] =
        {
            { 1.0f, 0.0f, 0.0f },
            { 0.0f, 1.0f, 0.0f },
            { trans.x, trans.y, 1.0f },
        };
        return Matrix3(temp);
    }

    static const Matrix3 Identity;

    static bool UnitTest();
};

// (Unit) Quaternion
class Quaternion
{
public:
    float x;
    float y;
    float z;
    float w;

    Quaternion()
    {
        *this = Quaternion::Identity;
    }

    // This directly sets the quaternion components --
    // don't use for axis/angle
    explicit Quaternion(float inX, float inY, float inZ, float inW)
    {
        Set(inX, inY, inZ, inW);
    }

    // Construct the quaternion from an axis and angle
    // It is assumed that axis is already normalized,
    // and the angle is in radians
    explicit Quaternion(const Vector3& axis, float angle)
    {
        float scalar = sinf(angle / 2.0f);
        x = axis.x * scalar;
        y = axis.y * scalar;
        z = axis.z * scalar;
        w = cosf(angle / 2.0f);
    }

    // Directly set the internal components
    void Set(float inX, float inY, float inZ, float inW)
    {
        x = inX;
        y = inY;
        z = inZ;
        w = inW;
    }

    void Conjugate()
    {
        x *= -1.0f;
        y *= -1.0f;
        z *= -1.0f;
    }

    float LengthSq() const
    {
        return (x*x + y*y + z*z + w*w);
    }

    float Length() const
    {
        return sqrtf(LengthSq());
    }

    void Normalize()
    {
        float length = Length();
        x /= length;
        y /= length;
        z /= length;
        w /= length;
    }

    // Normalize the provided quaternion
    friend Quaternion Normalize(const Quaternion& q)
    {
        Quaternion retVal = q;
        retVal.Normalize();
        return retVal;
    }

    // Linear interpolation
    friend Quaternion Lerp(const Quaternion& a, const Quaternion& b, float f)
    {
        const float dotResult = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
        float bias = -1.0f;
        if (dotResult >= 0.0f)
        {
            bias = 1.0f;
        }

        Quaternion retVal;
        retVal.x = b.x * f + a.x * bias * (1.0f - f);
        retVal.y = b.y * f + a.y * bias * (1.0f - f);
        retVal.z = b.z * f + a.z * bias * (1.0f - f);
        retVal.w = b.w * f + a.w * bias * (1.0f - f);
        retVal.Normalize();
        return retVal;
    }

    // Spherical Linear Interpolation
    friend Quaternion Slerp(const Quaternion& a, const Quaternion& b, float f);

    // Concatenate
    // Rotate by q FOLLOWED BY p
    friend Quaternion Concatenate(const Quaternion& q, const Quaternion& p)
    {
        Quaternion retVal;

        // Vector component is:
        // ps * qv + qs * pv + pv x qv
        Vector3 qv(q.x, q.y, q.z);
        Vector3 pv(p.x, p.y, p.z);
        Vector3 newVec = p.w * qv + q.w * pv + Cross(pv, qv);
        retVal.x = newVec.x;
        retVal.y = newVec.y;
        retVal.z = newVec.z;

        // Scalar component is:
        // ps * qs - pv . qv
        retVal.w = p.w * q.w - Dot(pv, qv);

        return retVal;
    }

    // Dot product between two quaternions
    friend float Dot(const Quaternion& a, const Quaternion& b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    }

    static const Quaternion Identity;

    static bool UnitTest();
};

// Inline implementations of transform functions that rely on matrices
inline Vector2 Transform(const Vector2& vec, const Matrix3& mat, float z = 0.0f)
{
    Vector2 retVal;
    retVal.x = vec.x * mat.mat[0][0] + vec.y * mat.mat[1][0] + z * mat.mat[2][0];
    retVal.y = vec.x * mat.mat[0][1] + vec.y * mat.mat[1][1] + z * mat.mat[2][1];
    //ignore z since we aren't returning a new value for it...
    return retVal;
}

inline Vector3 Transform(const Vector3& vec, const Matrix3& mat)
{
    Vector3 retVal;
    retVal.x = vec.x * mat.mat[0][0] + vec.y * mat.mat[1][0] +
        vec.z * mat.mat[2][0];
    retVal.y = vec.x * mat.mat[0][1] + vec.y * mat.mat[1][1] +
        vec.z * mat.mat[2][1];
    retVal.z = vec.x * mat.mat[0][2] + vec.y * mat.mat[1][2] +
        vec.z * mat.mat[2][2];
    return retVal;
}

inline Vector3 Transform(const Vector3& vec, const Matrix4& mat, float w = 1.0f)
{
	Vector3 retVal;
	retVal.x = vec.x * mat.mat[0][0] + vec.y * mat.mat[1][0] +
		vec.z * mat.mat[2][0] + w * mat.mat[3][0];
	retVal.y = vec.x * mat.mat[0][1] + vec.y * mat.mat[1][1] +
		vec.z * mat.mat[2][1] + w * mat.mat[3][1];
	retVal.z = vec.x * mat.mat[0][2] + vec.y * mat.mat[1][2] +
		vec.z * mat.mat[2][2] + w * mat.mat[3][2];
	return retVal;
}

inline Vector4 Transform(const Vector4& vec, const Matrix4& mat)
{
	Vector4 retVal;
	retVal.x = vec.x * mat.mat[0][0] + vec.y * mat.mat[1][0] +
		vec.z * mat.mat[2][0];
	retVal.y = vec.x * mat.mat[0][1] + vec.y * mat.mat[1][1] +
		vec.z * mat.mat[2][1];
	retVal.z = vec.x * mat.mat[0][2] + vec.y * mat.mat[1][2] +
		vec.z * mat.mat[2][2];
	retVal.w = vec.x * mat.mat[0][3] + vec.y * mat.mat[1][3] +
		vec.z * mat.mat[2][3];
	return retVal;
}

// Transform a Vector3 by a quaternion
inline Vector3 Transform(const Vector3& v, const Quaternion& q)
{
    // v + 2.0*cross(q.xyz, cross(q.xyz,v) + q.w*v);
    Vector3 qv(q.x, q.y, q.z);
    Vector3 retVal = v;
    retVal += 2.0f * Cross(qv, Cross(qv, v) + q.w * v);
    return retVal;
}
