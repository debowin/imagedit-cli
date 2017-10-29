//Image.h
//
//Class representing an image
//  original by Wagner Correa, 1999
//  turned to C++ by Robert Osada, 2000
//  updated by Stephen J. Guy, 2017

#ifndef IMAGE_INCLUDED
#define IMAGE_INCLUDED

#include <assert.h>
#include <stdio.h>
#include "pixel.h"


#include "stb_image.h"
#include "stb_image_write.h"

/**
 * constants
 **/
enum {
    IMAGE_SAMPLING_POINT,
    IMAGE_SAMPLING_BILINEAR,
    IMAGE_SAMPLING_GAUSSIAN,
    IMAGE_N_SAMPLING_METHODS
};

enum {
    IMAGE_CHANNEL_RED,
    IMAGE_CHANNEL_GREEN,
    IMAGE_CHANNEL_BLUE,
    IMAGE_CHANNEL_ALPHA,
    IMAGE_N_CHANNELS
};


/**
 * Image
 **/
class Image
{
public:
    union PixelData{
       Pixel *pixels;
       uint8_t *raw;
    };
    
    PixelData data;
    //PixelInfo *pixels; //pixel array
    //uint8_t *pixelData;
    int width, height, num_pixels;
    int sampling_method;
	//BMP* bmpImg;

public:
    // Creates a blank image with the given dimensions
    Image (int width, int height);

    // Copy iamage
    Image (const Image& src);

	// Make image from file
	Image(char *fname);

    // Destructor
    ~Image ();

    // Pixel access
    int ValidCoord (int x, int y)  const { return x>=0 && x<width && y>=0 && y<height; }
    Pixel& GetPixel (int x, int y) const { assert(ValidCoord(x,y));  return data.pixels[y*width + x]; }
    void SetPixel (int x, int y, Pixel p) const { assert(ValidCoord(x,y));  data.pixels[y*width + x] = p; }

    // Dimension access
    int Width     () const { return width; }
    int Height    () const { return height; }
    int NumPixels () const { return num_pixels; }

	// Make file from image
	void Write( char *fname );

    // Adds noise to an image.  The amount of noise is given by the factor
    // in the range [0.0..1.0].  0.0 adds no noise.  1.0 adds a lot of noise.
    void AddNoise (double factor);

    // Brightens the image by multiplying each pixel component by the factor.
    void Brighten (double factor);

    /**
     * Changes the contrast of an image by interpolating between the image
     * and a constant gray image with the average luminance.
     * Interpolation reduces constrast, extrapolation boosts constrast,
     * and negative factors generate inverted images.
     **/
    void ChangeContrast (double factor);

    /**
     * Changes the saturation of an image by interpolating between the
     * image and a gray level version of the image.  Interpolation
     * decreases saturation, extrapolation increases it, negative factors
     * presrve luminance but invert the hue of the input image.
     **/
    void ChangeSaturation (double factor);

    /**
     * Extracts a sub image from the image, at position (x, y), width w,
     * and height h.
     **/
    Image* Crop(int x, int y, int w, int h);

    /**
     * Extracts a channel of an image.  Leaves the specified channel
     * intact.  Sets all other ones to zero.
     **/
    void ExtractChannel(int channel);

    /**
     * Quantizes an image with "nbits" bits per channel.
     **/
    void Quantize (int nbits);

    // Converts and image to nbits per channel using random dither.
    void RandomDither(int nbits);

    // Blurs an image with an n x n Gaussian filter.
    void Blur(int n);

	// Sharpens an image by blurring with an n x n Gaussian filter and then extrapolating
    void Sharpen(int n);

    // Detects edges in an image.
    void EdgeDetect();

    /**
     * Converts an image to nbits per channel using ordered dither, with a
     * 4x4 Bayer's pattern matrix.
     **/
    void OrderedDither(int nbits);

    /**
     * Converts an image to nbits per channel using Floyd-Steinberg dither
     * with error diffusion.
     **/
    void FloydSteinbergDither(int nbits);

    // Scales an image in x by sx, and y by sy.
    Image* Scale(double sx, double sy);

    // Rotates an image by the given angle.
    Image* Rotate(double angle);

    // Warps an image using a creative filter of your choice.
    void Fun();

    // Sets the sampling method.
    void SetSamplingMethod(int method);

    // Sample image using current sampling method.
    Pixel Sample(double u, double v);
};

#endif
