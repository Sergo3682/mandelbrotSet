#[Archive] My take on the mandelbrot set.
**The task was accomplished during an internship at [Milandr](https://www.milandr.com/) in 2022.**

### Usage guide:
```
gcc main.c -Wall -o app
```
Run it without arguments to get usage guide:
```
./app
```
```
Expected input format:
<output_file_name> <width> <heigth> <BG_BLUE_0..255> <BG_GREEN_0..255> <BG_RED_0..255>
```
| Argument | Explanation  |
|:--:|:--:|
| ```<output_file_name> ``` | Name of the result bmp file. Must contain *.bmp extention in it. |
| ``` <width> ``` | Result image size width in pixels. Width **must** equal Height. |
| ``` <heigth> ``` | Result image size height in pixels. Width **must** equal Height. |
| ``` <BG_BLUE_0..255> <BG_GREEN_0..255> <BG_RED_0..255>  ``` | Color of image background. |