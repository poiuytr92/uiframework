// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "gfx/gdi_util.h"

#include "base/memory/scoped_ptr.h"

namespace gfx {

void CreateBitmapHeader(int width, int height, BITMAPINFOHEADER* hdr) {
  CreateBitmapHeaderWithColorDepth(width, height, 32, hdr);
}

void CreateBitmapHeaderWithColorDepth(int width, int height, int color_depth,
                                      BITMAPINFOHEADER* hdr) {
  // These values are shared with gfx::PlatformDevice
  hdr->biSize = sizeof(BITMAPINFOHEADER);
  hdr->biWidth = width;
  hdr->biHeight = -height;  // minus means top-down bitmap
  hdr->biPlanes = 1;
  hdr->biBitCount = color_depth;
  hdr->biCompression = BI_RGB;  // no compression
  hdr->biSizeImage = 0;
  hdr->biXPelsPerMeter = 1;
  hdr->biYPelsPerMeter = 1;
  hdr->biClrUsed = 0;
  hdr->biClrImportant = 0;
}


void CreateBitmapV4Header(int width, int height, BITMAPV4HEADER* hdr) {
  // Because bmp v4 header is just an extension, we just create a v3 header and
  // copy the bits over to the v4 header.
  BITMAPINFOHEADER header_v3;
  CreateBitmapHeader(width, height, &header_v3);
  memset(hdr, 0, sizeof(BITMAPV4HEADER));
  memcpy(hdr, &header_v3, sizeof(BITMAPINFOHEADER));

  // Correct the size of the header and fill in the mask values.
  hdr->bV4Size = sizeof(BITMAPV4HEADER);
  hdr->bV4RedMask   = 0x00ff0000;
  hdr->bV4GreenMask = 0x0000ff00;
  hdr->bV4BlueMask  = 0x000000ff;
  hdr->bV4AlphaMask = 0xff000000;
}

// Creates a monochrome bitmap header.
void CreateMonochromeBitmapHeader(int width,
                                  int height,
                                  BITMAPINFOHEADER* hdr) {
  hdr->biSize = sizeof(BITMAPINFOHEADER);
  hdr->biWidth = width;
  hdr->biHeight = -height;
  hdr->biPlanes = 1;
  hdr->biBitCount = 1;
  hdr->biCompression = BI_RGB;
  hdr->biSizeImage = 0;
  hdr->biXPelsPerMeter = 1;
  hdr->biYPelsPerMeter = 1;
  hdr->biClrUsed = 0;
  hdr->biClrImportant = 0;
}

void SubtractRectanglesFromRegion(HRGN hrgn,
                                  const std::vector<gfx::Rect>& cutouts) {
  if (cutouts.size()) {
    HRGN cutout = ::CreateRectRgn(0, 0, 0, 0);
    for (size_t i = 0; i < cutouts.size(); i++) {
      ::SetRectRgn(cutout,
                   cutouts[i].x(),
                   cutouts[i].y(),
                   cutouts[i].right(),
                   cutouts[i].bottom());
      ::CombineRgn(hrgn, hrgn, cutout, RGN_DIFF);
    }
    ::DeleteObject(cutout);
  }
}

HRGN ConvertPathToHRGN(const gfx::Path& path) {
#if defined(USE_AURA)
  int point_count = path.getPoints(NULL, 0);
  scoped_array<SkPoint> points(new SkPoint[point_count]);
  path.getPoints(points.get(), point_count);
  scoped_array<POINT> windows_points(new POINT[point_count]);
  for (int i = 0; i < point_count; ++i) {
    windows_points[i].x = SkScalarRound(points[i].fX);
    windows_points[i].y = SkScalarRound(points[i].fY);
  }

  return ::CreatePolygonRgn(windows_points.get(), point_count, ALTERNATE);
#elif defined(OS_WIN)
  return path.CreateNativeRegion();
#endif
}


double CalculatePageScale(HDC dc, int page_width, int page_height) {
  int dc_width = GetDeviceCaps(dc, HORZRES);
  int dc_height = GetDeviceCaps(dc, VERTRES);

  // If page fits DC - no scaling needed.
  if (dc_width >= page_width && dc_height >= page_height)
    return 1.0;

  double x_factor =
      static_cast<double>(dc_width) / static_cast<double>(page_width);
  double y_factor =
      static_cast<double>(dc_height) / static_cast<double>(page_height);
  return std::min(x_factor, y_factor);
}

// Apply scaling to the DC.
bool ScaleDC(HDC dc, double scale_factor) {
  SetGraphicsMode(dc, GM_ADVANCED);
  XFORM xform = {0};
  xform.eM11 = xform.eM22 = scale_factor;
  return !!ModifyWorldTransform(dc, &xform, MWT_LEFTMULTIPLY);
}

}  // namespace gfx
