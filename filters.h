#ifndef FILTERS_H
#define FILTERS_H

#endif // FILTERS_H
#include "Image_Class.h"
#include <vector>
#include <algorithm>
using namespace std;
void gray_scale(Image &image)
{
    for (int i = 0; i < image.width; i++)
    {
        for (int j = 0; j < image.height; j++)
        {
            unsigned int avg = 0;
            for (int k = 0; k < image.channels; k++)
            {
                avg += image(i, j, k);
            }
            avg /= 3;
            for (int k = 0; k < 3; k++)
            {
                image(i, j, k) = avg;
            }
        }
    }
}

void Black_White(Image &image)
{
    int therhold = 128;
    for (int i = 0; i < image.width; i++)
    {
        for (int j = 0; j < image.height; j++)
        {
            int avg = 0;

            for (int channel = 0; channel < 3; channel++)
            {
                avg += image(i, j, channel);
            }

            avg /= 3;

            for (int channel = 0; channel < 3; channel++)
            {
                image(i, j, channel) = avg;
            }
            if (image(i, j, 0) >= therhold)
            {
                image(i, j, 0) = 255;
                image(i, j, 1) = 255;
                image(i, j, 2) = 255;
            }
            else
            {
                image(i, j, 0) = 0;
                image(i, j, 1) = 0;
                image(i, j, 2) = 0;
            }
        }
    }
}

void invert(Image &image)
{
    for (int i = 0; i < image.width; ++i)
    {
        for (int j = 0; j < image.height; ++j)
        {
            for (int k = 0; k < 3; ++k)
            {
                image(i, j, k) = 255 - image(i, j, k);
            }
        }
    }
}

void oldTVFilter(Image &image)
{
    for (int j = 0; j < image.height; j++)
    {
        if (j % 2 == 0)
        {
            for (int i = 0; i < image.width; i++)
            {
                image(i, j, 0) = image(i, j, 0) * 0.6;
                image(i, j, 1) = image(i, j, 1) * 0.6;
                image(i, j, 2) = image(i, j, 2) * 0.6;
            }
        }
    }
}

Image edge_detection(Image &image) {
    int width = image.width;
    int height = image.height;
    Image edges(width, height);
    int Gx[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    int Gy[3][3] = {
        {-1, -2, -1},
        { 0,  0,  0},
        { 1,  2,  1}
    };

    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            int sumX = 0;
            int sumY = 0;
            for (int ky = -1; ky <= 1; ky++) {
                for (int kx = -1; kx <= 1; kx++) {
                    int r = image(x + kx, y + ky, 0);
                    int g = image(x + kx, y + ky, 1);
                    int b = image(x + kx, y + ky, 2);
                    int gray = (r + g + b) / 3;

                    sumX += gray * Gx[ky + 1][kx + 1];
                    sumY += gray * Gy[ky + 1][kx + 1];
                }
            }
            int magnitude = sumX * sumX + sumY * sumY;
            int value;
            if (magnitude >= 128*128)
                value = 0;
            else
                value = 255;
            for (int k =0; k<3; k++) {
                edges(x, y, k) = value;
            }
        }
    }

    return edges;

}

Image oil_painting(Image& image, int radius = 3, int intensityLevels = 35)
{
    Image original = image;
    int width = image.width;
    int height = image.height;

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {

            vector<int> intensityCount(intensityLevels, 0);
            vector<long> sumR(intensityLevels, 0);
            vector<long> sumG(intensityLevels, 0);
            vector<long> sumB(intensityLevels, 0);

            for (int dy = -radius; dy <= radius; dy++)
            {
                for (int dx = -radius; dx <= radius; dx++)
                {
                    int nx = x + dx;
                    int ny = y + dy;

                    if (nx >= 0 && nx < width && ny >= 0 && ny < height)
                    {
                        unsigned char r, g, b;
                        r = original(nx, ny, 0);
                        g = original(nx, ny, 1);
                        b = original(nx, ny, 2);

                        int intensity = ((r + g + b) / 3.0) * intensityLevels / 256;

                        if (intensity >= intensityLevels)
                            intensity = intensityLevels - 1;

                        intensityCount[intensity]++;
                        sumR[intensity] += r;
                        sumG[intensity] += g;
                        sumB[intensity] += b;
                    }
                }
            }

            int maxIndex = 0;
            for (int i = 1; i < intensityLevels; i++)
            {
                if (intensityCount[i] > intensityCount[maxIndex])
                    maxIndex = i;
            }

            int count = max(intensityCount[maxIndex], 1);
            char newR = sumR[maxIndex] / count;
            char newG = sumG[maxIndex] / count;
            char newB = sumB[maxIndex] / count;

            image(x, y, 0) = newR;
            image(x, y, 1) = newG;
            image(x, y, 2) = newB;

        }
    }
    return image;
}

void Infra_Red(Image& image)
{
    int width = image.width;
    int height = image.height;

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            unsigned char r = image(x, y, 0);
            unsigned char g = image(x, y, 1);
            unsigned char b = image(x, y, 2);

            g = 255 - g;
            b = 255 - b;

            r = 255;

            image(x, y, 0) = r;
            image(x, y, 1) = g;
            image(x, y, 2) = b;
        }
    }
}

void purple_scale(Image &image) {
    float redBoost   = 1.2f;
    float greenScale = 0.8f;
    float blueBoost  = 1.2f;

    for (int x = 0; x < image.width; x++) {
        for (int y = 0; y < image.height; y++) {

            unsigned int R = image(x, y, 0);
            unsigned int G = image(x, y, 1);
            unsigned int B = image(x, y, 2);

            int newR = min(255, int(R * redBoost));
            int newG = max(0,   int(G * greenScale));
            int newB = min(255, int(B * blueBoost));

            image(x, y, 0) = (unsigned int)newR;
            image(x, y, 1) = (unsigned int)newG;
            image(x, y, 2) = (unsigned int)newB;
        }
    }
}

void sunlight_glow(Image &image, float warmth = 0.25f, float brightness = 0.15f, float contrast = 1.1f) {
    int width = image.width;
    int height = image.height;

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            float r = image(i, j, 0);
            float g = image(i, j, 1);
            float b = image(i, j, 2);

            r = r + (255 - r) * brightness;
            g = g + (255 - g) * brightness;
            b = b + (255 - b) * brightness;

            r = r * (1.0f + warmth);
            g = g * (1.0f + warmth * 0.5f);
            b = b * (1.0f - warmth * 0.5f);

            r = ((r - 128) * contrast) + 128;
            g = ((g - 128) * contrast) + 128;
            b = ((b - 128) * contrast) + 128;

            image(i, j, 0) = min(255, max(0, (int)r));
            image(i, j, 1) = min(255, max(0, (int)g));
            image(i, j, 2) = min(255, max(0, (int)b));
        }
    }
}

Image rotate(Image &image, int angle)
{
    Image rotated = image;
    switch (angle)
    {
    case 270:
    {
        Image newimage(image.height, image.width);
        for (int i = 0; i < image.width; ++i)
            for (int j = 0; j < image.height; ++j)
                for (int k = 0; k < image.channels; ++k)
                    newimage(j, image.width - 1 - i, k) = image(i, j, k);
        rotated = newimage;
        break;
    }
    case 180:
    {
        Image newimage(image.width, image.height);
        for (int i = 0; i < image.width; ++i)
            for (int j = 0; j < image.height; ++j)
                for (int k = 0; k < image.channels; ++k)
                    newimage(image.width - 1 - i, image.height - 1 - j, k) = image(i, j, k);
        rotated = newimage;
        break;
    }
    case 90:
    {
        Image newimage(image.height, image.width);
        for (int i = 0; i < image.width; ++i)
            for (int j = 0; j < image.height; ++j)
                for (int k = 0; k < image.channels; ++k)
                    newimage(image.height - 1 - j, i, k) = image(i, j, k);
        rotated = newimage;
        break;
    }
    }
    return rotated;
}

void flip(Image& image, int flip_type) {
    // int flip_type;
    // while (true) {
    //     cout << "Enter flipping option: \n"
    //             "1- Flip Vertical\n"
    //             "2- Flip Horizontal\n";
    //     cin >> flip_type;
    //     if (flip_type == 1 || flip_type == 2) {
    //         break;
    //     }else {
    //         cout << "Invalid Option." << endl;
    //     }
    // }
    if (flip_type == 1) {
        for (int x = 0; x < image.width; x++) {
            for (int y = 0; y < image.height / 2; y++) {
                int oppositeY = image.height - 1 - y;
                for (int channel = 0; channel < 3; channel++) {
                    swap(image(x, y, channel), image(x, oppositeY, channel));
                }
            }
        }
    }
    else if (flip_type == 2) {
        for (int y = 0; y < image.height; y++) {
            for (int x = 0; x < image.width / 2; x++) {
                int oppositeX = image.width - 1 - x;
                for (int channel = 0; channel < 3; channel++) {
                    swap(image(x, y, channel), image(oppositeX, y, channel));
                }
            }
        }
    }
}

Image crop_image(Image image, const int row, const int col, const int width, const int height) {
    Image cropped_image(width, height);
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            cropped_image(i, j, 0) = image(row + i, col + j, 0);
            cropped_image(i, j, 1) = image(row + i, col + j, 1);
            cropped_image(i, j, 2) = image(row + i, col + j, 2);
        }
    }
    return cropped_image;
}

void darken_and_lighten(Image &image, double lvl=0) {
    // while (true) {
    //     cout << "Enter level of brightness: ";
    //     cin >> lvl;
    //     lvl /= 100.0;
    //     if (lvl <= 0.0) {
    //         cout << "Invalid brightness level.\n";
    //     }
    //     else {
    //         break;
    //     }
    // }
    if(lvl >= 0){
        lvl+=100;
    }
    else{
        lvl=-lvl;
    }
    lvl /= 100.0;
    for (int i = 0; i< image.width; i++) {
        for (int j = 0; j<image.height; j++) {
            for (int k = 0; k <image.channels; k++) {
                int new_brightness = image(i, j, k) * lvl;
                if (new_brightness < 0) new_brightness = 0;
                if (new_brightness > 255) new_brightness = 255;
                image(i, j, k) = new_brightness;
            }
        }
    }
}

Image resize_image(Image &image, int new_Width, int new_Height)
{
    Image new_Image(new_Width, new_Height);
    float x_ratio = static_cast<float>(image.width) / new_Width;
    float y_ratio = static_cast<float>(image.height) / new_Height;

    for (int i = 0; i < new_Width; i++)
    {
        for (int j = 0; j < new_Height; j++)
        {
            int srcX = min(static_cast<int>(i * x_ratio), image.width - 1);
            int srcY = min(static_cast<int>(j * y_ratio), image.height - 1);

            for (int channel = 0; channel < 3; channel++)
            {
                new_Image(i, j, channel) = image(srcX, srcY, channel);
            }
        }
    }
    return new_Image;
}

Image blur_image(Image &image, int radius, bool horizontal)
{
    Image temp(image.width, image.height);
    for (int j = 0; j < image.height; j++)
    {
        for (int i = 0; i < image.width; i++)
        {
            int sum[3] = {0, 0, 0}, count = 0;
            for (int k = -radius; k <= radius; k++)
            {
                int ni = horizontal ? i + k : i;
                int nj = horizontal ? j : j + k;
                if (ni >= 0 && ni < image.width && nj >= 0 && nj < image.height)
                {
                    for (int c = 0; c < 3; c++)
                        sum[c] += image.getPixel(ni, nj, c);
                    count++;
                }
            }
            for (int c = 0; c < 3; c++)
                temp.setPixel(i, j, c, sum[c] / count);
        }
    }
    return temp;
}

void skewImage(Image &image, double angle = 0)
{
    double rad = angle * M_PI / 180.0;
    double skew = tan(rad);
    int newWidth = image.width + abs(image.height * skew);
    int newHeight = image.height;
    Image newImage(newWidth, newHeight);
    int xshift;
    if (skew > 0)
    {
        xshift = abs(image.height * skew);
    }
    else
    {
        xshift = 0;
    }
    for (int j = 0; j < image.height; j++)
    {
        int shift = -j * skew;
        for (int i = 0; i < image.width; i++)
        {
            int newI = i + shift + xshift;
            if (newI >= 0 && newI < newWidth)
            {
                for (int c = 0; c < image.channels; c++)
                    newImage(newI, j, c) = image(i, j, c);
            }
        }
    }
    image = newImage;
}

Image merge_image(Image image1, Image image2, const string& option )
{

    int merge_width, merge_height;
    if (option == "resize") {
        merge_width = max(image1.width, image2.width);
        merge_height = max(image1.height, image2.height);
        image1 = resize_image(image1, merge_width, merge_height);
        image2 = resize_image(image2, merge_width, merge_height);
    }
    else if (option == "overlap") {
        merge_width=min(image1.width, image2.width);
        merge_height = min(image1.height, image2.height);
    }
    Image merged(merge_width, merge_height);
    for (int i = 0; i < merge_width; i++)
    {
        for (int j = 0; j < merge_height; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                merged(i, j, k) = static_cast<unsigned int>(image1(i, j, k) + image2(i, j, k)) / 2;
            }
        }
    }
    return merged;
}

Image neon_filter(Image &image)
{
    Image result(image.width, image.height);

    for (int j = 1; j < image.height - 1; j++)
    {
        for (int i = 1; i < image.width - 1; i++)
        {
            int gi = 0, gj = 0;
            for (int c = 0; c < 3; c++)
            {
                int left = image(i - 1, j, c);
                int right = image(i + 1, j, c);
                int up = image(i, j - 1, c);
                int down = image(i, j + 1, c);
                gi += abs(right - left);
                gj += abs(down - up);
            }
            int edge = min(255, (gi + gj) / 2);
            int r = edge / 6;
            int g = edge / 2;
            int b = edge;
            result(i, j, 0) = r;
            result(i, j, 1) = g;
            result(i, j, 2) = b;
        }
    }

    return result;
}

Image water_wave(Image &image)
{
    Image result(image.width, image.height);
    double freq = 0.09;
    double amp = 10.0;
    for (int j = 0; j < image.height; j++)
    {
        int shift = (int)(sin(j * freq) * amp);
        for (int i = 0; i < image.width; i++)
        {
            int newi = i + shift;
            if (newi >= 0 && newi < image.width)
            {
                for (int c = 0; c < 3; c++)
                    result(i, j, c) = image(newi, j, c);
            }
            else
            {
                for (int c = 0; c < 3; c++)
                    result(i, j, c) = 0;
            }
        }
    }
    return result;
}

enum FrameStyle
{
    SIMPLE = 1,
    FANCY = 2
};

void frame(Image &image, int thickness, int innerThickness,int style, bool doubleLine = false, unsigned char r=0, unsigned char g=0, unsigned char b=0, unsigned char wr=0, unsigned char wg=0, unsigned char wb=0)
{
    // string name = colorName;
    // for (auto &c : name)
    //     c = tolower(c);
    // if (name == "red")
    //     r = 255;
    // else if (name == "green")
    //     g = 255;
    // else if (name == "blue")
    //     b = 255;
    if (style == 2)
    {
        int newWidth = image.width + 2 * thickness;
        int newHeight = image.height + 2 * thickness;
        Image framed(newWidth, newHeight);
        for (int j = 0; j < newHeight; j++)
        {
            for (int i = 0; i < newWidth; i++)
            {
                if (i >= thickness && i < newWidth - thickness &&
                    j >= thickness && j < newHeight - thickness)
                {
                    framed(i, j, 0) = image(i - thickness, j - thickness, 0);
                    framed(i, j, 1) = image(i - thickness, j - thickness, 1);
                    framed(i, j, 2) = image(i - thickness, j - thickness, 2);
                }
                else
                {
                    int pattern = (int)(40 * sin(i * 0.25) + 40 * cos(j * 0.25)) + (rand() % 40 - 20);
                    int rr = max(0, min(255, r + pattern));
                    int gg = max(0, min(255, g + pattern));
                    int bb = max(0, min(255, b + pattern));
                    framed(i, j, 0) = (unsigned char)rr;
                    framed(i, j, 1) = (unsigned char)gg;
                    framed(i, j, 2) = (unsigned char)bb;
                }
            }
        }
        image = framed;
        return;
    }
    auto drawPixel = [&](int i, int j, unsigned char rr, unsigned char gg, unsigned char bb)
    {
        if (i >= 0 && i < image.width && j >= 0 && j < image.height)
        {
            image(i, j, 0) = rr;
            image(i, j, 1) = gg;
            image(i, j, 2) = bb;
        }
    };

    if (thickness > image.width / 2)
        thickness = image.width / 2;
    if (thickness > image.height / 2)
        thickness = image.height / 2;
    for (int j = 0; j < thickness; j++)
    {
        for (int i = 0; i < image.width; i++)
        {
            drawPixel(i, j, r, g, b);
            drawPixel(i, image.height - 1 - j, r, g, b);
        }
    }
    for (int i = 0; i < thickness; i++)
    {
        for (int j = 0; j < image.height; j++)
        {
            drawPixel(i, j, r, g, b);
            drawPixel(image.width - 1 - i, j, r, g, b);
        }
    }
    if (doubleLine)
    {
        // int innerThickness = 10;
        // unsigned char wr = 255, wg = 255, wb = 255;

        for (int j = thickness; j < thickness + innerThickness; j++)
        {
            for (int i = thickness; i < image.width - thickness; i++)
            {
                drawPixel(i, j, wr, wg, wb);
                drawPixel(i, image.height - 1 - j, wr, wg, wb);
            }
        }
        for (int i = thickness; i < thickness + innerThickness; i++)
        {
            for (int j = thickness; j < image.height - thickness; j++)
            {
                drawPixel(i, j, wr, wg, wb);
                drawPixel(image.width - 1 - i, j, wr, wg, wb);
            }
        }
    }
}

void shakeFilter(Image &image)
{
    int Shake_degree = 10;
    Image result(image.width, image.height);
    for (int j = 0; j < image.height; j++)
    {
        for (int i = 0; i < image.width; i++)
        {
            int Shift = i + Shake_degree;;
            if (Shift >= image.width)
                Shift = image.width - 1;
            for (int c = 0; c < image.channels; c++)
            {
                result(i, j, c) = (image(i, j, c) * 0.6 + image(Shift, j, c) * 0.4);
            }
        }
    }
    image = result;
}
