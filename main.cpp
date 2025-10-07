#include <iostream>
#include "Image_Class.h"
#include <vector>
using namespace std;

void gray_scale(Image &image)
{
    cout << ">>> Applying gray scale filter to your image... <<<" << endl;
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
    cout << ">>> Applying Black and White filter to your image... <<<" << endl;
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
    cout << ">>> Applying invert filter to your image... <<<" << endl;
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

Image merge_image(Image image1, Image image2)
{
    int mx_width = max(image1.width, image2.width);
    int mx_height = max(image1.height, image2.height);
    image1 = resize_image(image1, mx_width, mx_height);
    image2 = resize_image(image2, mx_width, mx_height);
    Image merged(mx_width, mx_height);
    for (int i = 0; i < image1.width; i++)
    {
        for (int j = 0; j < image1.height; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                merged(i, j, k) = static_cast<unsigned int>(image1(i, j, k) + image2(i, j, k)) / 2;
            }
        }
    }
    return merged;
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
Image blur_image(Image &image, int kernelSize)
{
    int off = kernelSize / 2;
    Image newimage(image.width, image.height);
    for (int j = 0; j < image.height; j++)
    {
        for (int i = 0; i < image.width; i++)
        {
            int sum[3] = {0, 0, 0};
            int count = 0;
            for (int kj = 0; kj < kernelSize; kj++)
            {
                for (int ki = 0; ki < kernelSize; ki++)
                {
                    int ni = i + (ki - off);
                    int nj = j + (kj - off);
                    if (ni >= 0 && ni < image.width && nj >= 0 && nj < image.height)
                    {
                        for (int c = 0; c < 3; c++)
                            sum[c] += image.getPixel(ni, nj, c);
                        count++;
                    }
                }
            }
            for (int c = 0; c < 3; c++)
                newimage.setPixel(i, j, c, sum[c] / count);
        }
    }
    return newimage;
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

void skewImage(Image &image, double angle)
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

Image get_image(string &filename)
{
    Image image;
    while (true)
    {
        cout << "Enter image path (.jpg, .bmp, .png, .tga): ";
        cin >> filename;
        try
        {
            image.loadNewImage(filename);
            break;
        }
        catch (invalid_argument &e)
        {
            cout << "Error: " << e.what() << endl;
        }
        cout << "* SUCCESS: Image '" << filename << "' loaded successfully." << endl;
    }
    return image;
}

void list_filter()
{
    cout << "--------------------------------------------------------------------------------" << endl;
    cout << "                                  FILTER MENU" << endl;
    cout << "--------------------------------------------------------------------------------" << endl;
    cout << "Choose Filter:\n"
            "1- Gray Scale\n"
            "2- Black and White\n"
            "3- Invert Image\n"
            "4- Resize Image\n"
            "5- Merge Image\n"
            "6- Rotate\n"
            "7- Undo last filter\n"
            "8- Redo last filter\n"
            "9- Return to Main Menu"
         << endl;
    cout << ">> Enter filter number (1-9): _";
}

int get_width()
{
    int new_Width;
    cout << "Enter NEW width: " << endl;
    cin >> new_Width;
    return new_Width;
}
int get_height()
{
    int new_Height;
    cout << "Enter NEW height: " << endl;
    cin >> new_Height;
    return new_Height;
}

void resize_output(Image &img)
{
    cout << ">>> ACTION: Resize Image <<<" << endl;
    int width = get_width();
    int height = get_height();
    img = resize_image(img, width, height);
}

Image merge_output(Image &image1, string &filename)
{
    string filename2;
    Image image2 = get_image(filename2);
    Image merged = merge_image(image1, image2);
    return merged;
}

Image rotate_output(Image image)
{
    int angle;
    cout << "Enter rotation angle (90 / 180 / 270): ";
    cin >> angle;
    Image rotated = rotate(image, angle);
    return rotated;
}

void undo(vector<Image> &history, int &current)
{
    if (current > 0)
    {
        current--;
        cout << "< UNDO: Last filter reverted. >" << endl;
    }
    else
    {
        cout << "Cannot undo: Already at the first image." << endl;
    }
}
void redo(vector<Image> &history, int &current)
{
    if (current < history.size() - 1)
    {
        current++;
        cout << "< REDO: Last filter Applied. >" << endl;
    }
    else
    {
        cout << "Cannot redo: Already at the latest image." << endl;
    }
}

void main_menu(unsigned long long size, string &filename)
{
    cout << "================================================================================" << endl;
    cout << "                                BABY PHOTOSHOP (C++) " << endl;
    cout << "================================================================================" << endl;
    cout << " CURRENT IMAGE: [" << filename << "]  |  HISTORY DEPTH: " << size - 1 << endl;
    cout << "--------------------------------------------------------------------------------" << endl;
    cout << "                                MAIN MENU" << endl;
    cout << "--------------------------------------------------------------------------------" << endl;
    cout << "1- Load a new image\n"
            "2- List Filters\n"
            "3- Save Current Image\n"
            "4- Exit"
         << endl;
    cout << ">> Enter your choice (1-4): _";
}

void save_image(string &filename, Image &image, int save_option)
{
    while (true)
    {
        cout << "1- Save image on the same file\n"
                "2- Save image on a new file"
             << endl;
        cout << ">> Enter your choice (1-2): _";
        cin >> save_option;
        if (save_option == 1)
        {
            image.saveImage(filename);
            break;
        }
        else if (save_option == 2)
        {
            bool saved = false;
            string new_filename;
            while (!saved)
            {
                cout << "Enter image path (.jpg, .bmp, .png, .tga): ";
                cin >> new_filename;
                try
                {
                    image.saveImage(new_filename);
                    saved = true;
                }
                catch (invalid_argument &e)
                {
                    cout << "Error: " << e.what() << endl;
                }
            }
            break;
        }
        else
        {
            cout << "Invalid Option." << endl;
        }
    }
    cout << ">>> Saved Successfully <<<" << endl;
}
enum Menu_Options
{
    LOAD = 1,
    LIST,
    SAVE,
    EXIT
};
enum Filter_Options
{
    GRAY = 1,
    BLACK_WHITE,
    INVERT,
    RESIZE,
    MERGE,
    ROTATE,
    UNDO,
    REDO,
    RETURN
};
int main()
{
    string filename;
    vector<Image> history;
    Image image = get_image(filename);

    history.push_back(image);
    int option = 0, filter_option = 0, save_option = 0, current = 0;
    while (option != 4)
    {
        main_menu(history.size(), filename);
        cin >> option;
        switch (option)
        {
        case LOAD:
            image = get_image(filename);
            break;
        case LIST:
            filter_option = 0;
            while (filter_option != 9)
            {
                list_filter();
                cin >> filter_option;
                switch (filter_option)
                {
                case GRAY:
                    gray_scale(image);
                    break;
                case BLACK_WHITE:
                    Black_White(image);
                    break;
                case INVERT:
                    invert(image);
                    break;
                case RESIZE:
                    resize_output(image);
                    break;
                case MERGE:
                    image = merge_output(image, filename);
                    break;
                case ROTATE:
                    image = rotate_output(image);
                    break;
                case UNDO:
                    undo(history, current);
                    image = history[current];
                    continue;
                case REDO:
                    redo(history, current);
                    image = history[current];
                    continue;
                case RETURN:
                    cout << ">>> Returning to main menu <<<" << endl;
                    break;
                default:
                    cout << "Invalid Option." << endl;
                }
                if (filter_option >= 1 && filter_option <= 6)
                {
                    if (current < history.size() - 1)
                    {
                        history.erase(history.begin() + current + 1, history.end());
                    }
                    current++;
                    history.push_back(image);
                    cout << "Applied Successfully :)" << endl;
                }
            }
            break;
        case SAVE:
            save_image(filename, image, save_option);
            break;
        case EXIT:
            cout << "<<< Bye :) >>>" << endl;
            break;
        default:
            cout << "Invalid Option.";
        }
    }

    return 0;
}