#ifndef MWGUI_LAYOUTS_H
#define MWGUI_LAYOUTS_H

#include <components/esm_store/store.hpp>

#include <boost/array.hpp>
#include <boost/lexical_cast.hpp>

#include <sstream>
#include <set>
#include <string>
#include <utility>

#include "../mwmechanics/stat.hpp"
#include "window_base.hpp"

/*
  This file contains classes corresponding to window layouts
  defined in resources/mygui/ *.xml.

  Each class inherites GUI::Layout and loads the XML file, and
  provides some helper functions to manipulate the elements of the
  window.

  The windows are never created or destroyed (except at startup and
  shutdown), they are only hid. You can control visibility with
  setVisible().
 */

namespace MWGui
{
  class HUD : public OEngine::GUI::Layout
  {
  public:
    HUD(int width, int height, bool fpsSwitch);
    void setStats(int h, int hmax, int m, int mmax, int s, int smax);
    void setWeapIcon(const char *str);
    void setSpellIcon(const char *str);
    void setWeapStatus(int s, int smax);
    void setSpellStatus(int s, int smax);
    void setEffect(const char *img);
    void setValue (const std::string& id, const MWMechanics::DynamicStat<int>& value);
    void setFPS(float fps);

    MyGUI::ProgressPtr health, magicka, stamina;
    MyGUI::StaticImagePtr weapImage, spellImage;
    MyGUI::ProgressPtr weapStatus, spellStatus;
    MyGUI::WidgetPtr effectBox;
    MyGUI::StaticImagePtr effect1;
    MyGUI::StaticImagePtr minimap;
    MyGUI::StaticImagePtr compass;
    MyGUI::StaticImagePtr crosshair;

    MyGUI::WidgetPtr fpsbox;
    MyGUI::StaticTextPtr fpscounter;
  };

  class MapWindow : public OEngine::GUI::Layout
  {
  public:
    MapWindow()
      : Layout("openmw_map_window_layout.xml")
    {
      setCoord(500,0,320,300);
      setText("WorldButton", "World");
      setImage("Compass", "compass.dds");

      // Obviously you should override this later on
      setCellName("No Cell Loaded");
    }

    void setCellName(const std::string& cellName)
    {
      mMainWidget->setCaption(cellName);
    }
  };

  class MainMenu : public OEngine::GUI::Layout
  {
  public:
    MainMenu(int w, int h)
      : Layout("openmw_mainmenu_layout.xml")
    {
      setCoord(0,0,w,h);
    }
  };
}
#endif
