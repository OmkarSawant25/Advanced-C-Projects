# 🖼️ Steganography using LSB Technique (BMP Image Encoding and Decoding)

## 📘 Project Overview

This project implements **image steganography** using the **Least Significant Bit (LSB)** technique in the C programming language.
It allows users to **hide (encode)** a secret text or code file inside a `.bmp` image and later **extract (decode)** the hidden information safely without visible distortion of the image.

The system supports multiple file types (e.g. `.txt`, `.c`, `.h`, `.sh`) and performs full data integrity checks during encoding and decoding.

## 🧩 Features

* 🔒 **Secure Data Hiding** using LSB bit manipulation.
* 🖼️ Supports **24-bit BMP images**.
* 📄 Handles multiple **file types** (`.txt`, `.c`, `.h`, `.sh`).
* ✅ **Robust validation** for file names, extensions, and image capacity.
* 🔍 **Magic string verification** to ensure correct decoding.
* 🧠 Modular C code with clear function separation for readability.
* 💡 Detailed console messages for easy debugging and understanding.

## ⚙️ System Architecture

The project consists of the following source and header files:

| File       | Purpose                                                                       |
| ---------- | ----------------------------------------------------------------------------- |
| `main.c`   | Entry point; controls encode/decode operations                                |
| `encode.c` | Contains all encoding logic                                                   |
| `encode.h` | Header file defining structures and prototypes for encoding                   |
| `decode.c` | Contains all decoding logic                                                   |
| `decode.h` | Header file defining structures and prototypes for decoding                   |
| `common.h` | Defines constants like `MAGIC_STRING` and enums for `Status`, `OperationType` |
| `types.h`  | Contains typedefs for data types like `uint` and `Status`                     |


## 🧠 Working Principle (LSB Encoding)

The **Least Significant Bit (LSB)** of each pixel byte is modified to store the secret data bits.

For example:

| Original Byte | Binary   | After Encoding a bit ‘1’ |
| ------------- | -------- | ------------------------ |
| 240           | 11110000 | 11110001                 |

Each character of the secret file (8 bits) is hidden in the 8 least significant bits of 8 consecutive image bytes.


## 🧮 Encoding Steps

1. **Validate Input Files**

   * Source image must be `.bmp`
   * Secret file can be `.txt`, `.c`, `.h`, `.sh`
2. **Open Required Files** (`src.bmp`, `secret.txt`, `stego.bmp`)
3. **Check Capacity** — Ensure image can hold the secret data.
4. **Copy BMP Header** (first 54 bytes unchanged)
5. **Encode the following sequentially:**

   * Magic string (e.g., `#*`)
   * Secret file extension size
   * Secret file extension (e.g., `.txt`)
   * Secret file size
   * Secret file data (actual contents)
6. **Copy Remaining Image Data** after encoding.
7. **Output:** Stego image (`destination.bmp`) containing the hidden data.


## 🔍 Decoding Steps

1. **Validate and Open Stego Image**
2. **Skip BMP Header (54 bytes)**
3. **Read and Verify Magic String**
4. **Decode Extension Size**
5. **Decode Extension Name**
6. **Decode Secret File Size**
7. **Extract Secret File Data** and write to decoded file.


## 🧰 Data Structures

### `EncodeInfo` (from `encode.h`)

typedef struct _EncodeInfo {
    char *src_image_fname;
    FILE *fptr_src_image;
    uint image_capacity;

    char *secret_fname;
    FILE *fptr_secret;
    char extn_secret_file[5];
    char secret_data[100000];
    long size_secret_file;

    char *stego_image_fname;
    FILE *fptr_stego_image;
} EncodeInfo;

### `DecodeInfo` (from `decode.h`)

typedef struct _DecodeInfo {
    char *stego_image_fname;
    FILE *fptr_stego_image;

    char *secret_fname;
    FILE *fptr_secret;
    long ext_size;
    char extn_secret_file[5];
    long size_secret_file;
} DecodeInfo;


## 🚀 Usage

### 🧭 Command Format

./steg -e <source_image.bmp> <secret_file.txt> [output_image.bmp]
./steg -d <stego_image.bmp> [output_file_name]

### 🔒 Encoding Example

./steg -e flower.bmp message.txt hidden.bmp

Output: `hidden.bmp` now contains the hidden file `message.txt`

### 🔓 Decoding Example

./steg -d hidden.bmp decoded

Output: Decoded file (e.g. `decoded.txt`) is created with original contents.


## 🧩 Example Flow

> ./steg -e source.bmp secret.txt output.bmp
Selected encoding operation.
Opened required files successfully.
Source image has sufficient capacity.
BMP header copied successfully.
Magic string encoded successfully.
Secret file extension encoded successfully.
Secret file data encoded successfully.
Encoding completed successfully!

> ./steg -d output.bmp decoded
Selected decoding operation.
Decode arguments validated successfully.
Decoding completed successfully!
```


## ⚠️ Error Handling

* Invalid file names or extensions are reported.
* Missing arguments or corrupted BMP files trigger descriptive error messages.
* Magic string mismatch ensures you don’t decode unencoded files.


## 🧩 Future Enhancements

* Support for other image formats (PNG, JPEG).
* Password-based encryption before embedding.
* GUI-based front-end for user interaction.
* Batch encoding of multiple files.


## 🧑‍💻 Author

Omkar Ashok Sawant
📍 Banglore, Karnataka, India
💻 Enthusiastic C programmer specializing in embedded systems and data security projects.


