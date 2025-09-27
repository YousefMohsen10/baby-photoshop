#include <iostream>
#include "Image_Class.h"

using namespace std;

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




int main(){
    cout << "Hello" << endl;
    return 0;
}