// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_MESSAGE_CENTER_MESSAGE_BUBBLE_BASE_H_
#define UI_MESSAGE_CENTER_MESSAGE_BUBBLE_BASE_H_

#include "base/memory/scoped_ptr.h"
#include "message_center/message_center_export.h"
#include "message_center/notification_list.h"
#include "views/bubble/tray_bubble_view.h"

namespace message_center {

class MESSAGE_CENTER_EXPORT MessageBubbleBase {
 public:
  explicit MessageBubbleBase(NotificationList::Delegate* list_delegate);

  virtual ~MessageBubbleBase();

  // Gets called when when the bubble view associated with this bubble is
  // destroyed. Clears |bubble_view_| and calls OnBubbleViewDestroyed.
  void BubbleViewDestroyed();

  // Gets the init params for the implementation.
  virtual views::TrayBubbleView::InitParams GetInitParams(
      views::TrayBubbleView::AnchorAlignment anchor_alignment) = 0;

  // Called after the bubble view has been constructed. Creates and initializes
  // the bubble contents.
  virtual void InitializeContents(views::TrayBubbleView* bubble_view) = 0;

  // Called from BubbleViewDestroyed for implementation specific details.
  virtual void OnBubbleViewDestroyed() = 0;

  // Updates the bubble; implementation dependent.
  virtual void UpdateBubbleView() = 0;

  // Called when the mouse enters/exists the view.
  virtual void OnMouseEnteredView() = 0;
  virtual void OnMouseExitedView() = 0;

  // Schedules bubble for layout after all notifications have been
  // added and icons have had a chance to load.
  void ScheduleUpdate();

  bool IsVisible() const;

  views::TrayBubbleView* bubble_view() const { return bubble_view_; }

  static const SkColor kBackgroundColor;
  static const SkColor kHeaderBackgroundColorLight;
  static const SkColor kHeaderBackgroundColorDark;

 protected:
  views::TrayBubbleView::InitParams GetDefaultInitParams(
      views::TrayBubbleView::AnchorAlignment anchor_alignment);
  NotificationList::Delegate* list_delegate() { return list_delegate_; }
  void set_bubble_view(views::TrayBubbleView* bubble_view) {
    bubble_view_ = bubble_view;
  }

 private:
  NotificationList::Delegate* list_delegate_;
  views::TrayBubbleView* bubble_view_;
  base::WeakPtrFactory<MessageBubbleBase> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(MessageBubbleBase);
};

}  // namespace message_center

#endif // UI_MESSAGE_CENTER_MESSAGE_BUBBLE_BASE_H_
