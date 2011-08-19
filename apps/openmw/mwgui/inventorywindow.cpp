#include <components/esm_store/store.hpp>
#include "inventorywindow.hpp"
#include "../mwworld/class.hpp"
#include "../mwworld/environment.hpp"
#include "../mwworld/refdata.hpp"

namespace MWGui{
  InventoryWindow::InventoryWindow (MWWorld::ContainerStore<MWWorld::RefData> *container, ESMS::RecIDListT<ESM::GameSetting> settings,bool *Drag, std::pair<MWWorld::Ptr,int> *DragingItem)
    : Layout("openmw_inventory_window_layout.xml")
    , categoryMode(CM_All)

    // color should be fetched from skin
    , activeColor(0, 0, 1)
    , inactiveColor(0.7, 0.7, 0.7)
    , lastPos(0)
    , iIconSize(40)
    , iSpacingSize(8)
    , avatarAspect(2.217)
    , mDrag(Drag)
    , mDragingItem(DragingItem)
  {
    mContainer=container;

    setCoord(0, 400, 500, 300);
    // These are just demo values, you should replace these with
    // real calls from outside the class later.

    mMainWidget->setCaption("Glass Frostsword");
    MyGUI::Window* window = mMainWidget->castType<MyGUI::Window>(false);
    window->eventWindowChangeCoord=MyGUI::newDelegate(this,&InventoryWindow::onResize);
    window->setMinSize(200,64);

    setText("EncumbranceBarT", "176/210");

    MyGUI::ProgressPtr pt;
    getWidget(pt, "EncumbranceBar");
    pt->setProgressRange(210);
    pt->setProgressPosition(176);

    getWidget(scroll, "InventoryScroller");
    scroll->setVisible(true);
    scroll->setTrackSize(16);
    scroll->setScrollRange(1);
    scroll->setScrollPage(iIconSize+iSpacingSize);


    scroll->eventScrollChangePosition = MyGUI::newDelegate(this,&InventoryWindow::onScrollChangePosition);

    getWidget(items, "Items");

    getWidget(mAvatarWidget, "Avatar");

    mAvatarWidget->setSize(mAvatarWidget->getSize().height/avatarAspect,mAvatarWidget->getSize().height);
    mAvatarWidget->eventMouseButtonClick=MyGUI::newDelegate(this, &InventoryWindow::onAvatarClick);

    // Adjust armor rating text to bottom of avatar widget
    MyGUI::StaticTextPtr armor_rating;
    getWidget(armor_rating, "ArmorRating");
    armor_rating->setCaption("Armor: 11");
    MyGUI::IntCoord coord = armor_rating->getCoord();

    coord.top =mAvatarWidget->getCoord().height - 4 - coord.height;
    armor_rating->setCoord(coord);

    tabtext[0]="salltab";
    tabtext[1]="sweapontab";
    tabtext[2]="sappareltab";
    tabtext[3]="smaigctab";
    tabtext[4]="smisctab";
    
    names[0]="All";
    names[1]="Weapon";
    names[2]="Apparel";
    names[3]="Magic";
    names[4]="Misc";

    boost::array<CategoryMode, 5> categories = { {
      CM_All, CM_Weapon, CM_Apparel, CM_Magic, CM_Misc
    } };

    // Initialize buttons with text and adjust sizes, also mark All as active button
    int margin = 2;
    int last_x = 0;
    for (unsigned int i = 0; i < categories.size(); ++i)
    {
        CategoryMode mode = categories[i];
        std::string name = names[mode];
        name += "Button";
        setText(name, settings.list[tabtext[mode]].str);
        getWidget(buttons[mode], name);

        MyGUI::ButtonPtr &button_pt = buttons[mode];
        if (mode == CM_All)
            button_pt->setTextColour(activeColor);
        else
            button_pt->setTextColour(inactiveColor);
        MyGUI::IntCoord coord = button_pt->getCoord();
        coord.left = last_x;
        last_x += coord.width + margin;
        button_pt->setCoord(coord);

        button_pt->eventMouseButtonClick = MyGUI::newDelegate(this, &InventoryWindow::onCategorySelected);
    }


  }


  void InventoryWindow::setCategory(CategoryMode mode)
  {
    MyGUI::ButtonPtr pt = getCategoryButton(categoryMode);
    pt->setTextColour(inactiveColor);
    pt = getCategoryButton(mode);
    pt->setTextColour(activeColor);
    categoryMode = mode;
    refreshView(0);
  }

  MyGUI::ButtonPtr InventoryWindow::getCategoryButton(CategoryMode mode)
  {
      return buttons[mode];
  }
  void InventoryWindow::onCategorySelected(MyGUI::Widget *widget)
  {
      boost::array<CategoryMode, 5> categories = { {
      CM_All, CM_Weapon, CM_Apparel, CM_Magic, CM_Misc
      } };

      for (unsigned int i = 0; i < categories.size(); ++i)
      {
          CategoryMode mode = categories[i];
          if (widget == buttons[mode])
          {
              setCategory(mode);
              return;
          }
      }
  }

 //actually should be call in window_manager, which calls InventoryWindow::refreshView(0) while inventory is shown
 //but WindowManager::additem sounds, for me, counterintuitive, so inventory widgets recreating ll occuring while nothing is visible
  void InventoryWindow::addItem(MWWorld::Ptr &ptr)
  {
    refreshView(0);
  }


  void InventoryWindow::refreshView(int i)
  {

    switch(i){
      case 0: //category change/new item. mindless smashing all widgets and recreating needed
        for(std::map<MyGUI::WidgetPtr, MWWorld::Ptr >::iterator it= mItems.begin();it!= mItems.end();it++)
            MyGUI::Gui::getInstance().destroyWidget(it->first);

        mItems.clear();

        x=4-lastPos;
        y=4;
        if(categoryMode==CM_All){
            drawItemWidget(mContainer->weapons.list);
            drawItemWidget(mContainer->armors.list);
            drawItemWidget(mContainer->clothes.list);
            drawItemWidget(mContainer->potions.list);
            drawItemWidget(mContainer->ingreds.list);
            drawItemWidget(mContainer->books.list);
            drawItemWidget(mContainer->appas.list);
            drawItemWidget(mContainer->lockpicks.list);
            drawItemWidget(mContainer->miscItems.list);
            drawItemWidget(mContainer->probes.list);
            drawItemWidget(mContainer->repairs.list);
        }
        if(categoryMode==CM_Weapon ){
            drawItemWidget(mContainer->weapons.list);
        }
        if(categoryMode==CM_Magic){
            drawItemWidget(mContainer->weapons.list);
            drawItemWidget(mContainer->armors.list);
            drawItemWidget(mContainer->clothes.list);
            drawItemWidget(mContainer->potions.list);
            drawItemWidget(mContainer->books.list);
        }
        if(categoryMode==CM_Apparel){
            drawItemWidget(mContainer->armors.list);
            drawItemWidget(mContainer->clothes.list);
        }
        if(categoryMode==CM_Misc){
            drawItemWidget(mContainer->ingreds.list);
            drawItemWidget(mContainer->books.list);
            drawItemWidget(mContainer->appas.list);
            drawItemWidget(mContainer->lockpicks.list);
            drawItemWidget(mContainer->miscItems.list);
            drawItemWidget(mContainer->probes.list);
            drawItemWidget(mContainer->repairs.list);
        }
      break;
      case 1: // rearange items after resize

        x=4-lastPos;
        y=4;
        for(std::map<MyGUI::WidgetPtr, MWWorld::Ptr >::iterator it=mItems.begin(); it!=mItems.end(); it++){
            it->first->setPosition(x,y);
            if(y+2*iIconSize+iSpacingSize+16 > items->getClientCoord().bottom()){
                y=4;
                x+=iIconSize+iSpacingSize;
            }else{
                y+=iIconSize+iSpacingSize;
            }
            if(x+iIconSize+iSpacingSize>items->getClientCoord().right()){
                scroll->setScrollRange(x+iIconSize+iSpacingSize+lastPos-items->getClientCoord().right());
                scroll->setScrollViewPage(scroll->getScrollRange());
            }else{
                scroll->setScrollPosition(0);
                scroll->setScrollRange(1);
                lastPos=0;
            }
        }
      break;
    }
  }

  void InventoryWindow::onScrollChangePosition(MyGUI::VScrollPtr _sender, size_t pos)
  {
    int diff = lastPos - pos;
    // Adjust position of all widgets according to difference
    if (diff == 0)
        return;
    lastPos = pos;

    for (std::map<MyGUI::WidgetPtr,MWWorld::Ptr>::const_iterator it = mItems.begin(); it != mItems.end(); ++it)
    {
        it->first->setCoord(it->first->getCoord() + MyGUI::IntPoint(diff, 0));
    }
  }

  void InventoryWindow::onInventoryClick(MyGUI::WidgetPtr _sender)
  {
    int count=1,oldcount=mItems[_sender].getRefData().getCount();
//    printf("%i\n",oldcount);
    if(!*mDrag){ //drag
        if(oldcount>1){
        //set separate mode, nothing can be done while in it (except ESC menu):

        //here number to split should be returned to count variable
        }
        *mDragingItem=std::make_pair(mItems[_sender],count);
        if(oldcount-count>1){
            _sender->getChildAt(0)->setCaption(MyGUI::utility::toString(oldcount-count));
        }else if(oldcount-count==1){
            MyGUI::Gui::getInstance().destroyWidget(_sender->getChildAt(0));
        }else{
            mItems.erase(_sender);
            MyGUI::Gui::getInstance().destroyWidget(_sender);
        }

        // actually here comes a new pile, in which .getRefData().getCount() should
        // be ignored, and, if dropped on the ground/placed in another container,
        // updated with count, or something, not sure yet
        // and count left in inventory should be updated accordingly (also in case of consumption by avatar)
        *mDrag=true;
//        refreshView(0);
    }else{ //drop the thing to inventory

       *mDrag=false;
    }
  }

  void InventoryWindow::onAvatarClick(MyGUI::Widget* _sender)
  {
/*    if(mDrag){

      //TODO: get type of Ptr somehow, and do gui accordingly
        if(mDragingItem type == "apparatus"){
            //show alchemy gui etc.
        }else if(mDragingItem type == "book"){

        }else if(mDragingItem type == "consumable"){

        }else if(mDragingItem type == "enchantment"){

        }else if(mDragingItem type == "repairment"){

        }else{ //armor/clothing and misc is rest

            if(true){ //type wearable not misc
                //TODO:check for already equipped item, redecorate it, and add newly equipped to invetory gui
                mEquipped[BP_SHIRT]=mDragingItem;
            }
        }
        mDrag=false;
    }else{  //items can be stripped from avatar

    }
*/
  }

  void InventoryWindow::onResize(MyGUI::Window*_sender)
  {
    mAvatarWidget->setSize(
      (mMainWidget->getCoord().height-mAvatarWidget->getCoord().top-42)/avatarAspect,
      mMainWidget->getCoord().height-mAvatarWidget->getCoord().top-42);

    items->setPosition(mAvatarWidget->getCoord().right()+4, items->getCoord().top);
    items->setCoord(mAvatarWidget->getCoord().right()+4, items->getCoord().top,mMainWidget->getClientCoord().width-mAvatarWidget->getCoord().right()-8,items->getCoord().height);
    refreshView(1);
  }

  template <typename T>
  void InventoryWindow::drawItemWidget(std::list<ESMS::LiveCellRef<T, MWWorld::RefData> > itemlist)
  {
    MyGUI::StaticImagePtr Item;
    MyGUI::StaticTextPtr countWidget;
    std::string icon; // storage for icon name manipualtion
    size_t found;
    for(typename std::list<ESMS::LiveCellRef<T, MWWorld::RefData> >::iterator it = itemlist.begin(); it != itemlist.end(); it++){
        if(categoryMode==CM_Magic){
            if(typeid(&*it)==typeid(ESMS::LiveCellRef<ESM::Weapon, MWWorld::RefData>*))
                if(((ESMS::LiveCellRef<ESM::Weapon, MWWorld::RefData>*)&*it)->base->enchant == "")
                    break;
            if(typeid(&*it)==typeid(ESMS::LiveCellRef<ESM::Armor, MWWorld::RefData>*))
                if(((ESMS::LiveCellRef<ESM::Armor, MWWorld::RefData>*)&*it)->base->enchant == "")
                    break;
            if(typeid(&*it)==typeid(ESMS::LiveCellRef<ESM::Clothing, MWWorld::RefData>*))
                if(((ESMS::LiveCellRef<ESM::Clothing, MWWorld::RefData>*)&*it)->base->enchant == "")
                    break;
            if(typeid(&*it)==typeid(ESMS::LiveCellRef<ESM::Book, MWWorld::RefData>*))
                if(((ESMS::LiveCellRef<ESM::Book, MWWorld::RefData>*)&*it)->base->enchant == "")
                    break;
        }
        MWWorld::Ptr ptr=MWWorld::Ptr(&*it, 0);
        Item=items->createWidget<MyGUI::StaticImage>("StaticImage", x, y, iIconSize, iIconSize, MyGUI::Align::Default );
        icon=MWWorld::Class::get (ptr).getInventoryIcon (ptr);
        found=icon.rfind(".tga");
        if (found!=std::string::npos)
            icon.replace (found,strlen(".tga"),".dds");
        else
            std::cout<<"non .tga icon returned for ptr\n(crash will possibly follow)\n";
        Item->setImageTexture("icons\\"+icon);
        //TODO: colour, align, stretching etc
        if(ptr.getRefData().getCount()>1){
            countWidget=Item->createWidget<MyGUI::StaticText>("StaticText",25,23,20,20,MyGUI::Align::Default); //test values
            countWidget->setCaption(MyGUI::utility::toString(ptr.getRefData().getCount()));
        }
        Item->eventMouseButtonClick=MyGUI::newDelegate(this,&InventoryWindow::onInventoryClick);
        mItems.insert(std::make_pair(Item, ptr));
        if(y+2*iIconSize+iSpacingSize+16 > items->getClientCoord().bottom()){
            y=4;
            x+=iIconSize+iSpacingSize;

        }else{
            y+=iIconSize+iSpacingSize;
            if(x+iIconSize+iSpacingSize>items->getClientCoord().right()){
                scroll->setScrollRange(x+iIconSize+iSpacingSize+lastPos-items->getClientCoord().right());
                scroll->setScrollViewPage(scroll->getScrollRange());
            }
        }
    }
  }
} //namespace
