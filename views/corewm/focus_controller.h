// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_COREWM_FOCUS_CONTROLLER_H_
#define UI_VIEWS_COREWM_FOCUS_CONTROLLER_H_

#include "base/compiler_specific.h"
#include "base/scoped_observer.h"
#include "ui/aura/client/activation_client.h"
#include "ui/aura/client/focus_client.h"
#include "ui/aura/env_observer.h"
#include "ui/aura/window_observer.h"
#include "ui/views/views_export.h"

namespace views {
namespace corewm {

class FocusRules;

// FocusController handles focus and activation changes for an environment
// encompassing one or more RootWindows. Within an environment there can be
// only one focused and one active window at a time. When focus or activation
// changes notifications are sent using the
// aura::client::Focus/ActivationChangeObserver interfaces.
// Changes to focus and activation can be from three sources:
// . the Aura Client API (implemented here in aura::client::ActivationClient).
//   (The FocusController must be set as the ActivationClient implementation
//    for all RootWindows).
// . input events (implemented here in ui::EventHandler).
//   (The FocusController must be registered as a pre-target handler for
//    the applicable environment owner, either a RootWindow or another type).
// . Window disposition changes (implemented here in aura::WindowObserver).
//   (The FocusController registers itself as an observer of the active and
//    focused windows).
class VIEWS_EXPORT FocusController : public aura::client::ActivationClient,
                                     public aura::client::FocusClient,
                                     public ui::EventHandler,
                                     public aura::WindowObserver,
                                     public aura::EnvObserver {
 public:
  // |rules| cannot be NULL.
  explicit FocusController(FocusRules* rules);
  virtual ~FocusController();

 private:
  // Overridden from aura::client::ActivationClient:
  virtual void AddObserver(
      aura::client::ActivationChangeObserver* observer) OVERRIDE;
  virtual void RemoveObserver(
      aura::client::ActivationChangeObserver* observer) OVERRIDE;
  virtual void ActivateWindow(aura::Window* window) OVERRIDE;
  virtual void DeactivateWindow(aura::Window* window) OVERRIDE;
  virtual aura::Window* GetActiveWindow() OVERRIDE;
  virtual aura::Window* GetActivatableWindow(aura::Window* window) OVERRIDE;
  virtual aura::Window* GetToplevelWindow(aura::Window* window) OVERRIDE;
  virtual bool OnWillFocusWindow(aura::Window* window,
                                 const ui::Event* event) OVERRIDE;
  virtual bool CanActivateWindow(aura::Window* window) const OVERRIDE;

  // Overridden from aura::client::FocusClient:
  virtual void AddObserver(
      aura::client::FocusChangeObserver* observer) OVERRIDE;
  virtual void RemoveObserver(
      aura::client::FocusChangeObserver* observer) OVERRIDE;
  virtual void FocusWindow(aura::Window* window,
                           const ui::Event* event) OVERRIDE;
  virtual aura::Window* GetFocusedWindow() OVERRIDE;
  virtual void OnWindowHiddenInRootWindow(aura::Window* window,
                                          aura::RootWindow* root_window,
                                          bool destroyed) OVERRIDE;

  // Overridden from ui::EventHandler:
  virtual void OnKeyEvent(ui::KeyEvent* event) OVERRIDE;
  virtual void OnMouseEvent(ui::MouseEvent* event) OVERRIDE;
  virtual void OnScrollEvent(ui::ScrollEvent* event) OVERRIDE;
  virtual void OnTouchEvent(ui::TouchEvent* event) OVERRIDE;
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;

  // Overridden from aura::WindowObserver:
  virtual void OnWindowVisibilityChanged(aura::Window* window,
                                         bool visible) OVERRIDE;
  virtual void OnWindowDestroying(aura::Window* window) OVERRIDE;
  virtual void OnWindowDestroyed(aura::Window* window) OVERRIDE;
  virtual void OnWindowRemovingFromRootWindow(aura::Window* window) OVERRIDE;

  // Overridden from aura::EnvObserver:
  virtual void OnWindowInitialized(aura::Window* window) OVERRIDE;

  // Internal implementations that set the focused/active windows, fire events
  // etc. These functions must be called with valid focusable/activatable
  // windows.
  void SetFocusedWindow(aura::Window* window);
  void SetActiveWindow(aura::Window* window);

  // Called when a window's disposition changed such that it and its hierarchy
  // are no longer focusable/activatable. The system must determine what window
  // to focus next based on rules.
  void WindowLostFocusFromDispositionChange(aura::Window* window);

  // Called when an attempt is made to focus or activate a window via an input
  // event targeted at that window. Rules determine the best focusable window
  // for the input window.
  void WindowFocusedFromInputEvent(aura::Window* window);

  aura::Window* active_window_;
  aura::Window* focused_window_;

  bool updating_focus_;

  scoped_ptr<FocusRules> rules_;

  ObserverList<aura::client::ActivationChangeObserver> activation_observers_;
  ObserverList<aura::client::FocusChangeObserver> focus_observers_;

  ScopedObserver<aura::Window, aura::WindowObserver> observer_manager_;

  DISALLOW_COPY_AND_ASSIGN(FocusController);
};

}  // namespace corewm
}  // namespace views

#endif  // UI_VIEWS_COREWM_FOCUS_CONTROLLER_H_
