
#include "inventory_window.hpp"

namespace MWGui{
  InventoryWindow::InventoryWindow ()
    : Layout("openmw_inventory_window_layout.xml")
    , categoryMode(CM_All)

    // color should be fetched from skin
    , activeColor(0, 0, 1)
    , inactiveColor(0.7, 0.7, 0.7)
    , mDrag(false)
  {
    setCoord(0, 400, 500, 300);

    // These are just demo values, you should replace these with
    // real calls from outside the class later.

    mMainWidget->setCaption("Glass Frostsword");
    setText("EncumbranceBarT", "176/210");

    MyGUI::ProgressPtr pt;
    getWidget(pt, "EncumbranceBar");
    pt->setProgressRange(210);
    pt->setProgressPosition(176);

    getWidget(scroll, "InventoryScroller");
    scroll->setVisible(true);
    scroll->setTrackSize(16);
    scroll->setScrollRange(1);
    scroll->setScrollPage(ICON_SIZE+ICON_SPACING);


    scroll->eventScrollChangePosition = MyGUI::newDelegate(this,&InventoryWindow::onScrollChangePosition);

    getWidget(items, "Items");

    getWidget(mAvatarWidget, "Avatar");
    
    mAvatarWidget->eventMouseButtonClick=MyGUI::newDelegate(this, &InventoryWindow::onEquip);

    // Adjust armor rating text to bottom of avatar widget
    MyGUI::StaticTextPtr armor_rating;
    getWidget(armor_rating, "ArmorRating");
    armor_rating->setCaption("Armor: 11");
    MyGUI::IntCoord coord = armor_rating->getCoord();
    coord.top =mAvatarWidget->getCoord().height - 4 - coord.height;
    armor_rating->setCoord(coord);

    names[0] = "All";
    names[1] = "Weapon";
    names[2] = "Apparel";
    names[3] = "Magic";
    names[4] = "Misc";

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
        setText(name, names[mode]);
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
    mItems.push_back(ptr);
    refreshView(0);
  }

  void InventoryWindow::refreshView(int i)
  {
    MyGUI::StaticImagePtr Item;

    switch(i){
    case 0: //category change/new item. mindless smashing and recreating of all widgets
        for(std::map<MWWorld::Ptr, MyGUI::StaticImagePtr>::iterator it= mWItems.begin();it!= mWItems.end();it++){
            MyGUI::Gui::getInstance().destroyWidget(it->second);
        }
        mWItems.clear();
        x=4;
        y=4;
        for(std::vector<MWWorld::Ptr>::iterator it=mItems.begin();it!=mItems.end();it++){
            printf("typename:%s\n",it->getTypeName().c_str());
            if(categoryMode==CM_Misc && ! it->getTypeName().substr(8,13).compare("Miscellaneous")){
                Item=items->createWidget<MyGUI::StaticImage>("StaticImage", x, y, ICON_SIZE, ICON_SIZE, MyGUI::Align::Default );
                Item->setImageTexture("icons\\c\\tx_shirtcomm_03.dds");
                Item->eventMouseButtonClick=MyGUI::newDelegate(this,&InventoryWindow::onPick);
                mWItems.insert(std::make_pair(*it,Item));

                if(y+ICON_SIZE+ICON_SPACING+ICON_SIZE+16 > items->getClientCoord().bottom()){
                    y=4;
                    x+=ICON_SIZE+ICON_SPACING;

                }else{
                    y+=ICON_SIZE+ICON_SPACING;
                    if(x+ICON_SIZE+ICON_SPACING>items->getClientCoord().right()){
                        scroll->setScrollRange(x+ICON_SIZE+ICON_SPACING+lastPos-items->getClientCoord().right());
                        scroll->setScrollViewPage(scroll->getScrollRange());
                    }
                }

            }
        }
    break;
    case 1: //resize
    break;
    case 2: //FIXME: test. BUG PERSIST 
        for(std::multimap<std::string, MyGUI::StaticImagePtr>::iterator it= mWItemsTEST.begin();it!= mWItemsTEST.end();it++){
            MyGUI::Gui::getInstance().destroyWidget(it->second);
        }
        mWItemsTEST.clear();
    
        x=4-lastPos;
        y=4;

        for(std::vector<std::string>::iterator it=mItemsTEST.begin();it!=mItemsTEST.end();it++){
            Item=items->createWidget<MyGUI::StaticImage>("StaticImage", x, y, ICON_SIZE, ICON_SIZE, MyGUI::Align::Default );
            Item->setImageTexture(*it);
            Item->eventMouseButtonClick=MyGUI::newDelegate(this,&InventoryWindow::onPick);

            mWItemsTEST.insert(std::make_pair(*it,Item));

            if(y+ICON_SIZE+ICON_SPACING+ICON_SIZE+16 > items->getClientCoord().bottom()){
                y=4;
                x+=ICON_SIZE+ICON_SPACING;

            }else{
                y+=ICON_SIZE+ICON_SPACING;
                if(x+ICON_SIZE+ICON_SPACING>items->getClientCoord().right()){
                    scroll->setScrollRange(x+ICON_SIZE+ICON_SPACING+lastPos-items->getClientCoord().right());
                    scroll->setScrollViewPage(scroll->getScrollRange());
                }
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
/*
    for (std::map<MyGUI::StaticImagePtr,MWWorld::Ptr>::const_iterator it = mWItems.begin(); it != mWItems.end(); ++it)
    {
        (*it).second->setCoord((*it).second->getCoord() + MyGUI::IntPoint(diff, 0));
    }
*/
    //FIXME:TEST
    for (std::map<std::string,MyGUI::StaticImagePtr>::const_iterator it = mWItemsTEST.begin(); it != mWItemsTEST.end(); ++it)
    {
        (*it).second->setCoord((*it).second->getCoord() + MyGUI::IntPoint(diff, 0));
    }

  }
  void InventoryWindow::onPick(MyGUI::WidgetPtr _sender)
  {

	addItem("icons\\c\\tx_shirtcomm_03.dds"); //just for test

//    if(){ //many items with same name should be stacked and dialog on number to take popups
        //set separate mode, nothing can be done while in it (except ESC menu):
//    }
    mDrag=true;
  }
  void InventoryWindow::onEquip(MyGUI::Widget* _sender)
  {
    if(mDrag){

/*      //TODO: get type of Ptr somehow, and do gui accordingly
        if(mDragingItem type == "apparatus"){
            //show alchemy gui etc.
        }else if(mDragingItem type == "book"){

        }else if(mDragingItem type == "consumable"){

        }else if(mDragingItem type == "enchantment"){

        }else if(mDragingItem type == "repairment"){

        }else{ //armor/clothing and misc is rest
*/
        if(true){ //type wearable not misc
            //TODO:check for already equipped, redecorate it, and add newly equipped to invetory gui
            mEquipped[3]=mDragingItem;
        }
//        printf("%s equipped at slot:%i\n",mDragingItem.first.c_str(), mDragingItem.second);
        mDrag=false;

        }else{  //items can be stripped from avatar
    }
  }
    //FIXME: test
  void InventoryWindow::addItem(std::string str)
  {
    mItemsTEST.push_back(str);
    refreshView(2);
  }
} //namespace
