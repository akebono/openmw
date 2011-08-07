#ifndef MWGUI_INVENTORY_H
#define MWGUI_INVENTORY_H

#include <openengine/gui/layout.hpp>

#include <boost/array.hpp>

#include "../mwworld/ptr.hpp"
#include "../mwworld/class.hpp"

/* \brief invetory window in inventory mode
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
    enum BodyPart
      {
        BP_HEAD = 0,
        BP_SHOULDERS = 1,
        BP_WRIST_LEFT = 2,
        BP_WRIST_RIGHT = 3,
        BP_CUIRASS = 4,
        BP_GREAVES = 5,
        BP_BOOTS = 6,
        //clothing
        BP_SHIRT = 7,
        BP_PANTS = 8,
        BP_AMULET = 9,
        BP_BELT = 10,
        BP_RING_LEFT = 11,
        BP_RING_RIGHT = 12,
      };
    InventoryWindow();
    InventoryWindow(MWWorld::ContainerStore<MWWorld::RefData>*);
    void setCategory(CategoryMode mode);
    MyGUI::ButtonPtr getCategoryButton(CategoryMode mode);
    void onCategorySelected(MyGUI::Widget *widget);
    //FIXME: for scripts
    void addItem(MWWorld::Ptr& ptr);



    CategoryMode categoryMode;        // Current category filter
    MyGUI::ButtonPtr buttons[5];    // Button pointers
    std::string names[5];            // Names of category buttons

    MyGUI::Colour activeColor;
    MyGUI::Colour inactiveColor;
//    void refreshView(int);
    void refreshView(int,MWWorld::ContainerStore<MWWorld::RefData>);
  private:
    void onScrollChangePosition(MyGUI::VScrollPtr, size_t);
    void onInventoryClick(MyGUI::WidgetPtr);
    void onAvatarClick(MyGUI::WidgetPtr);
    void onResize(MyGUI::Window* _sender);

    MWWorld::ContainerStore<MWWorld::RefData> mContainer;

    //soon will be done, somehow
    std::map<MyGUI::WidgetPtr, ESMS::LiveCellRef<ESM::Weapon, MWWorld::RefData> > mWeapons;
    std::map<MyGUI::WidgetPtr, ESMS::LiveCellRef<ESM::Armor, MWWorld::RefData> > mArmors;
    std::map<MyGUI::WidgetPtr, ESMS::LiveCellRef<ESM::Miscellaneous, MWWorld::RefData> > mMisc;
    std::map<MyGUI::WidgetPtr, ESMS::LiveCellRef<ESM::Potion, MWWorld::RefData> > mPotions;
    std::map<MyGUI::WidgetPtr, ESMS::LiveCellRef<ESM::Apparatus, MWWorld::RefData> > mApparatus;
    std::map<MyGUI::WidgetPtr, ESMS::LiveCellRef<ESM::Ingredient, MWWorld::RefData> > mIngredients;
    std::map<MyGUI::WidgetPtr, ESMS::LiveCellRef<ESM::Clothing, MWWorld::RefData> > mClothings;
    std::map<MyGUI::WidgetPtr, ESMS::LiveCellRef<ESM::Book, MWWorld::RefData> > mBooks;

    bool mDrag;
    MWWorld::Ptr mDragingItem;

    int x;
    int y;
    int lastPos;

    MyGUI::WidgetPtr mAvatarWidget;
    MyGUI::WidgetPtr items;
    MyGUI::HScrollPtr scroll;
    const int iIconSize;
    const int iSpacingSize;
    float avatarAspect;
    //FIXME:TEST
  public:
    void addItem(std::string);
    void test(MWWorld::Ptr);
  private:
    std::vector<MWWorld::Ptr> testvalue;
    std::vector<std::string> mItemsTEST;
    std::string mDragingItemTest;
    std::multimap<MyGUI::WidgetPtr, std::string> mWItemsTEST;
    std::map<int,std::string> mEquippedTEST;
  };
}
#endif
