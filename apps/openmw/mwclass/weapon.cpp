
#include "weapon.hpp"

#include <components/esm/loadweap.hpp>

#include <components/esm_store/cell_store.hpp>

#include "../mwworld/ptr.hpp"
#include "../mwworld/actiontake.hpp"

#include "../mwrender/cellimp.hpp"

#include "containerutil.hpp"

namespace MWClass
{
    void Weapon::insertObj (const MWWorld::Ptr& ptr, MWRender::CellRenderImp& cellRender,
        MWWorld::Environment& environment) const
    {
        ESMS::LiveCellRef<ESM::Weapon, MWWorld::RefData> *ref =
            ptr.get<ESM::Weapon>();

        assert (ref->base != NULL);
        const std::string &model = ref->base->model;
        if (!model.empty())
        {
            MWRender::Rendering rendering (cellRender, ref->ref);
            cellRender.insertMesh ("meshes\\" + model);
            cellRender.insertObjectPhysics();
            ref->mData.setHandle (rendering.end (ref->mData.isEnabled()));
        }
    }

    std::string Weapon::getName (const MWWorld::Ptr& ptr) const
    {
        ESMS::LiveCellRef<ESM::Weapon, MWWorld::RefData> *ref =
            ptr.get<ESM::Weapon>();

        return ref->base->name;
    }

    boost::shared_ptr<MWWorld::Action> Weapon::activate (const MWWorld::Ptr& ptr,
        const MWWorld::Ptr& actor, const MWWorld::Environment& environment) const
    {
        return boost::shared_ptr<MWWorld::Action> (
            new MWWorld::ActionTake (ptr));
    }

    bool Weapon::hasItemHealth (const MWWorld::Ptr& ptr) const
    {
        return true;
    }

    int Weapon::getItemMaxHealth (const MWWorld::Ptr& ptr) const
    {
        ESMS::LiveCellRef<ESM::Weapon, MWWorld::RefData> *ref =
            ptr.get<ESM::Weapon>();

        return ref->base->data.health;
    }

    void Weapon::insertIntoContainer (const MWWorld::Ptr& ptr,
        MWWorld::ContainerStore<MWWorld::RefData>& containerStore) const
    {
        insertIntoContainerStore (ptr, containerStore.weapons);
    }

    void Weapon::removeFromContainer (const MWWorld::Ptr& ptr,
        MWWorld::ContainerStore<MWWorld::RefData>& containerStore) const
    {
        removeFromContainerStore (ptr, containerStore.weapons);
    }

    std::string Weapon::getScript (const MWWorld::Ptr& ptr) const
    {
        ESMS::LiveCellRef<ESM::Weapon, MWWorld::RefData> *ref =
            ptr.get<ESM::Weapon>();

        return ref->base->script;
    }

    std::string Weapon::getInventoryIcon (const MWWorld::Ptr& ptr) const
    {
        ESMS::LiveCellRef<ESM::Weapon, MWWorld::RefData> *ref =
            ptr.get<ESM::Weapon>();

        return ref->base->icon;
    }

    void Weapon::registerSelf()
    {
        boost::shared_ptr<Class> instance (new Weapon);

        registerClass (typeid (ESM::Weapon).name(), instance);
    }
}
