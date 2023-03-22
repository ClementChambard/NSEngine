#include "math.h"

namespace math {

    template<typename T>
    void swap(T& var1, T& var2)
    {
        T temp = var1;
        var1 = var2;
        var2 = temp;
    }

    float point_distance_to_segment(glm::vec2 s1, glm::vec2 s2, glm::vec2 p)
    {
        float tempt = (p.x-s1.x)*(s2.x-s1.x)+(p.y-s1.y)*(s2.y-s1.y);
        float dist = point_distance_sq(s1.x,s1.y,s2.x,s2.y);
        float t = clamp(tempt / dist,0.f,1.f);
        
        return point_distance(p.x,p.y,s1.x+(s2.x-s1.x)*t,s1.y+(s2.y-s1.y)*t);
    }

    bool segment_intersect(glm::vec2 a1, glm::vec2 a2, glm::vec2 b1, glm::vec2 b2)
    {
        // Find the four orientations needed for general and 
        // special cases 
        int o1 = sign((b1.y - a1.y) * (a2.x - b1.x) - (b1.x - a1.x) * (a2.y - b1.y));
        int o2 = sign((b2.y - a1.y) * (a2.x - b2.x) - (b2.x - a1.x) * (a2.y - b2.y));
        int o3 = sign((a1.y - b1.y) * (b2.x - a1.x) - (a1.x - b1.x) * (b2.y - a1.y));
        int o4 = sign((a2.y - b1.y) * (b2.x - a2.x) - (a2.x - b1.x) * (b2.y - a2.y));
      
        // General case 
        if (o1 != o2 && o3 != o4) 
            return true; 
  
        // Special Cases 
        // p1, q1 and p2 are colinear and p2 lies on segment p1q1 
        if (o1 == 0 && point_distance_to_segment(a1,a2,b1)==0) return true; 
      
        // p1, q1 and q2 are colinear and q2 lies on segment p1q1 
        if (o2 == 0 && point_distance_to_segment(a1,a2,b1)==0) return true; 
      
        // p2, q2 and p1 are colinear and p1 lies on segment p2q2 
        if (o3 == 0 && point_distance_to_segment(b1,b2,a1)==0) return true; 
      
         // p2, q2 and q1 are colinear and q1 lies on segment p2q2 
        if (o4 == 0 && point_distance_to_segment(b1,b2,a1)==0) return true; 
      
        return false; // Doesn't fall in any of the above cases 	
    }

    float segment_distance_x(glm::vec2 a1, glm::vec2 a2, glm::vec2 b1, glm::vec2 b2, bool min)
    {
        if (a1.y > a2.y)
            swap(a1,a2);
        if (b1.y > b2.y)
            swap(b1,b2);
        
        if (a1.y > b2.y || b1.y > a2.y) return 0;

        float disttop = 0;
        float distbottom = 0;
        
        if (b1.y < a1.y) disttop = b1.x+(b2.x-b1.x)*(a1.y-b1.y)/(b2.y-b1.y)-a1.x;
        else disttop = b1.x - (a1.x+(a2.x-a1.x)*(b1.y-a1.y)/(a2.y-a1.y));
        if (b2.y < a2.y) distbottom = b2.x - (a1.x+(a2.x-a1.x)*(b2.y-a1.y)/(a2.y-a1.y));
        else distbottom = b1.x+(b2.x-b1.x)*(a2.y-b1.y)/(b2.y-b1.y)-a2.x;
        
        return min?fmin(disttop,distbottom):fmax(disttop,distbottom);
    }

    float segment_distance_y(glm::vec2 a1, glm::vec2 a2, glm::vec2 b1, glm::vec2 b2, bool min)
    {
        if (a1.x > a2.x)
            swap(a1,a2);
        if (b1.x > b2.x)
            swap(b1,b2);

        if (a1.x > b2.x || b1.x > a2.x) return 0;

        float distleft = 0;
        float distright = 0;
        
        if (b1.x < a1.x) distleft = b1.y+(b2.y-b1.y)*(a1.x-b1.x)/(b2.x-b1.x)-a1.y;
        else distleft = b1.y - a1.y+(a2.y-a1.y)*(b1.x-a1.x)/(a2.x-a1.x);
        if (b2.x < a2.x) distright = b2.y - a1.y+(a2.y-a1.y)*(b2.x-a1.x)/(a2.x-a1.x);
        else distright = b1.y+(b2.y-b1.y)*(a2.x-b1.x)/(b2.x-b1.x)-a2.y;
        
        return min?fmin(distleft,distright):fmax(distleft,distright);
    }

    float point_direction(float x1, float y1, float x2, float y2)
    {
        if (x1 == x2 && y1 == y2) return 0;
        float a = atan((float)(y2-y1) / (float)(x2 - x1));
        if ((x2-x1) < 0) a -= PI;
        if (a < -PI) a+=2*PI;
        return a;
    }

    float point_distance_sq(float x1, float y1, float x2, float y2)
    {
        float a = (x2-x1), b = (y2-y1);
        return a*a+b*b;
    }

    float point_distance_sq(float x1, float y1, float z1, float x2, float y2, float z2)
    {
        float a = (x2-x1), b = (y2-y1), c = (z2-z1);
        return a*a+b*b+c*c;
    }

    float point_distance(float x1, float y1, float x2, float y2)
    { return sqrt(point_distance_sq(x1,y1,x2,y2)); }
    float point_distance(float x1, float y1, float z1, float x2, float y2, float z2)
    { return sqrt(point_distance_sq(x1,y1,z1,x2,y2,z2)); }

    float lengthdir_x(float length, float direction)
    {
        return length * cos(direction);
    }

    float lengthdir_y(float length, float direction)
    {
        return length * sin(direction);
    }

    glm::vec2 lengthdir_vec(float length, float direction)
    {
        return glm::vec2(lengthdir_x(length, direction), lengthdir_y(length, direction));
    }

    bool point_in_rectangle(float x, float y, float x1, float y1, float x2, float y2)
    {
        if (x2 < x1) swap(x1,x2);
        if (y2 < y1) swap(y1,y2);
        return (x <= x2 && x >= x1 && y <= y2 && y >= y1);
    }

    bool rectangle_intersect(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, bool equals)
    {
        if (x2 < x1) swap(x1,x2);
        if (y2 < y1) swap(y1,y2);
        if (x4 < x3) swap(x3,x4);
        if (y4 < y3) swap(y3,y4);
        if (equals)
        {
            if (x1 >= x4 || x2 <= x3 || y1 >= y4 || y2 <= y3) return false;
            return true;
        }
        else
        {
            if (x1 > x4 || x2 < x3 || y1 > y4 || y2 < y3) return false;
            return true;
        }
    }

    float warp(float val, float min, float max)
    {
        while (val < min) val += (max-min);
        while (val >= max) val += (min-max);
        return val;
    }
}
