
/*
 * Copyright 2011 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */
#include "SkKernel33MaskFilter.h"
#include "SkColorPriv.h"
#include "SkFlattenableBuffers.h"

SkMask::Format SkKernel33ProcMaskFilter::getFormat() const {
    return SkMask::kA8_Format;
}

bool SkKernel33ProcMaskFilter::filterMask(SkMask* dst, const SkMask& src,
                                      const SkMatrix&, SkIPoint* margin) const {
    // margin???
    dst->fImage = NULL;
    dst->fBounds = src.fBounds;
    dst->fBounds.inset(-1, -1);
    dst->fFormat = SkMask::kA8_Format;

    if (NULL == src.fImage) {
        return true;
    }

    dst->fRowBytes = dst->fBounds.width();
    size_t size = dst->computeImageSize();
    if (0 == size) {
        return false;   // too big to allocate, abort
    }
    dst->fImage = SkMask::AllocImage(size);

    const int h = src.fBounds.height();
    const int w = src.fBounds.width();
    const int srcRB = src.fRowBytes;
    const uint8_t* srcImage = src.fImage;
    uint8_t* dstImage = dst->fImage;

    uint8_t* srcRows[3];
    uint8_t storage[3][3];

    srcRows[0] = storage[0];
    srcRows[1] = storage[1];
    srcRows[2] = storage[2];

    unsigned scale = fPercent256;

    for (int y = -1; y <= h; y++) {
        uint8_t* dstRow = dstImage;
        for (int x = -1; x <= w; x++) {
            memset(storage, 0, sizeof(storage));
            uint8_t* storagePtr = &storage[0][0];

            for (int ky = y - 1; ky <= y + 1; ky++) {
                const uint8_t* srcRow = srcImage + ky * srcRB;  // may be out-of-range
                for (int kx = x - 1; kx <= x + 1; kx++) {
                    if ((unsigned)ky < (unsigned)h && (unsigned)kx < (unsigned)w) {
                        *storagePtr = srcRow[kx];
                    }
                    storagePtr++;
                }
            }
            int value = this->computeValue(srcRows);

            if (scale < 256) {
                value = SkAlphaBlend(value, srcRows[1][1], scale);
            }
            *dstRow++ = SkToU8(value);
        }
        dstImage += dst->fRowBytes;
    }
    return true;
}

void SkKernel33ProcMaskFilter::flatten(SkFlattenableWriteBuffer& wb) const {
    this->INHERITED::flatten(wb);
    wb.writeInt(fPercent256);
}

SkKernel33ProcMaskFilter::SkKernel33ProcMaskFilter(SkFlattenableReadBuffer& rb)
        : SkMaskFilter(rb) {
    fPercent256 = rb.readInt();
}

///////////////////////////////////////////////////////////////////////////////

uint8_t SkKernel33MaskFilter::computeValue(uint8_t* const* srcRows) const {
    int value = 0;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            value += fKernel[i][j] * srcRows[i][j];
        }
    }

    value >>= fShift;

    if (value < 0) {
        value = 0;
    } else if (value > 255) {
        value = 255;
    }
    return (uint8_t)value;
}

void SkKernel33MaskFilter::flatten(SkFlattenableWriteBuffer& wb) const {
    this->INHERITED::flatten(wb);
    wb.writeIntArray(&fKernel[0][0], 9);
    wb.writeInt(fShift);
}

SkKernel33MaskFilter::SkKernel33MaskFilter(SkFlattenableReadBuffer& rb)
        : SkKernel33ProcMaskFilter(rb) {
    const uint32_t count = rb.readIntArray(&fKernel[0][0]);
    SkASSERT(9 == count);
    fShift = rb.readInt();
}

