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



int main(){
    cout << "Hello" << endl;
    return 0;
}