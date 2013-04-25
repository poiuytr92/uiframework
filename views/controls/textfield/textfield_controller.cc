// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "views/controls/textfield/textfield_controller.h"

#include "uibase/dragdrop/drag_drop_types.h"

namespace views {

int TextfieldController::OnDrop(const ui::OSExchangeData& data) {
  return ui::DragDropTypes::DRAG_NONE;
}

bool TextfieldController::IsCommandIdEnabled(int command_id) const {
  return false;
}

bool TextfieldController::IsItemForCommandIdDynamic(int command_id) const {
  return false;
}

string16 TextfieldController::GetLabelForCommandId(int command_id) const {
  return string16();
}

bool TextfieldController::HandlesCommand(int command_id) const {
  return false;
}

}  // namespace views
