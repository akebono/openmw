
#include "inventory_window.hpp"
#include "../mwworld/class.hpp"
#include "../mwworld/environment.hpp"
#include "../mwworld/refdata.hpp"
namespace MWGui{
  InventoryWindow::InventoryWindow ()//MWWorld::ContainerStore<MWWorld::RefData>& conatiner)
    : Layout("openmw_inventory_window_layout.xml")
    , categoryMode(CM_All)

    // color should be fetched from skin
    , activeColor(0, 0, 1)
    , inactiveColor(0.7, 0.7, 0.7)
    , mDrag(false)
    , lastPos(0)
    , iIconSize(40)
    , iSpacingSize(8)
    , avatarAspect(2.217)
  {

//    player = environment.mWorld->getPtr ("player", true);
//    mContainer=container;

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

  InventoryWindow::InventoryWindow (MWWorld::ContainerStore<MWWorld::RefData> *container)
    : Layout("openmw_inventory_window_layout.xml")
    , categoryMode(CM_All)

    // color should be fetched from skin
    , activeColor(0, 0, 1)
    , inactiveColor(0.7, 0.7, 0.7)
    , mDrag(false)
    , lastPos(0)
    , iIconSize(40)
    , iSpacingSize(8)
    , avatarAspect(2.217)
  {

//    player = environment.mWorld->getPtr ("player", true);
//    MWWorld::ContainerStore<MWWorld::RefData>;
    //mContainer=container;

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
//    printf("avatar:%f:%i\n",avatarAspect,mAvatarWidget->getSize().height);
    mAvatarWidget->setSize(mAvatarWidget->getSize().height/avatarAspect,mAvatarWidget->getSize().height);
    mAvatarWidget->eventMouseButtonClick=MyGUI::newDelegate(this, &InventoryWindow::onAvatarClick);

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

//    refreshView(0);
  }

//TODO: commented prototype, and sorting, and stacking
//  void InventoryWindow::refreshView(int i)
  void InventoryWindow::refreshView(int i, MWWorld::ContainerStore<MWWorld::RefData> container)
  {
    MyGUI::StaticImagePtr Item;
    mContainer=container;

    std::string icon; // storage for icon name manipualtion

    switch(i){
      case 0: //category change/new item. mindless smashing and recreating of all widgets
        for(std::map<MyGUI::WidgetPtr, MWWorld::Ptr >::iterator it= mItems.begin();it!= mItems.end();it++)
            MyGUI::Gui::getInstance().destroyWidget(it->first);

        mItems.clear();

        x=4-lastPos;
        y=4;
        if(categoryMode==CM_All ||categoryMode==CM_Weapon){
            for(std::list<ESMS::LiveCellRef<ESM::Weapon, MWWorld::RefData> >::iterator it= mContainer.weapons.list.begin();it!= mContainer.weapons.list.end();it++){ //
                Item=items->createWidget<MyGUI::StaticImage>("StaticImage", x, y, iIconSize, iIconSize, MyGUI::Align::Default );

                MWWorld::Ptr ptr=MWWorld::Ptr(&*it, 0);

                icon=MWWorld::Class::get (ptr).getInventoryIcon (ptr);
                size_t found=icon.rfind(".tga");
                if (found!=std::string::npos)
                    icon.replace (found,strlen(".tga"),".dds");

                Item->setImageTexture("icons\\"+icon);
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
        } //category CM_Weapon
        if(categoryMode==CM_All ||categoryMode==CM_Apparel){
            for(std::list<ESMS::LiveCellRef<ESM::Armor, MWWorld::RefData> >::iterator it= mContainer.armors.list.begin();it!= mContainer.armors.list.end();it++){
                Item=items->createWidget<MyGUI::StaticImage>("StaticImage", x, y, iIconSize, iIconSize, MyGUI::Align::Default );
                MWWorld::Ptr ptr=MWWorld::Ptr(&*it, 0);
                icon=MWWorld::Class::get (ptr).getInventoryIcon (ptr);
                size_t found=icon.rfind(".tga");
                if (found!=std::string::npos)
                    icon.replace (found,strlen(".tga"),".dds");

                Item->setImageTexture("icons\\"+icon);
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
        } //category CM_Apparel
        if(categoryMode==CM_All ||categoryMode==CM_Magic){ //there are should be more items
            for(std::list<ESMS::LiveCellRef<ESM::Weapon, MWWorld::RefData> >::iterator it= mContainer.weapons.list.begin();it!= mContainer.weapons.list.end();it++){
                Item=items->createWidget<MyGUI::StaticImage>("StaticImage", x, y, iIconSize, iIconSize, MyGUI::Align::Default );

                MWWorld::Ptr ptr=MWWorld::Ptr(&*it, 0);
                icon=MWWorld::Class::get (ptr).getInventoryIcon (ptr);
                size_t found=icon.rfind(".tga");
                if (found!=std::string::npos)
                    icon.replace (found,strlen(".tga"),".dds");

                Item->setImageTexture("icons\\"+icon);//MWWorld::Class::get (ptr).getInventoryIcon (ptr));
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
            } //magic Weapon
            for(std::list<ESMS::LiveCellRef<ESM::Armor, MWWorld::RefData> >::iterator it= mContainer.armors.list.begin();it!= mContainer.armors.list.end();it++){
                Item=items->createWidget<MyGUI::StaticImage>("StaticImage", x, y, iIconSize, iIconSize, MyGUI::Align::Default );
/*                if(it->base->enchant != 0)
                    printf("enchanted?\n");
                else
                    printf("not enchanted?\n");
*/
                MWWorld::Ptr ptr=MWWorld::Ptr(&*it, 0);
                icon=MWWorld::Class::get (ptr).getInventoryIcon (ptr);
                size_t found=icon.rfind(".tga");
                if (found!=std::string::npos)
                    icon.replace (found,strlen(".tga"),".dds");

                Item->setImageTexture("icons\\"+icon);//MWWorld::Class::get (ptr).getInventoryIcon (ptr));
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
            } //magic Armor
            for(std::list<ESMS::LiveCellRef<ESM::Potion, MWWorld::RefData> >::iterator it= mContainer.potions.list.begin();it!= mContainer.potions.list.end();it++){
                Item=items->createWidget<MyGUI::StaticImage>("StaticImage", x, y, iIconSize, iIconSize, MyGUI::Align::Default );

                MWWorld::Ptr ptr=MWWorld::Ptr(&*it, 0);
                icon=MWWorld::Class::get (ptr).getInventoryIcon (ptr);
                size_t found=icon.rfind(".tga");
                if (found!=std::string::npos)
                    icon.replace (found,strlen(".tga"),".dds");

                Item->setImageTexture("icons\\"+icon);//MWWorld::Class::get (ptr).getInventoryIcon (ptr));
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
            } //Potion
            for(std::list<ESMS::LiveCellRef<ESM::Book, MWWorld::RefData> >::iterator it= mContainer.books.list.begin();it!= mContainer.books.list.end();it++){
                Item=items->createWidget<MyGUI::StaticImage>("StaticImage", x, y, iIconSize, iIconSize, MyGUI::Align::Default );

                MWWorld::Ptr ptr=MWWorld::Ptr(&*it, 0);
                icon=MWWorld::Class::get (ptr).getInventoryIcon (ptr);
                size_t found=icon.rfind(".tga");
                if (found!=std::string::npos)
                    icon.replace (found,strlen(".tga"),".dds");

                Item->setImageTexture("icons\\"+icon);//MWWorld::Class::get (ptr).getInventoryIcon (ptr));
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
            } //Scroll

        } //category CM_Magic
        if(categoryMode==CM_All ||categoryMode==CM_Misc){
            for(std::list<ESMS::LiveCellRef<ESM::Ingredient, MWWorld::RefData> >::iterator it= mContainer.ingreds.list.begin();it!= mContainer.ingreds.list.end();it++){
                Item=items->createWidget<MyGUI::StaticImage>("StaticImage", x, y, iIconSize, iIconSize, MyGUI::Align::Default );

                MWWorld::Ptr ptr=MWWorld::Ptr(&*it, 0);
                icon=MWWorld::Class::get (ptr).getInventoryIcon (ptr);
                size_t found=icon.rfind(".tga");
                if (found!=std::string::npos)
                    icon.replace (found,strlen(".tga"),".dds");

                Item->setImageTexture("icons\\"+icon);//MWWorld::Class::get (ptr).getInventoryIcon (ptr));
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
            } //Ingredient
            for(std::list<ESMS::LiveCellRef<ESM::Apparatus, MWWorld::RefData> >::iterator it= mContainer.appas.list.begin();it!= mContainer.appas.list.end();it++){
                Item=items->createWidget<MyGUI::StaticImage>("StaticImage", x, y, iIconSize, iIconSize, MyGUI::Align::Default );

                MWWorld::Ptr ptr=MWWorld::Ptr(&*it, 0);
                icon=MWWorld::Class::get (ptr).getInventoryIcon (ptr);
                size_t found=icon.rfind(".tga");
                if (found!=std::string::npos)
                    icon.replace (found,strlen(".tga"),".dds");

                Item->setImageTexture("icons\\"+icon);//MWWorld::Class::get (ptr).getInventoryIcon (ptr));
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
            } //Apparatus
            for(std::list<ESMS::LiveCellRef<ESM::Book, MWWorld::RefData> >::iterator it= mContainer.books.list.begin();it!= mContainer.books.list.end();it++){
                Item=items->createWidget<MyGUI::StaticImage>("StaticImage", x, y, iIconSize, iIconSize, MyGUI::Align::Default );

                MWWorld::Ptr ptr=MWWorld::Ptr(&*it, 0);
                icon=MWWorld::Class::get (ptr).getInventoryIcon (ptr);
                size_t found=icon.rfind(".tga");
                if (found!=std::string::npos)
                    icon.replace (found,strlen(".tga"),".dds");

                Item->setImageTexture("icons\\"+icon);//MWWorld::Class::get (ptr).getInventoryIcon (ptr));
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
            } //Book
            for(std::list<ESMS::LiveCellRef<ESM::Tool, MWWorld::RefData> >::iterator it= mContainer.lockpicks.list.begin();it!= mContainer.lockpicks.list.end();it++){
                Item=items->createWidget<MyGUI::StaticImage>("StaticImage", x, y, iIconSize, iIconSize, MyGUI::Align::Default );

                MWWorld::Ptr ptr=MWWorld::Ptr(&*it, 0);
                icon=MWWorld::Class::get (ptr).getInventoryIcon (ptr);
                size_t found=icon.rfind(".tga");
                if (found!=std::string::npos)
                    icon.replace (found,strlen(".tga"),".dds");

                Item->setImageTexture("icons\\"+icon);//MWWorld::Class::get (ptr).getInventoryIcon (ptr));
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
            } //Lockpicks
            for(std::list<ESMS::LiveCellRef<ESM::Miscellaneous, MWWorld::RefData> >::iterator it= mContainer.miscItems.list.begin();it!= mContainer.miscItems.list.end();it++){
                Item=items->createWidget<MyGUI::StaticImage>("StaticImage", x, y, iIconSize, iIconSize, MyGUI::Align::Default );

                MWWorld::Ptr ptr=MWWorld::Ptr(&*it, 0);
                icon=MWWorld::Class::get (ptr).getInventoryIcon (ptr);
                size_t found=icon.rfind(".tga");
                if (found!=std::string::npos)
                    icon.replace (found,strlen(".tga"),".dds");

                Item->setImageTexture("icons\\"+icon);//MWWorld::Class::get (ptr).getInventoryIcon (ptr));
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
            } //Miscellaneous (keys, gold, soulgems, propylon indexes etc.)
            for(std::list<ESMS::LiveCellRef<ESM::Probe, MWWorld::RefData> >::iterator it= mContainer.probes.list.begin();it!= mContainer.probes.list.end();it++){
                Item=items->createWidget<MyGUI::StaticImage>("StaticImage", x, y, iIconSize, iIconSize, MyGUI::Align::Default );

                MWWorld::Ptr ptr=MWWorld::Ptr(&*it, 0);
                icon=MWWorld::Class::get (ptr).getInventoryIcon (ptr);
                size_t found=icon.rfind(".tga");
                if (found!=std::string::npos)
                    icon.replace (found,strlen(".tga"),".dds");

                Item->setImageTexture("icons\\"+icon);//MWWorld::Class::get (ptr).getInventoryIcon (ptr));
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
            } //Probes

        } //category CM_Misc
      break;
      case 1: //resize

        x=4-lastPos;
        y=4;
        for(std::map<MyGUI::WidgetPtr, MWWorld::Ptr >::iterator it=mItems.begin(); it!=mItems.end(); it++){
            it->first->setPosition(x,y);
            if(y+2*iIconSize+iSpacingSize+16 > items->getClientCoord().bottom()){
                y=4;
                x+=iIconSize+iSpacingSize;

            }else{
                y+=iIconSize+iSpacingSize;
                if(x+iIconSize+iSpacingSize>items->getClientCoord().right()){
                    scroll->setScrollRange(x+iIconSize+iSpacingSize+lastPos-items->getClientCoord().right());
                    scroll->setScrollViewPage(scroll->getScrollRange());
                }else{
                    scroll->setScrollPosition(0);
                    scroll->setScrollRange(1);
                    lastPos=0;
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

    for (std::map<MyGUI::WidgetPtr,MWWorld::Ptr>::const_iterator it = mItems.begin(); it != mItems.end(); ++it)
    {
        it->first->setCoord(it->first->getCoord() + MyGUI::IntPoint(diff, 0));
    }
  }

  void InventoryWindow::onInventoryClick(MyGUI::WidgetPtr _sender)
  {

//    MyGUI::StaticImagePtr Item;
//    Item=items->createWidget<MyGUI::StaticImage>("StaticImage", x, y, iIconSize, iIconSize, MyGUI::Align::Default );
//    Item->setImageTexture("icons\\c\\tx_shirtcomm_03.dds");
    if(!mDrag){ //drag
//        if(){ //many items with same name should be stacked and dialog on number to take popups
            //set separate mode, nothing can be done while in it (except ESC menu):
//        }
        mDrag=true;
    }else{ //drop the thing to inventory

        mDrag=false;
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
//FIXME:container pointer 
    refreshView(1,mContainer);
  }


} //namespace
