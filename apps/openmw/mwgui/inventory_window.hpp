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

    InventoryWindow(MWWorld::ContainerStore<MWWorld::RefData>*);
    void setCategory(CategoryMode mode);
    MyGUI::ButtonPtr getCategoryButton(CategoryMode mode);
    void onCategorySelected(MyGUI::Widget *widget);
    //TODO: for scripts
    void addItem(MWWorld::Ptr& ptr);



    CategoryMode categoryMode;        // Current category filter
    MyGUI::ButtonPtr buttons[5];    // Button pointers
    std::string names[5];            // Names of category buttons

    MyGUI::Colour activeColor;
    MyGUI::Colour inactiveColor;
    void refreshView(int);

  private:
    void onScrollChangePosition(MyGUI::VScrollPtr, size_t);
    void onInventoryClick(MyGUI::WidgetPtr);
    void onAvatarClick(MyGUI::WidgetPtr);
    void onResize(MyGUI::Window* _sender);
    template <typename T>
    void drawItemWidget(ESMS::LiveCellRef<T, MWWorld::RefData>*);//MWWorld::Ptr);

    MWWorld::ContainerStore<MWWorld::RefData> *mContainer;

    std::map<MyGUI::WidgetPtr, MWWorld::Ptr> mItems;

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
  };
}
#endif
