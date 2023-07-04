#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <png.h>


#define PAL_WHITE 0
#define PAL_BLACK 1
#define PAL_BLUE  2
#define PAL_GREEN 3
#define PALETTE_SIZE 4
const png_color PALETTE[PALETTE_SIZE] = {
  { 255, 255, 255 },
  {   0,   0,   0 },
  {  48,  80, 255 },
  {  48, 255,  48 }
};


int write_png(FILE* fp, int width, int height, const uint8_t* data) {
  // libpng initialization
  png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png_ptr) {
    fprintf(stderr, "[write_png]: png_create_write_struct failed\n");
    return 1;
  }

  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr) {
    fprintf(stderr, "[write_png]: png_create_info_struct failed\n");
    return 1;
  }

  if (setjmp(png_jmpbuf(png_ptr))) {
    fprintf(stderr, "[write_png]: png_init_io failed\n");
    return 1;
  }

  png_init_io(png_ptr, fp);

  // Write header
  if (setjmp(png_jmpbuf(png_ptr))) {
    fprintf(stderr, "[write_png]: png_write_info failed\n");
    return 1;
  }

  png_set_IHDR(png_ptr, info_ptr,
               width, height,
               8,
               PNG_COLOR_TYPE_PALETTE, //PNG_COLOR_TYPE_RGB
               PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE
  );

  png_set_PLTE(png_ptr, info_ptr, PALETTE, PALETTE_SIZE);

  png_write_info(png_ptr, info_ptr);

  // Write data
  if (setjmp(png_jmpbuf(png_ptr))) {
    fprintf(stderr, "[write_png]: png_write_row failed\n");
    return 1;
  }

  for (int r=0; r<height; r++) {
    png_write_row(png_ptr, data + r * width);
  }

  if (setjmp(png_jmpbuf(png_ptr))) {
    fprintf(stderr, "[write_png]: png_write_end failed\n");
    return 1;
  }

  png_write_end(png_ptr, NULL);

  return 0;
}


#define TEST_WIDTH 16
#define TEST_HEIGHT 8
int main(int argc, const char** argv) {
  int result = 0;

  const uint8_t pixels[TEST_HEIGHT][TEST_WIDTH] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
    {0, 1, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 1, 0},
    {0, 1, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 1, 0},
    {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  };

  const char* filename = "./test.png";
  FILE* fp = fopen(filename, "wb");
  if (!fp) {
    fprintf(stderr, "[main]: File %s could not be opened for writing\n", filename);
    return 1;
  }

  result |= write_png(fp, TEST_WIDTH, TEST_HEIGHT, &pixels[0][0]);
  fclose(fp);

  if (!result) {
    printf("[main]: File created: %s\n", filename);
  }

  return result;
}
