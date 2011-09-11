#ifndef MWGUI_INVENTORYWINDOW_H
#define MWGUI_INVENTORYWINDOW_H

#include <openengine/gui/layout.hpp>

#include <boost/array.hpp>

#include "../mwworld/ptr.hpp"
#include "../mwworld/class.hpp"
#include "../mwworld/containerutil.hpp"
#include "../mwsound/soundmanager.hpp"
/* \brief inventory window in inventory, trade and container modes
*/

namespace MWGui
{
  class InventoryWindow : public OEngine::GUI::Layout
  {
  public:
    enum CategoryMode
      {
        CM_All = 0,      // All items
        CM_Weapon = 1,   // Only weapons
        CM_Apparel = 2,  // Apparel
        CM_Magic = 3,    // Magic
        CM_Misc = 4      // Misc
      };
    InventoryWindow(MWWorld::Environment&, MWWorld::itemTransmission*);

    void setCategory(CategoryMode mode);
    MyGUI::ButtonPtr getCategoryButton(CategoryMode mode);
    void onCategorySelected(MyGUI::Widget *widget);
    void addItem(MWWorld::Ptr& ptr);

    CategoryMode categoryMode;        // Current category filter
    MyGUI::ButtonPtr buttons[5];    // Button pointers
    std::string names[5];            // Names of category buttons
    std::string tabtext[5];

    MyGUI::Colour activeColor;
    MyGUI::Colour inactiveColor;
    void refreshView(int);

  private:
    void onScrollChangePosition(MyGUI::VScrollPtr, size_t);
    void onInventoryClick(MyGUI::WidgetPtr);
    void onAvatarClick(MyGUI::WidgetPtr);
    void onResize(MyGUI::Window* _sender);

    template <typename T>
    void drawItemWidget(std::list<ESMS::LiveCellRef<T, MWWorld::RefData> >*);

    MWWorld::ContainerStore<MWWorld::RefData> *mContainer;

    typedef std::map<MyGUI::StaticImagePtr, MWWorld::Ptr> mapItems;

    mapItems mItems;

    int x;
    int y;

    /// scrollbar position
    int lastPos;

    MyGUI::WidgetPtr mAvatarWidget;
    MyGUI::WidgetPtr items;
    MyGUI::HScrollPtr scroll;
    const int iIconSize;
    const int iSpacingSize;
    float avatarAspect;

    /// these members for moving items between windows (trade, container and this)
    MWWorld::itemTransmission *mDragingItem; //item,count pair

    MWSound::SoundManager *mSoundManager;
  };
}
#endif
