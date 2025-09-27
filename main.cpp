#include <iostream>
#include "Image_Class.h"

using namespace std;

void gray_scale(Image &image) {
    for (int i = 0; i< image.width; i++) {
        for (int j = 0; j<image.height; j++) {
            unsigned int avg = 0;
            for (int k = 0; k <image.channels; k++) {
                avg += image(i, j, k);
            }
            avg/=3;
            for (int k =0; k<3; k++) {
                image(i, j, k) = avg;
            }
        }
    }
}

void Black_White(Image& image) {
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
            if (image(i, j, 0) >= therhold) {
                image(i, j, 0) = 255;
                image(i, j, 1) = 255;
                image(i, j, 2) = 255;
            }
            else {
                image(i, j, 0) = 0;
                image(i, j, 1) = 0;
                image(i, j, 2) = 0;
            }
        }
    }

}


Image resize_image(Image& image, int new_Width, int new_Height) {
    Image new_Image(new_Width, new_Height);
    float x_ratio = static_cast<float>(image.width) / new_Width;
    float y_ratio = static_cast<float>(image.height) / new_Height;

    for (int i = 0; i < new_Width; i++) {
        for (int j = 0; j < new_Height; j++) {
            int srcX = min(static_cast<int>(i * x_ratio), image.width - 1);
            int srcY = min(static_cast<int>(j * y_ratio), image.height - 1);

            for (int channel = 0; channel < 3; channel++) {
                new_Image(i, j, channel) = image(srcX, srcY, channel);
            }
        }
    }
    return new_Image;
}

Image merge_image(Image image1, Image image2) {
    int mx_width = max(image1.width, image2.width);
    int mx_height = max(image1.height, image2.height);
    image1 = resize_image(image1, mx_width, mx_height);
    image2 = resize_image(image2, mx_width, mx_height);
    Image merged(mx_width, mx_height);
    for (int i =0; i<image1.width; i++) {
        for (int j =0; j<image1.height; j++) {
            for (int k = 0; k<3; k++) {
                merged(i, j, k) = static_cast<unsigned int>(image1(i, j, k) + image2(i, j, k))/2;
            }
        }
    }
    return merged;
}




int main(){
    cout << "Hello" << endl;
    return 0;
}