#ifndef ZELDA3_SNES_PPU_H_
#define ZELDA3_SNES_PPU_H_

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include "saveload.h"

#include "../types.h"

typedef struct BgLayer {
	uint16_t hScroll;
	uint16_t vScroll;
	// -- snapshot starts here
	bool tilemapWider;
	bool tilemapHigher;
	uint16_t tilemapAdr;
	// -- snapshot ends here
	uint16_t tileAdr;
} BgLayer;

enum {
	kPpuXPixels = 256 + kPpuExtraLeftRight * 2,
};

typedef uint16_t PpuZbufType;

typedef struct PpuPixelPrioBufs {
	// This holds the prio in the upper 8 bits and the color in the lower 8 bits.
	PpuZbufType data[kPpuXPixels];
} PpuPixelPrioBufs;

enum {
	kPpuRenderFlags_NewRenderer = 1,
	// Render mode7 upsampled by 4x4
	kPpuRenderFlags_4x4Mode7 = 2,
	// Use 240 height instead of 224
	kPpuRenderFlags_Height240 = 4,
	// Disable sprite render limits
	kPpuRenderFlags_NoSpriteLimits = 8,
};


class Ppu {
public:
	bool lineHasSprites;
	uint8_t lastBrightnessMult;
	uint8_t lastMosaicModulo;
	uint8_t renderFlags;
	uint32_t renderPitch;
	uint8_t *renderBuffer;
	uint8_t extraLeftCur, extraRightCur, extraLeftRight, extraBottomCur;
	float mode7PerspectiveLow, mode7PerspectiveHigh;

	// TMW / TSW etc
	uint8 screenEnabled[2];
	uint8 screenWindowed[2];
	uint8 mosaicEnabled;
	uint8 mosaicSize;
	// object/sprites
	uint16_t objTileAdr1;
	uint16_t objTileAdr2;
	uint8_t objSize;
	// Window
	uint8_t window1left;
	uint8_t window1right;
	uint8_t window2left;
	uint8_t window2right;
	uint32_t windowsel;

	// color math
	uint8_t clipMode;
	uint8_t preventMathMode;
	bool addSubscreen;
	bool subtractColor;
	bool halfColor;
	uint8 mathEnabled;
	uint8_t fixedColorR, fixedColorG, fixedColorB;
	// settings
	bool forcedBlank;
	uint8_t brightness;
	uint8_t mode;

	// vram access
	uint16_t vramPointer;
	uint16_t vramIncrement;
	bool vramIncrementOnHigh;
	// cgram access
	uint8_t cgramPointer;
	bool cgramSecondWrite;
	uint8_t cgramBuffer;
	// oam access
	uint16_t oamAdr;
	bool oamSecondWrite;
	uint8_t oamBuffer;

	// background layers
	BgLayer bgLayer[4];
	uint8_t scrollPrev;
	uint8_t scrollPrev2;

	// mode 7
	int16_t m7matrix[8]; // a, b, c, d, x, y, h, v
	uint8_t m7prev;
	bool m7largeField;
	bool m7charFill;
	bool m7xFlip;
	bool m7yFlip;
	bool m7extBg_always_zero;
	// mode 7 internal
	int32_t m7startX;
	int32_t m7startY;

	uint16_t oam[0x110];

	// store 31 extra entries to remove the need for clamp
	uint8_t brightnessMult[32 + 31];
	uint8_t brightnessMultHalf[32 * 2];
	uint16_t cgram[0x100];
	uint8_t mosaicModulo[kPpuXPixels];
	uint32_t colorMapRgb[256];
	PpuPixelPrioBufs bgBuffers[2];
	PpuPixelPrioBufs objBuffer;
	uint16_t vram[0x8000];

	void reset();

	void SaveLoad(SaveLoadFunc *func, void *ctx);

	void BeginDrawing(uint8_t *buffer, size_t pitch, uint32_t render_flags);

	// Returns the current render scale, 1x = 256px, 2x=512px, 4x=1024px
	[[nodiscard]] int GetCurrentRenderScale(uint32_t render_flags) const;

	uint8_t read(uint8_t adr);

	void write(uint8_t adr, uint8_t val);

	// Drawing
	NOINLINE void DrawWholeLine(uint y);

	void DrawBackgrounds(int y, bool sub);
	void DrawBackground_mode7(uint y, bool sub, PpuZbufType z);
	void DrawMode7Upsampled(uint y);
	void DrawSprites(uint y, uint sub, bool clear_backdrop);
	void DrawBackground_2bpp_mosaic(int y, bool sub, uint layer, PpuZbufType zhi, PpuZbufType zlo);
	void DrawBackground_4bpp_mosaic(uint y, bool sub, uint layer, PpuZbufType zhi, PpuZbufType zlo);
	void DrawBackground_2bpp(uint y, bool sub, uint layer, PpuZbufType zhi, PpuZbufType zlo);
	void DrawBackground_4bpp(uint y, bool sub, uint layer, PpuZbufType zhi, PpuZbufType zlo);

	static inline void ClearBackdrop(PpuPixelPrioBufs *buf);

	void runLine(int line);

	void SetMode7PerspectiveCorrection(int low, int high);

	void SetExtraSideSpace(int left, int right, int bottom);

	void handlePixel(int x, int y);

	int getPixel(int x, int y, bool sub, int *r, int *g, int *b, int *pixel);

	int getPixelForBgLayer(int x, int y, int layer, bool priority);

	void calculateMode7Starts(int y);

	int getPixelForMode7(int x, int layer, bool priority);

	[[nodiscard]] bool getWindowState(int layer, int x) const;

	bool evaluateSprites(int line);
};

Ppu *ppu_init();

void ppu_free(Ppu *ppu);



#endif  // ZELDA3_SNES_PPU_H_