#pragma once
/*
 *      Copyright (C) 2005-2013 Team XBMC
 *      http://www.xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#include "libXBMC_addon.h"

typedef void* GUIHANDLE;

/* current ADDONGUI API version */
#define XBMC_GUI_API_VERSION "1.0.0"

/* min. ADDONGUI API version */
#define XBMC_GUI_MIN_API_VERSION "1.0.0"

#define ADDON_ACTION_PREVIOUS_MENU          10
#define ADDON_ACTION_CLOSE_DIALOG           51

class CAddonGUIWindow;
class CAddonGUISpinControl;
class CAddonGUIRadioButton;
class CAddonGUIProgressControl;
class CAddonListItem;
class CAddonGUIRenderingControl;

class CHelper_libXBMC_gui
{
public:
  CHelper_libXBMC_gui()
  {
  }

  ~CHelper_libXBMC_gui()
  {
  }

  bool RegisterMe(void *Handle)
  {
    (void)Handle;
    return true;
  }

  MOCK_METHOD0(Lock, void());
  MOCK_METHOD0(Unlock, void());
  MOCK_METHOD0(GetScreenHeight, int());
  MOCK_METHOD0(GetScreenWidth, int());
  MOCK_METHOD0(GetVideoResolution, int());

  MOCK_METHOD4(Window_create, CAddonGUIWindow*(const char *xmlFilename, const char *defaultSkin, bool forceFallback, bool asDialog));
  MOCK_METHOD1(Window_destroy, void(CAddonGUIWindow* p));
  MOCK_METHOD2(Control_getSpin, CAddonGUISpinControl*(CAddonGUIWindow *window, int controlId));
  MOCK_METHOD1(Control_releaseSpin, void(CAddonGUISpinControl* p));
  MOCK_METHOD2(Control_getRadioButton, CAddonGUIRadioButton*(CAddonGUIWindow *window, int controlId));
  MOCK_METHOD1(Control_releaseRadioButton, void(CAddonGUIRadioButton* p));
  MOCK_METHOD2(Control_getProgress, CAddonGUIProgressControl*(CAddonGUIWindow *window, int controlId));
  MOCK_METHOD1(Control_releaseProgress, void(CAddonGUIProgressControl* p));
  MOCK_METHOD5(ListItem_create, CAddonListItem*(const char *label, const char *label2, const char *iconImage, const char *thumbnailImage, const char *path));
  MOCK_METHOD1(ListItem_destroy, void(CAddonListItem* p));
  MOCK_METHOD2(Control_getRendering, CAddonGUIRenderingControl*(CAddonGUIWindow *window, int controlId));
  MOCK_METHOD1(Control_releaseRendering, void(CAddonGUIRenderingControl* p));
};

class CAddonGUISpinControl
{
public:
  CAddonGUISpinControl(void *hdl, void *cb, CAddonGUIWindow *window, int controlId);
  virtual ~CAddonGUISpinControl(void) {}

  virtual void SetVisible(bool yesNo);
  virtual void SetText(const char *label);
  virtual void Clear();
  virtual void AddLabel(const char *label, int iValue);
  virtual int GetValue();
  virtual void SetValue(int iValue);

private:
  CAddonGUIWindow *m_Window;
  int         m_ControlId;
  GUIHANDLE   m_SpinHandle;
  void *m_Handle;
  void *m_cb;
};

class CAddonGUIRadioButton
{
public:
  CAddonGUIRadioButton(void *hdl, void *cb, CAddonGUIWindow *window, int controlId);
  virtual ~CAddonGUIRadioButton() {}

  virtual void SetVisible(bool yesNo);
  virtual void SetText(const char *label);
  virtual void SetSelected(bool yesNo);
  virtual bool IsSelected();

private:
  CAddonGUIWindow *m_Window;
  int         m_ControlId;
  GUIHANDLE   m_ButtonHandle;
  void *m_Handle;
  void *m_cb;
};

class CAddonGUIProgressControl
{
public:
  CAddonGUIProgressControl(void *hdl, void *cb, CAddonGUIWindow *window, int controlId);
  virtual ~CAddonGUIProgressControl(void) {}

  virtual void SetPercentage(float fPercent);
  virtual float GetPercentage() const;
  virtual void SetInfo(int iInfo);
  virtual int GetInfo() const;
  virtual std::string GetDescription() const;

private:
  CAddonGUIWindow *m_Window;
  int         m_ControlId;
  GUIHANDLE   m_ProgressHandle;
  void *m_Handle;
  void *m_cb;
};

class CAddonListItem
{
friend class CAddonGUIWindow;

public:
  CAddonListItem(void *hdl, void *cb, const char *label, const char *label2, const char *iconImage, const char *thumbnailImage, const char *path);
  virtual ~CAddonListItem(void) {}

  virtual const char  *GetLabel();
  virtual void         SetLabel(const char *label);
  virtual const char  *GetLabel2();
  virtual void         SetLabel2(const char *label);
  virtual void         SetIconImage(const char *image);
  virtual void         SetThumbnailImage(const char *image);
  virtual void         SetInfo(const char *Info);
  virtual void         SetProperty(const char *key, const char *value);
  virtual const char  *GetProperty(const char *key) const;
  virtual void         SetPath(const char *Path);

//    {(char*)"select();
//    {(char*)"isSelected();
protected:
  GUIHANDLE   m_ListItemHandle;
  void *m_Handle;
  void *m_cb;
};

class CAddonGUIWindow
{
friend class CAddonGUISpinControl;
friend class CAddonGUIRadioButton;
friend class CAddonGUIProgressControl;
friend class CAddonGUIRenderingControl;

public:
  CAddonGUIWindow(void *hdl, void *cb, const char *xmlFilename, const char *defaultSkin, bool forceFallback, bool asDialog);
  virtual ~CAddonGUIWindow();

  virtual bool         Show();
  virtual void         Close();
  virtual void         DoModal();
  virtual bool         SetFocusId(int iControlId);
  virtual int          GetFocusId();
  virtual bool         SetCoordinateResolution(int res);
  virtual void         SetProperty(const char *key, const char *value);
  virtual void         SetPropertyInt(const char *key, int value);
  virtual void         SetPropertyBool(const char *key, bool value);
  virtual void         SetPropertyDouble(const char *key, double value);
  virtual const char  *GetProperty(const char *key) const;
  virtual int          GetPropertyInt(const char *key) const;
  virtual bool         GetPropertyBool(const char *key) const;
  virtual double       GetPropertyDouble(const char *key) const;
  virtual void         ClearProperties();
  virtual int          GetListSize();
  virtual void         ClearList();
  virtual GUIHANDLE    AddStringItem(const char *name, int itemPosition = -1);
  virtual void         AddItem(GUIHANDLE item, int itemPosition = -1);
  virtual void         AddItem(CAddonListItem *item, int itemPosition = -1);
  virtual void         RemoveItem(int itemPosition);
  virtual GUIHANDLE    GetListItem(int listPos);
  virtual void         SetCurrentListPosition(int listPos);
  virtual int          GetCurrentListPosition();
  virtual void         SetControlLabel(int controlId, const char *label);
  virtual void         MarkDirtyRegion();

  virtual bool         OnClick(int controlId);
  virtual bool         OnFocus(int controlId);
  virtual bool         OnInit();
  virtual bool         OnAction(int actionId);

  GUIHANDLE m_cbhdl;
  bool (*CBOnInit)(GUIHANDLE cbhdl);
  bool (*CBOnFocus)(GUIHANDLE cbhdl, int controlId);
  bool (*CBOnClick)(GUIHANDLE cbhdl, int controlId);
  bool (*CBOnAction)(GUIHANDLE cbhdl, int actionId);

protected:
  GUIHANDLE m_WindowHandle;
  void *m_Handle;
  void *m_cb;
};

class CAddonGUIRenderingControl
{
public:
  CAddonGUIRenderingControl(void *hdl, void *cb, CAddonGUIWindow *window, int controlId);
  virtual ~CAddonGUIRenderingControl();
  virtual void Init();

  virtual bool Create(int x, int y, int w, int h, void *device);
  virtual void Render();
  virtual void Stop();
  virtual bool Dirty();

  GUIHANDLE m_cbhdl;
  bool (*CBCreate)(GUIHANDLE cbhdl, int x, int y, int w, int h, void *device);
  void (*CBRender)(GUIHANDLE cbhdl);
  void (*CBStop)(GUIHANDLE cbhdl);
  bool (*CBDirty)(GUIHANDLE cbhdl);

private:
  CAddonGUIWindow *m_Window;
  int         m_ControlId;
  GUIHANDLE   m_RenderingHandle;
  void *m_Handle;
  void *m_cb;
};
