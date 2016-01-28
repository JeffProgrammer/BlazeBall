#ifdef __cplusplus
extern "C" {
#endif

#define TGA_ERROR_FILE_OPEN 1
#define TGA_ERROR_READING_FILE 2
#define TGA_ERROR_INDEXED_COLOR 3
#define TGA_ERROR_MEMORY 4
#define TGA_ERROR_COMPRESSED_FILE 5
#define TGA_OK 0

typedef struct {
	int status;
	unsigned char type, pixelDepth;
	short int width, height;
	unsigned char *imageData;
} tgaInfo;


void tgaLoadHeader(FILE *file, tgaInfo *info);
void tgaLoadImageData(FILE *file, tgaInfo *info);
void tgaRGBtogreyscale(tgaInfo *info);
void tgaDestroy(tgaInfo *info);
tgaInfo *tgaLoad(const char *filename);
int tgaSaveSeries(char		*filename, 
                  short int		width, 
                  short int		height, 
                  unsigned char	pixelDepth,
                  unsigned char	*imageData);
int tgaSave(char 		*filename, 
            short int	width, 
            short int	height, 
            unsigned char	pixelDepth,
            unsigned char	*imageData);

#ifdef __cplusplus
}
#endif
