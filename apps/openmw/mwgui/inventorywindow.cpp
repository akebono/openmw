#include "inventorywindow.hpp"

#include <components/esm_store/store.hpp>

#include "../mwworld/class.hpp"
#include "../mwworld/refdata.hpp"
#include "../mwworld/environment.hpp"
#include "../mwworld/world.hpp"


namespace MWGui{
  /// constructor with many parameters for transmission of storage for items that will move between trade/container and this window
  // \todo set width of bar for tabs' buttons with some MyGUI means, not as constant
  InventoryWindow::InventoryWindow (MWWorld::Environment& environment, MWWorld::itemTransmission *DragingItem)
    : Layout("openmw_inventory_window_layout.xml")
    , categoryMode(CM_All)

    // color should be fetched from skin
    , activeColor(0, 0, 1)
    , inactiveColor(0.7, 0.7, 0.7)
    , lastPos(0)
    , iIconSize(40)
    , iSpacingSize(8)
    , avatarAspect(2.217)
    , mDragingItem(DragingItem)
  {
    MWWorld::Ptr player = environment.mWorld->getPtr ("player", true);
    mContainer=&MWWorld::Class::get (player).getContainerStore (player);
    mSoundManager=environment.mSoundManager;
//    PickDropSound pds=PickDropSound(environment);
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
    items->eventMouseButtonClick=MyGUI::newDelegate(this,&InventoryWindow::onInventoryClick);

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
    tabtext[3]="smagictab";
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
    MyGUI::WidgetPtr catbar;
    for (unsigned int i = 0; i < categories.size(); ++i)
    {
        CategoryMode mode = categories[i];
        std::string name = names[mode];
        name += "Button";
        getWidget(buttons[mode], name);
        setText(name, environment.mWorld->getStore().gameSettings.find(tabtext[mode])->str);
        buttons[mode]->setSize(buttons[mode]->getTextSize().width+15,buttons[mode]->getClientCoord().height); //width adjustment for localized strings
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
    getWidget(catbar,"Categories");
    catbar->setSize(1024,catbar->getClientCoord().height); //dirtiest hack :(
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

  void InventoryWindow::addItem(MWWorld::Ptr &ptr)
  {
    refreshView(0);
  }

  // called when inventory showed, category switches (i is 0) it causes recreating of all widgets for items in player's inventory
  // also called upon resizing of inventory window (i = 1) which causes rearraging widgets accordingly

  void InventoryWindow::refreshView(int i)
  {
    switch(i){
      case 0: //category change/new item. mindless smashing all widgets and recreating needed
        for(mapItems::iterator it= mItems.begin();it!= mItems.end();it++)
            MyGUI::Gui::getInstance().destroyWidget(it->first);

        mItems.clear();
        x=4-lastPos;
        y=4;

        if(categoryMode==CM_All){
            drawItemWidget(&mContainer->weapons.list);
            drawItemWidget(&mContainer->armors.list);
            drawItemWidget(&mContainer->clothes.list);
            drawItemWidget(&mContainer->potions.list);
            drawItemWidget(&mContainer->ingreds.list);
            drawItemWidget(&mContainer->books.list);
            drawItemWidget(&mContainer->appas.list);
            drawItemWidget(&mContainer->lockpicks.list);
            drawItemWidget(&mContainer->miscItems.list);
            drawItemWidget(&mContainer->probes.list);
            drawItemWidget(&mContainer->repairs.list);
        }
        if(categoryMode==CM_Weapon ){
            drawItemWidget(&mContainer->weapons.list);
        }
        if(categoryMode==CM_Magic){
            drawItemWidget(&mContainer->weapons.list);
            drawItemWidget(&mContainer->armors.list);
            drawItemWidget(&mContainer->clothes.list);
            drawItemWidget(&mContainer->potions.list);
            drawItemWidget(&mContainer->books.list);
        }
        if(categoryMode==CM_Apparel){
            drawItemWidget(&mContainer->armors.list);
            drawItemWidget(&mContainer->clothes.list);
        }
        if(categoryMode==CM_Misc){
            drawItemWidget(&mContainer->ingreds.list);
            drawItemWidget(&mContainer->books.list);
            drawItemWidget(&mContainer->appas.list);
            drawItemWidget(&mContainer->lockpicks.list);
            drawItemWidget(&mContainer->miscItems.list);
            drawItemWidget(&mContainer->probes.list);
            drawItemWidget(&mContainer->repairs.list);
        }
      break;
      case 1: // rearange items after resize

        x=4-lastPos;
        y=4;
        for(mapItems::iterator it=mItems.begin(); it!=mItems.end(); it++){
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

    for (mapItems::iterator it = mItems.begin(); it != mItems.end(); ++it)
    {
        it->first->setCoord(it->first->getCoord() + MyGUI::IntPoint(diff, 0));
    }
  }

  void InventoryWindow::onInventoryClick(MyGUI::WidgetPtr _sender)
  {
    int count=1;
    int oldcount=1;
    mapItems::iterator it=mItems.find((MyGUI::StaticImagePtr)_sender);
    
    if(it!=mItems.end() ){
        oldcount=it->second.getRefData().getCount();
    }else if(!mDragingItem->drag){
//        printf("onInventoryClick error: not found\n");
        return;
    }

    if(!mDragingItem->drag){ //drag
        mSoundManager->playSound("item weapon blunt up",1.0,1.0);
        if(oldcount>1){

        //set separate mode, nothing can be done while in it (except ESC menu):

        //here number to split should be returned to count variable
        }
        // actually here comes a new pile,
        // and count left in inventory should be updated accordingly (consumption by avatar etc)
        mDragingItem->item=mItems[(MyGUI::StaticImagePtr)_sender];
        mDragingItem->count=count;

        if(oldcount-count>1){
            _sender->getChildAt(0)->setCaption(MyGUI::utility::toString(oldcount-count));
        }else if(oldcount-count==1){
            MyGUI::Gui::getInstance().destroyWidget(_sender->getChildAt(0));
        }else{
            MyGUI::Gui::getInstance().destroyWidget(_sender);
            mItems.erase((MyGUI::StaticImagePtr)_sender);
        }
        mDragingItem->drag=true;
        
        refreshView(1);
    }else{ //drop the thing to inventory
//            MWWorld::Class::get (mDragingItem->item).removeFromContainer (mDragingItem->item, mDragingItem->mContainer);
        mSoundManager->playSound("item weapon blunt down",1.0,1.0);
//    printf("*mDragingItem %s\n",mDragingItem->first.mTypeName.c_str());
//        MWWorld::Class::get (mDragingItem->first).insertIntoContainer (mDragingItem->item, mDragingItem->mContainer);
        mDragingItem->drag=false;
        refreshView(0);
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
  void InventoryWindow::drawItemWidget(std::list<ESMS::LiveCellRef<T, MWWorld::RefData> > *itemlist)
  {

    MyGUI::StaticImagePtr Item;
    MyGUI::StaticTextPtr countWidget;

    std::string icon; // storage for icon name manipualtion
    std::string::size_type found;

    for(typename std::list<ESMS::LiveCellRef<T, MWWorld::RefData> >::iterator it = itemlist->begin(); it != itemlist->end(); it++){
        if(categoryMode==CM_Magic){
            if(typeid(T)==typeid(ESM::Weapon))
                if(((ESMS::LiveCellRef<ESM::Weapon, MWWorld::RefData>*)&*it)->base->enchant.empty())
                    break;
            if(typeid(T)==typeid(ESM::Armor))
                if(((ESMS::LiveCellRef<ESM::Armor, MWWorld::RefData>*)&*it)->base->enchant.empty())
                    break;
            if(typeid(T)==typeid(ESM::Clothing))
                if(((ESMS::LiveCellRef<ESM::Clothing, MWWorld::RefData>*)&*it)->base->enchant.empty())
                    break;
            if(typeid(T)==typeid(ESM::Book))
                if(((ESMS::LiveCellRef<ESM::Book, MWWorld::RefData>*)&*it)->base->enchant.empty())
                    break;
        }

        MWWorld::Ptr ptr=MWWorld::Ptr(&*it, 0);
        Item=items->createWidget<MyGUI::StaticImage>("StaticImage", x, y, iIconSize, iIconSize, MyGUI::Align::Default );
        icon=MWWorld::Class::get (ptr).getInventoryIcon (ptr);
        found=icon.rfind(".tga");

        if (found!=std::string::npos)
            icon.replace (found,4,".dds");
        else
            std::cout<<"non .tga icon returned for ptr\n(crash will possibly follow)\n";

        Item->setImageTexture("icons\\"+icon);

        //TODO: colour, aligned sizing etc
        if(ptr.getRefData().getCount()>1){
            countWidget=Item->createWidget<MyGUI::StaticText>("StaticText",25,23,20,20,MyGUI::Align::Default); //test values
            countWidget->setCaption(MyGUI::utility::toString(ptr.getRefData().getCount()));
        }

        Item->eventMouseButtonClick=MyGUI::newDelegate(this,&InventoryWindow::onInventoryClick);
        std::pair<mapItems::iterator,bool> ret;
        ret=mItems.insert(std::make_pair(Item, ptr));

        // proper positioning
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
