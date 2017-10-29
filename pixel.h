//Pixel.h
//
//Class representing a pixel
//  original by Wagner Correa, 1999
//  turned to C++ by Robert Osada, 2000
//  updateded by Stephen J. Guy, 2017

#ifndef PIXEL_INCLUDED
#define PIXEL_INCLUDED

#include <stdint.h>

/**
 * Component, fundamental type
 **/
typedef unsigned char Component;

// Confines a component in the range [0..255]
inline Component ComponentClamp(int i)
{ return (i<0) ? 0 : (i>255) ? 255 : i; }

// Returns a random number in the range [0..255]
Component ComponentRandom(void);

// Scales the component by the given factor
Component ComponentScale(Component c, double f);

// Linear interpolation of the components
// Returns (1 - t) * c + t * d
Component ComponentLerp(Component c, Component d, double t);



/**
 * Pixel
 **/
struct Pixel
{
    // Data
    Component r, g, b, a;

    // Constructor
    Pixel (Component r_=0, Component g_=0, Component b_=0, Component a_=255) : r(r_), g(g_), b(b_), a(a_) {}
    Pixel (uint8_t *data) : r(data[0]), g(data[1]), b(data[2]), a(data[3]) {}


    // Set
    void Set (Component  r_, Component  g_, Component  b_, Component  a_) { r=r_; g=g_; b=b_; a=a_; }
    void Set (Component  r_, Component  g_, Component  b_)                { r=r_; g=g_; b=b_; }

    void SetClamp (double r_, double g_, double b_);
    void SetClamp (double r_, double g_, double b_, double a_);

    // Returns the luminance of the pixel.
    Component Luminance ();
};

// Returns a pixel with a random value.
Pixel PixelRandom (void);

// Component-wise addition of pixels.
Pixel operator+ (const Pixel& p, const Pixel& q);

    // Component-wise multiplication of pixels.
Pixel operator* (const Pixel& p, const Pixel& q);

// Component-wise multiplication of pixel by scalar.
Pixel operator* (const Pixel& p, double f);

// Linear interpolation of pixels.  Returns (1 - t) * p + t * q.
Pixel PixelLerp (const Pixel& p, const Pixel& q, double t);

Pixel PixelQuant(const Pixel &p, int nbits);

#endif
