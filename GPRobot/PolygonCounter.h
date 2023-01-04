#pragma once
#include <Windows.h>
#pragma once
#include <gl/GL.h>

// #pragma once
// #include "PolygonCounter.h"

struct Vertex
{
    float x, y, z;

    Vertex(): x(0), y(0), z(0)
    {
    }

    Vertex(float x, float y, float z) : x(x), y(y), z(z)
    {
    }
};

struct PolygonVertices
{
    // Array of vertices
    Vertex* vertices;
    int count;

    PolygonVertices(Vertex* vertices, int count) : vertices(vertices), count(count)
    {
    }

    PolygonVertices(float x, float y, float width, float height) : count(5)
    {
        Vertex* vertices = new Vertex[5];
        vertices[0] = Vertex(x, y, 0);
        vertices[1] = Vertex(x + width, y, 0);
        vertices[2] = Vertex(x + width, y + height, 0);
        vertices[3] = Vertex(x, y + height, 0);
        vertices[4] = Vertex(x, y, 0);
        this->vertices = vertices;
    }
};

class Polygons
{
public:
    static void polygonExtender(PolygonVertices polygon, float height);
    static void polygonTextureMapping(PolygonVertices polygon, float height);
};


inline void Polygons::polygonExtender(PolygonVertices polygon, float height)
{
    // Upper face
    // defColor(RED);
    glBegin(GL_POLYGON);
    for (int i = 0; i < polygon.count; i++)
    {
        glVertex3f(polygon.vertices[i].x, polygon.vertices[i].y, height);
    }
    glVertex3f(polygon.vertices[0].x, polygon.vertices[0].y, height);
    glEnd();

    // Bottom face
    // defColor(BLUE);
    glBegin(GL_POLYGON);
    for (int i = 0; i < polygon.count; i++)
    {
        glVertex3f(polygon.vertices[i].x, polygon.vertices[i].y, polygon.vertices[i].z);
    }
    glVertex3f(polygon.vertices[0].x, polygon.vertices[0].y, polygon.vertices[0].z);
    glEnd();

    // Adjacent face
    for (int i = 0; i < polygon.count - 1; i++)
    {
        // defColor(i < 5 ? i : i - 5);
        glBegin(GL_POLYGON);
        glVertex3f(polygon.vertices[i].x, polygon.vertices[i].y, polygon.vertices[i].z);
        glVertex3f(polygon.vertices[i].x, polygon.vertices[i].y, height);
        glVertex3f(polygon.vertices[i + 1].x, polygon.vertices[i + 1].y, height);
        glVertex3f(polygon.vertices[i + 1].x, polygon.vertices[i + 1].y, polygon.vertices[i + 1].z);
        glEnd();
    }
}

inline void Polygons::polygonTextureMapping(PolygonVertices polygon, float height)
{

    // Find smallest and largest x and y value in polygon
    float smallestX = polygon.vertices[0].x;
    float largestX = polygon.vertices[0].x;
    float smallestY = polygon.vertices[0].y;
    float largestY = polygon.vertices[0].y;

    for (int i = 1; i < polygon.count; i++)
    {
        if (polygon.vertices[i].x < smallestX)
        {
            smallestX = polygon.vertices[i].x;
        }
        if (polygon.vertices[i].x > largestX)
        {
            largestX = polygon.vertices[i].x;
        }
        if (polygon.vertices[i].y < smallestY)
        {
            smallestY = polygon.vertices[i].y;
        }
        if (polygon.vertices[i].y > largestY)
        {
            largestY = polygon.vertices[i].y;
        }
    }

    float diffX = largestX - smallestX;
    float diffY = largestY - smallestY;

    // Upper face
    glBegin(GL_POLYGON);
    for (int i = 0; i < polygon.count; i++)
    {
        glTexCoord2f((polygon.vertices[i].x - smallestX) / diffX, (polygon.vertices[i].y - smallestY) / diffY);
        glVertex3f(polygon.vertices[i].x, polygon.vertices[i].y, height);
    }
    glTexCoord2f((polygon.vertices[0].x - smallestX) / diffX, (polygon.vertices[0].y - smallestY) / diffY);
    glVertex3f(polygon.vertices[0].x, polygon.vertices[0].y, height);
    glEnd();

    // Bottom face
    glBegin(GL_POLYGON);
    for (int i = 0; i < polygon.count; i++)
    {
        glTexCoord2f((polygon.vertices[i].x - smallestX) / diffX, (polygon.vertices[i].y - smallestY) / diffY);
        glVertex3f(polygon.vertices[i].x, polygon.vertices[i].y, polygon.vertices[i].z);
    }
    glVertex3f(polygon.vertices[0].x, polygon.vertices[0].y, height);
    glVertex3f(polygon.vertices[0].x, polygon.vertices[0].y, polygon.vertices[0].z);
    glEnd();

    // Adjacent face
    for (int i = 0; i < polygon.count - 1; i++)
    {
        glBegin(GL_POLYGON);
        glTexCoord2f((polygon.vertices[i].x - smallestX) / diffX, (polygon.vertices[i].y - smallestY) / diffY);
        glVertex3f(polygon.vertices[i].x, polygon.vertices[i].y, polygon.vertices[i].z);
        glTexCoord2f((polygon.vertices[i].x - smallestX) / diffX, (polygon.vertices[i].y - smallestY) / diffY);
        glVertex3f(polygon.vertices[i].x, polygon.vertices[i].y, height);
        glTexCoord2f((polygon.vertices[i + 1].x - smallestX) / diffX, (polygon.vertices[i + 1].y - smallestY) / diffY);
        glVertex3f(polygon.vertices[i + 1].x, polygon.vertices[i + 1].y, height);
        glTexCoord2f((polygon.vertices[i + 1].x - smallestX) / diffX, (polygon.vertices[i + 1].y - smallestY) / diffY);
        glVertex3f(polygon.vertices[i + 1].x, polygon.vertices[i + 1].y, polygon.vertices[i + 1].z);
        glEnd();
    }
}
