#ifndef VEC2_H
#define VEC2_H

class vec2 {
    public:
    float x;
    float y;
    
    vec2() : x(0), y(0) {};
    vec2(float x, float y) : x(x), y(y) {};
    
    vec2(const vec2 &oth) { x = oth.x; y = oth.y; };
    vec2(vec2 &oth) { x = oth.x; y = oth.y; };
    
    vec2 operator+(const vec2 &rhs) { 
        return vec2 { x + rhs.x, y + rhs.y }; 
    }
    vec2 operator-(const vec2 &rhs) { 
        return vec2 { x - rhs.x, y - rhs.y }; 
    }
    vec2 operator*(const float m) { 
        return vec2 { x * m, y * m }; 
    }
    vec2 operator/(const float m) { 
        return vec2 { x / m, y / m }; 
    }
    vec2 operator+=(vec2 oth) { x += oth.x; y += oth.y; }
    vec2 operator-=(vec2 oth) { x -= oth.x; y -= oth.y; }
    vec2 operator*=(float m) { x *= m; y *= m; }
    vec2 operator/=(float m) { x /= m; y /= m; }
};

#endif