
#include "geometryhelper.h"

bool isPointInsideOfTriangle(Vectorf3 v1, Vectorf3 v2, Vectorf3 v3, Vectorf3 point) {

    // Calculating barycentric coordinates.

    Vectorf3 c = point - v1;
    Vectorf3 e3 = v2 - v1;
    Vectorf3 e2 = v3 - v1;

    float e23 = e3 * e2;
    float e33 = e3 * e3;
    float e22 = e2 * e2;
    float ce2 = c * e2;
    float ce3 = c * e3;

    float deno = e23 * e23 - e33 * e22;

    float t = (ce3 * e23 - ce2 * e33) / deno;
    float u = (ce2 * e23 - ce3 * e22) / deno;

    if (t >= 0 && u >= 0 && (t + u) < 1) {
        return true;
    }
    else {
        return false;
    }
}

bool isRayIntersectWithPlane(Vectorf3 v1, Vectorf3 v2, Vectorf3 v3, Vectorf3 origin, Vectorf3 direction, Vectorf3 outPointOfInersection) {

    // Find a normal of triangle.
    Vectorf3 e3{ v2 - v1 };
    Vectorf3 e1{ v3 - v2 };

    Vectorf3 normal{ e3.cross(e1) / (e3.cross(e1).length()) };

    // Calculate plane direction from [0.0.0];

    float d = normal * v1;
    float r = normal * direction;

    if (r == 0.0f) {

        // Direction vector and plane normal are perpendicular and no intersection.

        outPointOfInersection = Vectorf3{ 0.0f, 0.0f, 0.0f };
        return false;
    }

    float t = -((normal * origin) + d) / -r;  // Attention that put negative sign in denominator is because of z-axis negative.

    if (t < 0.0f) {

        // Direction vector points in the opposite direction from the triangle.

        outPointOfInersection = Vectorf3{ 0.0f, 0.0f, 0.0f };
        return false;
    }

    Vectorf3 p = origin + t * direction;

    bool flgInside = isPointInsideOfTriangle(v1, v2, v3, p);

    if (flgInside == true) {

        // Conclude that ray intersect with the triangle given.

        outPointOfInersection = p;
        return true;
    }
    else {

        // Conclude that ray does not intersect with the triangle given.

        outPointOfInersection = Vectorf3{ 0.0f, 0.0f, 0.0f };
        return false;

    }
}

