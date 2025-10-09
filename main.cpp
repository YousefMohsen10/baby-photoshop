#include <iostream>
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

Image merge_image(Image image1, Image image2, const string &option)
{

    int merge_width, merge_height;
    if (option == "resize")
    {
        merge_width = max(image1.width, image2.width);
        merge_height = max(image1.height, image2.height);
        image1 = resize_image(image1, merge_width, merge_height);
        image2 = resize_image(image2, merge_width, merge_height);
    }
    else if (option == "overlap")
    {
        merge_width = min(image1.width, image2.width);
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

void skewImage(Image &image, double angle = 0)
{
    cout << "Enter angle of skewing: ";
    cin >> angle;
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

void sunlight_glow(Image &image, float warmth = 0.25f, float brightness = 0.15f, float contrast = 1.1f)
{
    int width = image.width;
    int height = image.height;

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
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

void purple_scale(Image &image)
{
    cout << ">>> Applying Filter: Night Purple <<<" << endl;
    float redBoost = 1.2f;
    float greenScale = 0.8f;
    float blueBoost = 1.2f;

    for (int x = 0; x < image.width; x++)
    {
        for (int y = 0; y < image.height; y++)
        {

            unsigned int R = image(x, y, 0);
            unsigned int G = image(x, y, 1);
            unsigned int B = image(x, y, 2);

            int newR = min(255, int(R * redBoost));
            int newG = max(0, int(G * greenScale));
            int newB = min(255, int(B * blueBoost));

            image(x, y, 0) = (unsigned int)newR;
            image(x, y, 1) = (unsigned int)newG;
            image(x, y, 2) = (unsigned int)newB;
        }
    }
}

void flip(Image &image)
{
    int flip_type;
    while (true)
    {
        cout << "Enter flipping option: \n"
                "1- Flip Vertical\n"
                "2- Flip Horizontal\n";
        cin >> flip_type;
        if (flip_type == 1 || flip_type == 2)
        {
            break;
        }
        else
        {
            cout << "Invalid Option." << endl;
        }
    }

    if (flip_type == 1)
    {
        for (int x = 0; x < image.width; x++)
        {
            for (int y = 0; y < image.height / 2; y++)
            {
                int oppositeY = image.height - 1 - y;
                for (int channel = 0; channel < 3; channel++)
                {
                    swap(image(x, y, channel), image(x, oppositeY, channel));
                }
            }
        }
    }
    else if (flip_type == 2)
    {
        for (int y = 0; y < image.height; y++)
        {
            for (int x = 0; x < image.width / 2; x++)
            {
                int oppositeX = image.width - 1 - x;
                for (int channel = 0; channel < 3; channel++)
                {
                    swap(image(x, y, channel), image(oppositeX, y, channel));
                }
            }
        }
    }
}

void Infra_Red(Image &image)
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

void darken_and_lighten(Image &image, double lvl = 0)
{
    while (true)
    {
        cout << "Enter level of brightness: ";
        cin >> lvl;
        lvl /= 100.0;
        if (lvl <= 0.0)
        {
            cout << "Invalid brightness level.\n";
        }
        else
        {
            break;
        }
    }

    for (int i = 0; i < image.width; i++)
    {
        for (int j = 0; j < image.height; j++)
        {
            for (int k = 0; k < image.channels; k++)
            {
                int new_brightness = image(i, j, k) * lvl;
                if (new_brightness < 0)
                    new_brightness = 0;
                if (new_brightness > 255)
                    new_brightness = 255;
                image(i, j, k) = new_brightness;
            }
        }
    }
}

Image edge_detection(Image &image)
{
    int width = image.width;
    int height = image.height;
    Image edges(width, height);
    int Gx[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}};

    int Gy[3][3] = {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}};

    for (int y = 1; y < height - 1; y++)
    {
        for (int x = 1; x < width - 1; x++)
        {
            int sumX = 0;
            int sumY = 0;
            for (int ky = -1; ky <= 1; ky++)
            {
                for (int kx = -1; kx <= 1; kx++)
                {
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
            if (magnitude >= 128 * 128)
                value = 0;
            else
                value = 255;
            for (int k = 0; k < 3; k++)
            {
                edges(x, y, k) = value;
            }
        }
    }

    return edges;
}

Image oil_painting(Image &image, int radius = 3, int intensityLevels = 35)
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

    cout << ">>> Oil Painting Filter Applied Successfully! <<<" << endl;

    return image;
}

Image crop_image(Image image, const int row, const int col, const int width, const int height)
{
    if (row + width > image.width || col + height > image.height)
    {
        throw out_of_range("Crop dimensions exceed image bounds");
    }

    Image cropped_image(width, height);
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            cropped_image(i, j, 0) = image(row + i, col + j, 0);
            cropped_image(i, j, 1) = image(row + i, col + j, 1);
            cropped_image(i, j, 2) = image(row + i, col + j, 2);
        }
    }
    return cropped_image;
}

Image crop_output(const Image &image)
{
    int row, col, width, height;
    Image cropped;
    while (true)
    {
        cout << ">> Enter the starting row: _";
        cin >> row;
        cout << ">> Enter the starting column: _";
        cin >> col;
        cout << ">> Enter width of the area: _";
        cin >> width;
        cout << ">> Enter height of the area: _";
        cin >> height;
        try
        {
            cropped = crop_image(image, row, col, width, height);
            break;
        }
        catch (out_of_range &e)
        {
            cout << e.what() << endl;
        }
    }
    return cropped;
}

Image get_image(string &filename)
{
    Image image;
    while (true)
    {
        cout << ">> Enter image path (.jpg, .bmp, .png, .tga): ";
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
    }
    cout << "* SUCCESS: Image '" << filename << "' loaded successfully." << endl;
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
            "4- Merge Image\n"
            "5- Flip Image\n"
            "6- Rotate\n"
            "7- Darken and Lighten\n"
            "8- Crop Image\n"
            "9- Edge Detection\n"
            "10- Resize Image\n"
            "11- Blur Filter\n"
            "12- Sunlight Effect\n"
            "13- Oil Painting\n"
            "14- Old TV Effect\n"
            "15- Purple Scale\n"
            "16- Infrared Effect\n"
            "17- Skew\n"
            "18- Undo last filter\n"
            "19- Redo last filter\n"
            "20- Return to Main Menu"
         << endl;
    cout << ">> Enter filter number (1-20): _";
}

int get_width()
{
    int new_Width;
    cout << ">> Enter NEW width: _" << endl;
    cin >> new_Width;
    return new_Width;
}

int get_height()
{
    int new_Height;
    cout << ">> Enter NEW height: _" << endl;
    cin >> new_Height;
    return new_Height;
}

void resize_output(Image &img)
{
    int width = get_width();
    int height = get_height();
    img = resize_image(img, width, height);
}

Image merge_output(Image &image1, string &filename)
{
    string filename2;
    Image image2 = get_image(filename2);
    int merge_option;
    cout << "\n--------------------------------------------------" << endl;
    cout << "  [MERGE IMAGE] - Select Alignment Strategy" << endl;
    cout << "--------------------------------------------------" << endl;
    while (true)
    {
        cout << "  [1] RESIZE: Enlarge both images to the largest dimensions." << endl;
        cout << "  [2] OVERLAP: Use only the minimum shared area." << endl;
        cout << ">> Enter option (1 or 2): _";
        cin >> merge_option;
        if (merge_option == 1 || merge_option == 2)
        {
            break;
        }
        else
        {
            cout << "Invalid Option" << endl;
        }
    }
    string option = (merge_option == 1 ? "resize" : "overlap");
    Image merged = merge_image(image1, image2, option);
    return merged;
}

Image rotate_output(Image image)
{
    int angle;
    cout << ">> Enter rotation angle (90 / 180 / 270): _";
    cin >> angle;
    Image rotated = rotate(image, angle);
    return rotated;
}

void blur_output(Image &image)
{
    int percent;
    while (true)
    {
        cout << ">> Enter Blur Percentage(0->100): _";
        cin >> percent;
        if (percent >= 0 && percent <= 100)
        {
            break;
        }
        else
        {
            cout << "--- ERROR --- Invalid Blur Percentage." << endl;
        }
    }
    percent = max(1, min(100, percent));
    double ratio = percent / 100.0;
    int radius = max(1, int(min(image.width, image.height) * ratio / 80.0));
    Image temp = blur_image(image, radius, true);
    image = blur_image(temp, radius, false);
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
void redo(const vector<Image> &history, int &current)
{
    if (current < history.size() - 1)
    {
        current++;
        cout << "< REDO: Last filter applied. >" << endl;
    }
    else
    {
        cout << "Cannot redo: Already at the latest image." << endl;
    }
}

string split(string S1)
{

    string delim = "\\";
    short pos = 0;
    string sWord;
    while ((pos = S1.find(delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos);
        S1.erase(0, pos + delim.length());
    }
    return S1;
}

void main_menu(const unsigned long long size, const string &filename)
{
    cout << "================================================================================" << endl;
    cout << "                                BABY PHOTOSHOP (C++) " << endl;
    cout << "================================================================================" << endl;
    cout << " CURRENT IMAGE: [" << split(filename) << "]  |  HISTORY DEPTH: " << size - 1 << endl;
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

void save_image(const string &filename, Image &image, int save_option)
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
            cout << "--- ERROR --- Invalid Option." << endl;
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
    MERGE,
    FLIP,
    ROTATE,
    DARKEN_LIGHTEN,
    CROP,
    EDGE_DETECT,
    RESIZE,
    BLUR,
    SUNLIGHT,
    OIL_PAINT,
    OLD_TV,
    PURPLE,
    INFRARED,
    SKEW,
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
            history.clear();
            history.push_back(image);
            current = 0;
            break;
        case LIST:
            filter_option = 0;
            while (filter_option != 20)
            {
                list_filter();
                cin >> filter_option;
                switch (filter_option)
                {
                case GRAY:
                    cout << ">>> Applying Filter: Gray Scale <<<" << endl;
                    gray_scale(image);
                    break;
                case BLACK_WHITE:
                    cout << ">>> Applying Filter: Black & White <<<" << endl;
                    Black_White(image);
                    break;
                case INVERT:
                    cout << ">>> Applying Filter: Invert <<<" << endl;
                    invert(image);
                    break;
                case MERGE:
                    image = merge_output(image, filename);
                    break;
                case FLIP:
                    flip(image);
                    break;
                case ROTATE:
                    image = rotate_output(image);
                    break;
                case DARKEN_LIGHTEN:
                    darken_and_lighten(image);
                    break;
                case CROP:
                    image = crop_output(image);
                    break;
                case EDGE_DETECT:
                    cout << ">>> Applying Filter: Edge Detection <<<" << endl;
                    image = edge_detection(image);
                    break;
                case RESIZE:
                    cout << ">>> ACTION: Resize Image <<<" << endl;
                    resize_output(image);
                    break;
                case BLUR:
                    blur_output(image);
                    break;
                case SUNLIGHT:
                    cout << ">>> Applying Filter: Sunlight Glow <<<" << endl;
                    sunlight_glow(image);
                    break;
                case OIL_PAINT:
                    cout << ">>> Applying Filter: Oil Painting <<<" << endl;
                    oil_painting(image);
                    break;
                case OLD_TV:
                    cout << ">>> Applying Filter: Old TV <<<" << endl;
                    oldTVFilter(image);
                    break;
                case PURPLE:
                    cout << ">>> Applying Filter: Night Purple <<<" << endl;
                    purple_scale(image);
                    break;
                case INFRARED:
                    cout << ">>> Applying Filter: Infrared <<<" << endl;
                    Infra_Red(image);
                    break;
                case SKEW:
                    skewImage(image);
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
                    cout << "--- ERROR --- Invalid Option." << endl;
                }
                if (filter_option >= 1 && filter_option <= 17)
                {
                    if (current < history.size() - 1)
                    {
                        history.erase(history.begin() + current + 1, history.end());
                    }
                    current++;
                    history.push_back(image);
                    cout << "< Applied Successfully :). >" << endl;
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
            cout << "--- ERROR --- Invalid Option.";
        }
    }

    return 0;
}