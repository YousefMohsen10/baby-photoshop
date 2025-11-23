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
<img width="1636" height="1022" alt="Screenshot 2025-11-23 170425" src="https://github.com/user-attachments/assets/fb97cf05-4ceb-46f0-ac0c-49c1cdd93a50" />
<img width="1630" height="1021" alt="Screenshot 2025-11-23 170745" src="https://github.com/user-attachments/assets/d7bb886b-ba8d-48a1-97da-d45d856b1fed" />
<img width="1627" height="1022" alt="image" src="https://github.com/user-attachments/assets/819af143-0c2c-4805-8c82-3e0c631f0367" />
<img width="1635" height="1016" alt="image" src="https://github.com/user-attachments/assets/a627c664-a968-4ce7-ab67-db9c8dfe2254" />
<img width="1637" height="1017" alt="image" src="https://github.com/user-attachments/assets/186ecf52-25a9-40f0-a032-dd10b97b5270" />



## Credits
* Developed by: Alaa Mohamed Okasha, Sayed Ashraf Sayed Hassan, Yousef Mohsen Mahmoud

* Faculty: Faculty of Computing and Artificial Intelligence, Cairo University

* Supervisor: Dr. Mohamed Elramly
