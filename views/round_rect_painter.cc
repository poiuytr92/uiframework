// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "views/round_rect_painter.h"

#include "gfx/canvas.h"
#include "gfx/path.h"
#include "gfx/skia_util.h"

namespace views {

static const int kRadius = 2;

RoundRectPainter::RoundRectPainter(SkColor border_color)
    : border_color_(border_color) {
}

RoundRectPainter::~RoundRectPainter() {
}

void RoundRectPainter::Paint(gfx::Canvas* canvas, const gfx::Size& size) {
  SkPaint paint;
  paint.setColor(border_color_);
  paint.setStyle(SkPaint::kStroke_Style);
  paint.setStrokeWidth(1);
  paint.setFlags(SkPaint::kAntiAlias_Flag);
  gfx::Rect rect(size);
  rect.Inset(0, 0, 1, 1);
  SkRect skia_rect = gfx::RectToSkRect(rect);
  skia_rect.offset(.5, .5);
  canvas->sk_canvas()->drawRoundRect(skia_rect, SkIntToScalar(kRadius),
      SkIntToScalar(kRadius), paint);
}

// static
void RoundRectPainter::CreateRoundRectPath(const gfx::Rect& bounds,
                                           gfx::Path* path) {
  SkRect rect = SkRect::MakeWH(SkIntToScalar(bounds.width()),
                               SkIntToScalar(bounds.height()));
  SkScalar radius = SkIntToScalar(kRadius);
  SkScalar radii[8] = {radius, radius, radius, radius,
                       radius, radius, radius, radius};
  path->addRoundRect(rect, radii);
}

}  // namespace views
