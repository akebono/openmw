#ifndef MWGUI_INVENTORY_H
#define MWGUI_INVENTORY_H

#include <openengine/gui/layout.hpp>

#include <boost/array.hpp>

#include "../mwworld/ptr.hpp"
#include "../mwworld/class.hpp"

/* \brief invetory window in inventory mode
*/

#define ICON_SIZE 40
#define ICON_SPACING 8
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

    InventoryWindow ();
    void setCategory(CategoryMode mode);
    MyGUI::ButtonPtr getCategoryButton(CategoryMode mode);
    void onCategorySelected(MyGUI::Widget *widget);
    //FIXME: somehow should be called from somewhere
    void addItem(MWWorld::Ptr& ptr);


    CategoryMode categoryMode;        // Current category filter
    MyGUI::ButtonPtr buttons[5];    // Button pointers
    std::string names[5];            // Names of category buttons

    MyGUI::Colour activeColor;
    MyGUI::Colour inactiveColor;

  private:
    void onScrollChangePosition(MyGUI::VScrollPtr scroller, size_t pos);
    void onPick(MyGUI::WidgetPtr _sender);
    void onEquip(MyGUI::Widget* _sender);
    void refreshView(int);

    std::vector<MWWorld::Ptr> mItems;
    MyGUI::WidgetPtr inventory;
    MyGUI::WidgetPtr items;
    MyGUI::HScrollPtr scroll;

    std::map<MWWorld::Ptr, MyGUI::StaticImagePtr> mWItems;

    bool mDrag;
    MWWorld::Ptr mDragingItem;
    std::map<int,MWWorld::Ptr> mEquipped; //slot, item
    int x;
    int y;
    int lastPos;

    MyGUI::WidgetPtr mAvatarWidget;

    //FIXME:TEST
  public:
    void addItem(std::string);
  private:

    std::vector<std::string> mItemsTEST;
    std::string mDragingItemTest;
    std::multimap<std::string, MyGUI::StaticImagePtr> mWItemsTEST;
    std::map<int,std::string> mEquippedTEST;
  };
}
#endif
