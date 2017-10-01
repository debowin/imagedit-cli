//Image Manipulation Skeleton Code
//
//
//  main.c
//  original by Wagner Correa, 1999
//  modified by Robert Osada, 2000
//  modified by Renato Werneck, 2003
//  mmodified by Stephen J. Guy, 2010-2017

//This project uses the EasyBMP Cross-Platform Windows Bitmap Library
//Web page: http://easybmp.sourceforge.net 


#include "image.h"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>


#define STB_IMAGE_IMPLEMENTATION //only place once in one .cpp file
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION //only place once in one .cpp files
#include "stb_image_write.h"


using namespace std;


/**
 * prototypes
 **/
static void ShowUsage(void);
static void CheckOption(char *option, int argc, int minargc);

int main( int argc, char* argv[] ){
    Image *img = NULL;
    bool did_output = false;

    // first argument is program name
    argv++, argc--;

    // look for help
    for (int i = 0; i < argc; i++) {
        if (!strcmp(argv[i], "-help")) {
            ShowUsage();
        }
    }

    // no argument case
    if (argc == 0) {
        ShowUsage();
    }

    // parse arguments
    while (argc > 0)
    {
        if (**argv == '-')
        {
            if (!strcmp(*argv, "-input"))
            {
                CheckOption(*argv, argc, 2);
                if (img != NULL)
                    delete img;
                img = new Image(argv[1]);
                argv += 2, argc -= 2;
            }

            else if (!strcmp(*argv, "-output"))
            {
                CheckOption(*argv, argc, 2);
                if (img == NULL) ShowUsage();
                img->Write(argv[1]);
                did_output = true;
                argv += 2, argc -= 2;
            }

            else if (!strcmp(*argv, "-noise"))
            {
                double factor;
                CheckOption(*argv, argc, 2);
                if (img == NULL) ShowUsage();

                factor = atof(argv[1]);
                img->AddNoise(factor);
                argv += 2, argc -= 2;
            }

            else if (!strcmp(*argv, "-brightness"))
            {
                double factor;
                CheckOption(*argv, argc, 2);
                if (img == NULL) ShowUsage();

                factor = atof(argv[1]);
                img->Brighten(factor);
                argv += 2, argc -=2;
            }

            else if (!strcmp(*argv, "-contrast"))
            {
                double factor;
                CheckOption(*argv, argc, 2);
                if (img == NULL) ShowUsage();

                factor = atof(argv[1]);
                img->ChangeContrast(factor);
                argv += 2, argc -= 2;
            }

            else if (!strcmp(*argv, "-saturation"))
            {
                double factor;
                CheckOption(*argv, argc, 2);
                if (img == NULL) ShowUsage();

                factor = atof(argv[1]);
                img->ChangeSaturation(factor);
                argv += 2, argc -= 2;
            }

            else if (!strcmp(*argv, "-crop"))
            {
                int x, y, w, h;
                CheckOption(*argv, argc, 5);
                if (img == NULL) ShowUsage();

                x = atoi(argv[1]);
                y = atoi(argv[2]);
                w = atoi(argv[3]);
                h = atoi(argv[4]);

                Image *dst = img->Crop(x, y, w, h);
                delete img;
                img = dst;

                argv += 5, argc -= 5;
            }

            else if (!strcmp(*argv, "-extractChannel"))
            {
                int channel;
                CheckOption(*argv, argc, 2);
                if (img == NULL) ShowUsage();

                channel = atoi(argv[1]);
                img->ExtractChannel(channel);
                argv += 2, argc -= 2;
            }

            else if (!strcmp(*argv, "-quantize"))
            {
                int nbits;
                CheckOption(*argv, argc, 2);
                if (img == NULL) ShowUsage();

                nbits = atoi(argv[1]);
                img->Quantize(nbits);
                argv += 2, argc -= 2;
            }

            else if (!strcmp(*argv, "-randomDither"))
            {
                int nbits;
                CheckOption(*argv, argc, 2);
                if (img == NULL) ShowUsage();

                nbits = atoi(argv[1]);
                img->RandomDither(nbits);
                argv += 2, argc -= 2;
            }

            else if (!strcmp(*argv, "-blur"))
            {
                int n;
                CheckOption(*argv, argc, 2);
                if (img == NULL) ShowUsage();

                n = atoi(argv[1]);
                img->Blur(n);
                argv += 2, argc -= 2;
            }
            else if (!strcmp(*argv, "-sharpen"))
            {
                int n;
                CheckOption(*argv, argc, 2);
                if (img == NULL) ShowUsage();

                n = atoi(argv[1]);
                img->Sharpen(n);
                argv += 2, argc -= 2;
            }

            else if (!strcmp(*argv, "-edgeDetect"))
            {
                if (img == NULL) ShowUsage();

                img->EdgeDetect();
                argv++, argc--;
            }

            else if (!strcmp(*argv, "-orderedDither"))
            {
                int nbits;
                CheckOption(*argv, argc, 2);
                if (img == NULL) ShowUsage();

                nbits = atoi(argv[1]);
                img->OrderedDither(nbits);
                argv += 2, argc -= 2;
            }

            else if (!strcmp(*argv, "-FloydSteinbergDither"))
            {
                int nbits;
                CheckOption(*argv, argc, 2);
                if (img == NULL) ShowUsage();

                nbits = atoi(argv[1]);
                img->FloydSteinbergDither(nbits);
                argv += 2, argc -= 2;
            }

            else if (!strcmp(*argv, "-scale"))
            {
                CheckOption(*argv, argc, 3);
                if (img == NULL) ShowUsage();

                double sx = atof(argv[1]);
                double sy = atof(argv[2]);

                Image *dst = img->Scale(sx, sy);
                delete img;

                img = dst;
                argv += 3, argc -= 3;
            }

            else if (!strcmp(*argv, "-rotate"))
            {
                double angle;
                Image *dst;
                CheckOption(*argv, argc, 2);
                if (img == NULL) ShowUsage();

                angle = atof(argv[1]);
                dst = img->Rotate(angle);
                delete img;
                img = dst;
                dst = NULL;
                argv += 2, argc -= 2;
            }

            else if (!strcmp(*argv, "-fun"))
            {
                if (img == NULL) ShowUsage();

                img->Fun();
                argv++, argc--;
            }

            else if (!strcmp(*argv, "-sampling"))
            {
                if (img == NULL) ShowUsage();

                int method;
                CheckOption(*argv, argc, 2);
                method = atoi(argv[1]);
                img->SetSamplingMethod(method);
                argv += 2, argc -= 2;
            }

            else
            {
                fprintf(stderr, "image: invalid option: %s\n", *argv);
                ShowUsage();
            }
        }
        else
        {
            fprintf(stderr, "image: invalid option: %s\n", *argv);
            ShowUsage();
        }
    }

    if (!did_output)
    {
        fprintf( stderr, "Warning, you didn't tell me to output anything.  I hope that's OK.\n" );
    }

    delete img;
    return EXIT_SUCCESS;
}


/**
 * ShowUsage
 **/
static char options[] =
        "-help\n"
                "-input <file>\n"
                "-output <file>\n"
                "-noise <factor>\n"
                "-brightness <factor>\n"
                "-contrast <factor>\n"
                "-saturation <factor>\n"
                "-crop <x> <y> <width> <height>\n"
                "-extractChannel <channel no>\n"
                "-quantize <nbits>\n"
                "-randomDither <nbits>\n"
                "-blur <maskSize>\n"
                "-sharpen <maskSize>\n"
                "-edgeDetect\n"
                "-orderedDither <nbits>\n"
                "-FloydSteinbergDither <nbits>\n"
                "-scale <sx> <sy>\n"
                "-rotate <angle>\n"
                "-fun\n"
                "-sampling <method no>\n"
;

static void ShowUsage(void)
{
    fprintf(stderr, "Usage: image -input <filename> [-option [arg ...] ...] -output <filename>\n");
    fprintf(stderr, "%s", options);
    exit(EXIT_FAILURE);
}



/**
 * CheckOption
 **/
static void CheckOption(char *option, int argc, int minargc)
{
    if (argc < minargc)
    {
        fprintf(stderr, "Too few arguments for %s\n", option);
        ShowUsage();
    }
}