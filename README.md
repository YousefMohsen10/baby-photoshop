# Baby Photoshop 🖼️

## Overview
**Baby Photoshop** is a C++ desktop application that simulates basic image editing operations similar to those found in professional photo editing software.  
It allows users to apply filters, transformations, and enhancements to images using various image processing algorithms implemented from scratch.  

---

## Features
- Convert images to grayscale  
- Apply black and white filters  
- Adjust brightness and contrast  
- Detect and enhance edges  
- Merge multiple images  
- Apply blur and sharpen effects  
- Flip and rotate images  
- Add creative effects (e.g., invert colors, crop, resize)

---

## Technologies Used
- **Language:** C++  
- **Libraries:** Standard Template Library (STL), custom Image Class, Qt 6  
- **Concepts:** Image processing, file I/O, arrays, pixel manipulation, GUI development

---

## How It Works
1. The program reads an image file and stores its pixel data.  
2. Each filter or operation manipulates the image pixels according to its algorithm.  
3. The modified image can be displayed in the GUI or saved as a new file.  

## How to Run
Clone the repository:

```bash
git clone https://github.com/AlaaOkasha360/Baby-Photoshop.git
```
Compile the project:

```bash
g++ main.cpp -o baby_photoshop
```
Run the executable:

```bash
./baby_photoshop
```
Note: This runs the console version of the app, useful for testing algorithms without the GUI.

## Running the GUI Version (Qt)
 
1- Install Qt 6 and Qt Creator if you haven’t already.

2- Open Qt Creator and select Open Project, then open the Baby-Photoshop.pro or CMakeLists.txt file.

3- Configure the project for your compiler/kit (e.g., Desktop Qt 6).

4- Click Build → Run to launch the GUI application.

5- Use the GUI to load images and apply filters, transformations, and effects interactively.

Tip: All sliders, buttons, and menus update the image in real-time. Remember to click “Apply” to save changes.

## Screenshots

## Credits
* Developed by: Alaa Mohamed Okasha, Sayed Ashraf Sayed, Yousef Mohsen Mahmoud

* Faculty: Faculty of Computing and Artificial Intelligence, Cairo University

* Supervisor: Dr. Mohamed Elramly
