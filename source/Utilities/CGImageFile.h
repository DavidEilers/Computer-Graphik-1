#ifndef CG_IMAGEFILE_H
#define CG_IMAGEFILE_H

extern int cgImageWriteTGA(const char *filename, int w, int h, const unsigned char *data, int rle);
extern unsigned char *cgImageLoadTGA(const char *filename, int *w, int *h, unsigned char orientation);

extern int cgImageWrite(const char *filename, unsigned char *data, int width, int height);
extern unsigned char *cgImageLoad(const char *filename, int *width, int *height);

#endif
