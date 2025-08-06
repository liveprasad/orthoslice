# How to Compile and Run

**1. Compile the program**
```sh
g++ a4.cpp -std=c++11
```

**2. Run the program**
```sh
./a.out in3
```
- `in3` is your input file name.

---

# Output Folder Setup

1. Create 3 empty folders in the same directory as `a4.cpp`:
    - `xy`
    - `yx`
    - `zy`
2. Before each run, make sure these folders are empty to avoid mixing outputs.
3. Output files will be saved in these folders:
    - `xy`: slices along the z-axis (xy-plane projections)
    - `yx`, `zy`: similar for their axes
4. Output file format:  
   `<projectionPlane>_<sliceNumber>_<gridcellsize>_<yvalue>`

---

# Input Instructions

1. The first input parameter is the scale factor (to scale the object for SVG output).
2. The program will ask for the slice size for each projection plane (in this order):
    - xz (along y)
    - yx (along z)
    - zy (along x)
3. The program will ask for the grid size factor for each plane.
4. You’ll be prompted for these values at runtime.

---

# Input File Format (`in3`)

- The input file must be in the same directory.
- Each line contains 3 coordinates:  
  `x y z`
- No extra space at the end of the last line.

**Example:**
```
1.0 2.0 3.0
4.5 6.1 2.2
0.0 0.0 0.0
```

---

# Steps

1. Compile:  
   `g++ a4.cpp -std=c++11`
2. Create empty folders: `xy`, `yx`, `zy`
3. Prepare your input file: `in3`
4. Run:  
   `./a.out in3`
5. Follow prompts for scale factor, slice sizes, and grid size factors.
6. Check output slices in the appropriate folders.