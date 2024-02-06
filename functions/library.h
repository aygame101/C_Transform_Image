typedef struct
{
    int width;
    int height;
    unsigned char *pixels;
} GrayImage;

GrayImage loadPGM(const char *filename);
void savePGM(const char *filename, const GrayImage *image);
GrayImage mirrorImage(const GrayImage *image);
GrayImage rotateImage(const GrayImage *image, int angle);
GrayImage translateImage(const GrayImage *image, int x);
GrayImage scaleImage(const GrayImage *image, double scale_factor);
GrayImage adjustContrast(const GrayImage *image, double contrast_factor);
GrayImage adjustBrightness(const GrayImage *image, double brightness_factor);
GrayImage thresholdImage(const GrayImage *image, unsigned char threshold);
GrayImage applyBlur(const GrayImage *image, int blurAmount);
GrayImage pixelizeImage(const GrayImage *image, int pixelWidth, int pixelHeight);

void welcome();

void choix();