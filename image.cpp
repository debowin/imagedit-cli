#include "image.h"
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <cfloat>
#include <cmath>
#include <random>

double *GaussianKernel(int n) {
    auto *gaussianKernel = new double[n];
    double sum = 0;
    // construct gaussian kernel using sigma=n/2
    double sigma_squared = n * n / 4;
    for (int i = 0; i < n; i++) {
        gaussianKernel[i] = 1 / sqrt(2 * M_PI * sigma_squared) * exp(-pow(i - n / 2, 2) / (2 * sigma_squared));
        sum += gaussianKernel[i];
    }

    // normalize the kernel
    for (int i = 0; i < n; i++) {
        gaussianKernel[i] /= sum;
    }
    return gaussianKernel;
}

double GaussianValue2D(double x, double y, double sigma_squared) {
    return 1 / (2 * M_PI * sigma_squared) * exp(-(pow(x, 2) + pow(y, 2))/ (2 * sigma_squared));
}

/**
 * Image
 **/
Image::Image(int width_, int height_) {

    assert(width_ > 0);
    assert(height_ > 0);

    width = width_;
    height = height_;
    num_pixels = width * height;
    sampling_method = IMAGE_SAMPLING_POINT;

    data.raw = new uint8_t[num_pixels * 4];
    int b = 0; //which byte to write to
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            data.raw[b++] = 0;
            data.raw[b++] = 0;
            data.raw[b++] = 0;
            data.raw[b++] = 0;
        }
    }

    assert(data.raw != NULL);
}

Image::Image(const Image &src) {

    width = src.width;
    height = src.height;
    num_pixels = width * height;
    sampling_method = IMAGE_SAMPLING_POINT;

    data.raw = new uint8_t[num_pixels * 4];

    memcpy(data.raw, src.data.raw, (size_t) num_pixels * 4);
//    *data.raw = *src.data.raw;
}

Image::Image(char *fname) {

    int numComponents; //(e.g., Y, YA, RGB, or RGBA)
    data.raw = stbi_load(fname, &width, &height, &numComponents, 4);

    if (data.raw == NULL) {
        printf("Error loading image: %s", fname);
        exit(-1);
    }


    num_pixels = width * height;
    sampling_method = IMAGE_SAMPLING_POINT;

}

Image::~Image() {
    delete data.raw;
    data.raw = NULL;
}

void Image::Write(char *fname) {

    int lastc = strlen(fname);

    switch (fname[lastc - 1]) {
        case 'g': //jpeg (or jpg) or png
            if (fname[lastc - 2] == 'p' || fname[lastc - 2] == 'e') //jpeg or jpg
                stbi_write_jpg(fname, width, height, 4, data.raw, 95);  //95% jpeg quality
            else //png
                stbi_write_png(fname, width, height, 4, data.raw, width * 4);
            break;
        case 'a': //tga (targa)
            stbi_write_tga(fname, width, height, 4, data.raw);
            break;
        case 'p': //bmp
        default:
            stbi_write_bmp(fname, width, height, 4, data.raw);
    }
}

void Image::AddNoise(double factor) {
    /* WORK HERE DONE*/
    int x, y;
    for (x = 0; x < Width(); x++) {
        for (y = 0; y < Height(); y++) {
            Pixel p = GetPixel(x, y);
            Pixel noisy_p;
            noisy_p.SetClamp(rand() % 512 - 256, rand() % 512 - 256, rand() % 512 - 256);
            GetPixel(x, y) = PixelLerp(p, noisy_p, factor);
        }
    }
}

void Image::Brighten(double factor) {
    int x, y;
    for (x = 0; x < Width(); x++) {
        for (y = 0; y < Height(); y++) {
            Pixel p = GetPixel(x, y);
            Pixel scaled_p = p * factor;
            GetPixel(x, y) = scaled_p;
        }
    }
}


void Image::ChangeContrast(double factor) {
    /* WORK HERE DONE*/
    for (int x = 0; x < Width(); x++) {
        for (int y = 0; y < Height(); y++) {
            Pixel grayPixel = Pixel(127, 127, 127);
            Pixel originalPixel = GetPixel(x, y);
            GetPixel(x, y) = PixelLerp(grayPixel, originalPixel, factor);
        }
    }
}


void Image::ChangeSaturation(double factor) {
    /* WORK HERE DONE*/
    for (int x = 0; x < Width(); x++) {
        for (int y = 0; y < Height(); y++) {
            Pixel originalPixel = GetPixel(x, y);
            Pixel grayPixel = Pixel(
                    (unsigned char) (0.2126f * originalPixel.r + 0.7512f * originalPixel.g + 0.0722 * originalPixel.b),
                    (unsigned char) (0.2126f * originalPixel.r + 0.7512f * originalPixel.g + 0.0722 * originalPixel.b),
                    (unsigned char) (0.2126f * originalPixel.r + 0.7512f * originalPixel.g + 0.0722 * originalPixel.b));
            GetPixel(x, y) = PixelLerp(grayPixel, originalPixel, factor);
        }
    }
}


Image *Image::Crop(int x, int y, int w, int h) {
    /* WORK HERE DONE*/
    assert(ValidCoord(x, y));
    assert(ValidCoord(x + w, y + h));
    auto *croppedImage = new Image(w, h);
    for (int i = 0; i < w; i++)
        for (int j = 0; j < h; j++)
            croppedImage->SetPixel(i, j, GetPixel(x + i, y + j));
    return croppedImage;
}


void Image::ExtractChannel(int channel) {
    /* WORK HERE DONE*/
    int x, y;
    for (x = 0; x < Width(); x++) {
        for (y = 0; y < Height(); y++) {
            Pixel p = GetPixel(x, y);
            Pixel extracted_p;
            switch (channel) {
                case IMAGE_CHANNEL_RED:
                    // extract red
                    extracted_p.Set(p.r, 0, 0);
                    break;
                case IMAGE_CHANNEL_GREEN:
                    // extract green
                    extracted_p.Set(0, p.g, 0);
                    break;
                case IMAGE_CHANNEL_BLUE:
                    // extract blue
                    extracted_p.Set(0, 0, p.b);
                    break;
                default:
                    break;
            }
            GetPixel(x, y) = extracted_p;
        }
    }
}


void Image::Quantize(int nbits) {
    /* WORK HERE DONE */
    if (nbits >= 8)
        return;
    for (int x = 0; x < Width(); x++) {
        for (int y = 0; y < Height(); y++) {
            Pixel quantPixel = GetPixel(x, y);
//            printf("%d became %d\t", quantPixel.r, (quantPixel.r >> (8 - nbits)) << (8 - nbits));
//            printf("%d became %d\t", quantPixel.g, (quantPixel.g >> (8 - nbits)) << (8 - nbits));
//            printf("%d became %d\n", quantPixel.b, (quantPixel.b >> (8 - nbits)) << (8 - nbits));
            quantPixel.SetClamp((quantPixel.r >> (8 - nbits)) << (8 - nbits),
                                (quantPixel.g >> (8 - nbits)) << (8 - nbits),
                                (quantPixel.b >> (8 - nbits)) << (8 - nbits));
            GetPixel(x, y) = quantPixel;
        }
    }
}

void Image::RandomDither(int nbits) {
    /* WORK HERE DONE */
    if (nbits >= 8)
        return;
    for (int x = 0; x < Width(); x++) {
        for (int y = 0; y < Height(); y++) {
            Pixel ditheredPixel = GetPixel(x, y);
            int noise = rand() % (1 << (8 - nbits));

            int redMostSignificant = ditheredPixel.r >> (8 - nbits);
            int greenMostSignificant = ditheredPixel.g >> (8 - nbits);
            int blueMostSignificant = ditheredPixel.b >> (8 - nbits);

            int redLeastSignificant = ditheredPixel.r & ((1 << (8 - nbits)) - 1);
            int greenLeastSignificant = ditheredPixel.g & ((1 << (8 - nbits)) - 1);
            int blueLeastSignificant = ditheredPixel.b & ((1 << (8 - nbits)) - 1);

//            printf("%d became %d\t", ditheredPixel.r,
//                   (redMostSignificant + noise < redLeastSignificant ? 1 : 0) << (8 - nbits));
//            printf("%d became %d\t", ditheredPixel.g,
//                   (greenMostSignificant + noise < greenLeastSignificant ? 1 : 0) << (8 - nbits));
//            printf("%d became %d\n", ditheredPixel.b,
//                   (blueMostSignificant + noise < blueLeastSignificant ? 1 : 0) << (8 - nbits));

            ditheredPixel.SetClamp((redMostSignificant + (noise < redLeastSignificant ? 1 : 0)) << (8 - nbits),
                                   (greenMostSignificant + (noise < greenLeastSignificant ? 1 : 0)) << (8 - nbits),
                                   (blueMostSignificant + (noise < blueLeastSignificant ? 1 : 0)) << (8 - nbits));
            GetPixel(x, y) = ditheredPixel;
        }
    }
}


static int Bayer4[4][4] =
        {
                {15, 7,  13, 5},
                {3,  11, 1,  9},
                {12, 4,  14, 6},
                {0,  8,  2,  10}
        };


void Image::OrderedDither(int nbits) {
    /* WORK HERE DONE */
    if (nbits >= 8)
        return;
    for (int x = 0; x < Width(); x++) {
        for (int y = 0; y < Height(); y++) {
            Pixel ditheredPixel = GetPixel(x, y);
            int i = x % 4;
            int j = y % 4;

            int redMostSignificant = ditheredPixel.r >> (8 - nbits);
            int greenMostSignificant = ditheredPixel.g >> (8 - nbits);
            int blueMostSignificant = ditheredPixel.b >> (8 - nbits);

            int redError = ditheredPixel.r - redMostSignificant;
            int greenError = ditheredPixel.g - greenMostSignificant;
            int blueError = ditheredPixel.b - blueMostSignificant;

//            printf("%d became %d\t", ditheredPixel.r,
//                   (redMostSignificant + (redError > (8-nbits)*Bayer4[i][j] ? 1 : 0)) << (8 - nbits));
//            printf("%d became %d\t", ditheredPixel.g,
//                   (greenMostSignificant + (greenError > (8-nbits)*Bayer4[i][j] ? 1 : 0)) << (8 - nbits));
//            printf("%d became %d\n", ditheredPixel.b,
//                   (blueMostSignificant + (blueError > (8-nbits)*Bayer4[i][j] ? 1 : 0)) << (8 - nbits));

            ditheredPixel.SetClamp(
                    (redMostSignificant + (redError > (8 - nbits) * Bayer4[i][j] ? 1 : 0)) << (8 - nbits),
                    (greenMostSignificant + (greenError > (8 - nbits) * Bayer4[i][j] ? 1 : 0)) << (8 - nbits),
                    (blueMostSignificant + (blueError > (8 - nbits) * Bayer4[i][j] ? 1 : 0)) << (8 - nbits));
            GetPixel(x, y) = ditheredPixel;
        }
    }
}

/* Error-diffusion parameters */
const double
        ALPHA = 7.0 / 16.0,
        BETA = 3.0 / 16.0,
        GAMMA = 5.0 / 16.0,
        DELTA = 1.0 / 16.0;

void Image::FloydSteinbergDither(int nbits) {
    /* WORK HERE DONE */
    if (nbits >= 8)
        return;
    for (int y = 0; y < Height(); y++) {
        for (int x = 0; x < Width(); x++) {
            Pixel ditheredPixel = GetPixel(x, y);
            int newRed = (ditheredPixel.r >> (8 - nbits)) << (8 - nbits);
            int newGreen = (ditheredPixel.g >> (8 - nbits)) << (8 - nbits);
            int newBlue = (ditheredPixel.b >> (8 - nbits)) << (8 - nbits);
//            printf("%d became %d\t", ditheredPixel.r, newRed);
//            printf("%d became %d\t", ditheredPixel.g, newGreen);
//            printf("%d became %d\n", ditheredPixel.b, newBlue);
            Pixel errorPixel = Pixel((unsigned char) (ditheredPixel.r - newRed),
                                     (unsigned char) (ditheredPixel.g - newGreen),
                                     (unsigned char) (ditheredPixel.b - newBlue));
            if (x + 1 < Width())
                SetPixel(x + 1, y, errorPixel * ALPHA + GetPixel(x + 1, y));
            if (y + 1 < Height()) {
                if (x > 0)
                    SetPixel(x - 1, y + 1, errorPixel * BETA + GetPixel(x - 1, y + 1));
                if (x + 1 < Width())
                    SetPixel(x + 1, y + 1, errorPixel * DELTA + GetPixel(x + 1, y + 1));
                SetPixel(x, y + 1, errorPixel * GAMMA + GetPixel(x, y + 1));
            }
            ditheredPixel.SetClamp(newRed, newGreen, newBlue);
            GetPixel(x, y) = ditheredPixel;
        }
    }
}

void Image::Blur(int n) {
    /* WORK HERE DONE */
    assert(n % 2 != 0);

    auto *gaussianKernel = GaussianKernel(n);
    auto *resultImage = new Image(Width(), Height());
    // Apply 1D Gaussian to each row.
    for (int y = 0; y < Height(); y++) {
        for (int x = 0; x < Width(); x++) {
            // convolve
            auto *newPixel = new Pixel(0, 0, 0);
            for (int z = -n / 2; z <= n / 2; z++) {
                if ((x + z) < 0 || (x + z) >= Width()) {
                    // copy edge on spill
                    newPixel->SetClamp(newPixel->r + GetPixel(x, y).r * gaussianKernel[(z + n) / 2],
                                       newPixel->g + GetPixel(x, y).g * gaussianKernel[(z + n) / 2],
                                       newPixel->b + GetPixel(x, y).b * gaussianKernel[(z + n) / 2]);
                } else {
                    newPixel->SetClamp(newPixel->r + GetPixel(x + z, y).r * gaussianKernel[(z + n) / 2],
                                       newPixel->g + GetPixel(x + z, y).g * gaussianKernel[(z + n) / 2],
                                       newPixel->b + GetPixel(x + z, y).b * gaussianKernel[(z + n) / 2]);
                }
            }
            resultImage->SetPixel(x, y, *newPixel);
        }
    }
    // Apply 1D Gaussian to each column.
    for (int x = 0; x < Width(); x++) {
        for (int y = 0; y < Height(); y++) {
            // convolve
            auto *newPixel = new Pixel(0, 0, 0);
            for (int z = -n / 2; z <= n / 2; z++) {
                if ((y + z) < 0 || (y + z) >= Height()) {
                    // copy edge on spill
                    newPixel->SetClamp(newPixel->r + resultImage->GetPixel(x, y).r * gaussianKernel[(z + n) / 2],
                                       newPixel->g + resultImage->GetPixel(x, y).g * gaussianKernel[(z + n) / 2],
                                       newPixel->b + resultImage->GetPixel(x, y).b * gaussianKernel[(z + n) / 2]);
                } else {
                    newPixel->SetClamp(newPixel->r + resultImage->GetPixel(x, y + z).r * gaussianKernel[(z + n) / 2],
                                       newPixel->g + resultImage->GetPixel(x, y + z).g * gaussianKernel[(z + n) / 2],
                                       newPixel->b + resultImage->GetPixel(x, y + z).b * gaussianKernel[(z + n) / 2]);
                }
            }
            SetPixel(x, y, *newPixel);
        }
    }
}

void Image::Sharpen(int n) {
    /* WORK HERE DONE */
    auto *thisImage = new Image(*this);
    Blur(n);
    for (int x = 0; x < Width(); x++) {
        for (int y = 0; y < Height(); y++) {
            Pixel blurPixel = GetPixel(x, y);
            Pixel pixel = thisImage->GetPixel(x, y);
            SetPixel(x, y, PixelLerp(blurPixel, pixel, 1.5));
        }
    }
}

static int SobelVerticalMask[3][3] = {
        {1, 0, -1},
        {2, 0, -2},
        {1, 0, -1}
};

static int SobelHorizontalMask[3][3] = {
        {1,  2,  1},
        {0,  0,  0},
        {-1, -2, -1}
};

void Image::EdgeDetect() {
    /* WORK HERE DONE */
    auto *thisImage = new Image(*this);
    // Brighten and Scale Down - Up to catch darker and larger edges.
    thisImage->Brighten(0.50);
    thisImage->SetSamplingMethod(IMAGE_SAMPLING_GAUSSIAN);
    thisImage = thisImage->Scale(0.5, 0.5);
    thisImage = thisImage->Scale(2, 2);
    for (int x = 0; x < Width(); x++) {
        for (int y = 0; y < Height(); y++) {
            int xValue = 0, yValue = 0;
            if (x > 0 && x < Width() - 1 && y > 0 && y < Height() - 1) {
                // convolve
                for (int i = -1; i < 2; i++) {
                    for (int j = -1; j < 2; j++) {
                        int grayscaleValue = (thisImage->GetPixel(x + i, y + j).r +
                                              thisImage->GetPixel(x + i, y + j).g +
                                              thisImage->GetPixel(x + i, y + j).b) / 3;
                        xValue += grayscaleValue * SobelHorizontalMask[1 + i][1 + j];
                        yValue += grayscaleValue * SobelVerticalMask[1 + i][1 + j];
                    }
                }
                // can "bitwise not" this for color inversion (black edges on white background)
                auto finalValue = (unsigned char) sqrt(xValue * xValue + yValue * yValue);
                SetPixel(x, y, Pixel(finalValue, finalValue, finalValue));
            }
        }
    }
    Brighten(2);
}

Image *Image::Scale(double sx, double sy) {
    /* WORK HERE DONE */
    auto *scaledImage = new Image((int)(Width() * sx), (int)(Height() * sy));
    for(int y = 0; y < scaledImage->Height(); y++){
        for(int x = 0; x < scaledImage->Width(); x++){
            double u = x / sx;
            double v = y / sy;
            scaledImage->SetPixel(x, y, Sample(u, v));
        }
    }
    return scaledImage;
}

Image *Image::Rotate(double angle) {
    /* WORK HERE DONE */
    double angleRadians = angle*M_PI/180;
    auto *rotatedImage = new Image(Width(), Height());
    for(int y = 0; y < rotatedImage->Height(); y++){
        for(int x = 0; x < rotatedImage->Width(); x++){
            double u = (x-Width()/2)*cos(angleRadians) - (y-Height()/2)*sin(angleRadians)+Width()/2;
            double v = (x-Width()/2)*sin(angleRadians) + (y-Height()/2)*cos(angleRadians)+Height()/2;
            rotatedImage->SetPixel(x, y, Sample(u, v));
        }
    }
    return rotatedImage;
}

Image* Image::Fun(double twistFactor) {
    /* WORK HERE DONE */
    // Swirl Transform
    auto *twistedImage = new Image(Width(), Height());
    for(int y = 0; y < twistedImage->Height(); y++){
        for(int x = 0; x < twistedImage->Width(); x++){
            double angleRadians = twistFactor*M_PI/sqrt(pow(x-Width()/2, 2) + pow(y-Height()/2, 2)+0.0001);
            double u = (x-Width()/2)*cos(angleRadians) - (y-Height()/2)*sin(angleRadians)+Width()/2;
            double v = (x-Width()/2)*sin(angleRadians) + (y-Height()/2)*cos(angleRadians)+Height()/2;
            if(__isnan(u))
                printf("%lf %lf\n", u, v);
            twistedImage->SetPixel(x, y, Sample(u, v));
        }
    }
    return twistedImage;

    // Wave Transform
//    auto *wavyImage = new Image(Width(), Height());
//    for(int y = 0; y < wavyImage->Height(); y++){
//        for(int x = 0; x < wavyImage->Width(); x++){
//            double u = x + 20*sin(2*M_PI*x/24);
//            double v = y;
//            wavyImage->SetPixel(x, y, Sample(u, v));
//        }
//    }
//    return wavyImage;
}

/**
 * Image Sample
 **/
void Image::SetSamplingMethod(int method) {
    assert((method >= 0) && (method < IMAGE_N_SAMPLING_METHODS));
    sampling_method = method;
}


Pixel Image::Sample(double u, double v) {
    /* WORK HERE DONE */
    if(u >= Width() || u < 0 || v >= Height() || v < 0)
        return {};
    switch(sampling_method) {
        case IMAGE_SAMPLING_POINT:
            return NearestNeighborSampling(u, v);
        case IMAGE_SAMPLING_BILINEAR:
            return BilinearSampling(u, v);
        case IMAGE_SAMPLING_GAUSSIAN:
            return GaussianSampling(u, v, 7);
        default:
            return NearestNeighborSampling(u, v);
    }
}

Pixel Image::NearestNeighborSampling(double u, double v) {
    return GetPixel((int)u, (int)v);
}

Pixel Image::BilinearSampling(double u, double v) {
    double floorU = floor(u)<Width()?floor(u):floor(u-1);
    double ceilU = ceil(u)<Width()?ceil(u):ceil(u-1);
    double floorV = floor(v)<Height()?floor(v):floor(v-1);
    double ceilV = ceil(v)<Height()?ceil(v):ceil(v-1);

    // linearly interpolate across upper two pixel choices
    Pixel upperInterpolation = PixelLerp(GetPixel((int)floorU, (int)floorV),
                                         GetPixel((int)ceilU, (int)floorV),
                                         u-floorU);
    // linearly interpolate across lower two pixel choices
    Pixel lowerInterpolation = PixelLerp(GetPixel((int)floorU, (int)ceilV),
                                         GetPixel((int)ceilU, (int)ceilV),
                                         u-floorU);
    // linearly interpolate across upper and lower linear interpolations.
    return PixelLerp(upperInterpolation, lowerInterpolation, v-floorV);
}

Pixel Image::GaussianSampling(double u, double v, int r) {
    Pixel newPixel = Pixel();
    // Convolve across X and Y
    for(auto x=(int)ceil(u - r / 2); x <= (int)floor(u + r / 2); x++){
        for(auto y=(int)ceil(v - r / 2); y <= (int)floor(v + r / 2); y++){
            if(x >=Width() || x < 0 || y >= Height() || y < 0)
                continue;
            newPixel = newPixel +
                    GetPixel(x, y) * GaussianValue2D(fabs(x-u), fabs(y-v), 1);
        }
    }
    return newPixel;
}
