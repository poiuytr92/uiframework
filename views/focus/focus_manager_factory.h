// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_FOCUS_FOCUS_MANAGER_FACTORY_H_
#define UI_VIEWS_FOCUS_FOCUS_MANAGER_FACTORY_H_

#include "base/basictypes.h"
#include "views/views_export.h"

namespace views {

class FocusManager;
class Widget;

// A factory to create FocusManager. This is used in unit tests
// to inject a custom factory.
class VIEWS_EXPORT FocusManagerFactory {
 public:
  // Create a FocusManager for the given |widget| using installe Factory.
  static FocusManager* Create(Widget* widget);

  // Installs FocusManagerFactory. If |factory| is NULL, it resets
  // to the default factory which creates plain FocusManager.
  static void Install(FocusManagerFactory* factory);

 protected:
  FocusManagerFactory();
  virtual ~FocusManagerFactory();

  // Create a FocusManager for the given |widget|.
  virtual FocusManager* CreateFocusManager(Widget* widget) = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(FocusManagerFactory);
};

}  // namespace views

#endif  // UI_VIEWS_FOCUS_FOCUS_MANAGER_FACTORY_H_
