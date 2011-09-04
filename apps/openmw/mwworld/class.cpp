
#include "class.hpp"

#include <stdexcept>

#include <OgreVector3.h>

#include "ptr.hpp"
#include "nullaction.hpp"

namespace MWWorld
{
    std::map<std::string, boost::shared_ptr<Class> > Class::sClasses;

    Class::Class() {}

    Class::~Class() {}

    std::string Class::getId (const Ptr& ptr) const
    {
        throw std::runtime_error ("class does not support ID retrieval");
    }

    void Class::insertObj (const Ptr& ptr, MWRender::CellRenderImp& cellRender,
        MWWorld::Environment& environment) const
    {

    }

    void Class::enable (const Ptr& ptr, MWWorld::Environment& environment) const
    {

    }

    void Class::disable (const Ptr& ptr, MWWorld::Environment& environment) const
    {

    }

    MWMechanics::CreatureStats& Class::getCreatureStats (const Ptr& ptr) const
    {
        throw std::runtime_error ("class does not have creature stats");
    }

    MWMechanics::NpcStats& Class::getNpcStats (const Ptr& ptr) const
    {
        throw std::runtime_error ("class does not have NPC stats");
    }

    bool Class::hasItemHealth (const Ptr& ptr) const
    {
        return false;
    }

    int Class::getItemMaxHealth (const Ptr& ptr) const
    {
        throw std::runtime_error ("class does not have item health");
    }

    boost::shared_ptr<Action> Class::activate (const Ptr& ptr, const Ptr& actor,
        const Environment& environment) const
    {
        return boost::shared_ptr<Action> (new NullAction);
    }

    boost::shared_ptr<Action> Class::use (const Ptr& ptr,
        const Environment& environment) const
    {
        return boost::shared_ptr<Action> (new NullAction);
    }

    ContainerStore<RefData>& Class::getContainerStore (const Ptr& ptr) const
    {
        throw std::runtime_error ("class does not have a container store");
    }

    void Class::insertIntoContainer (const Ptr& ptr, ContainerStore<RefData>& containerStore) const
    {
        throw std::runtime_error ("class does not support inserting into a container");
    }

    void Class::removeFromContainer (const Ptr& ptr, ContainerStore<RefData>& containerStore) const
    {
        throw std::runtime_error ("class does not support removing from a container");
    }

    void Class::lock (const Ptr& ptr, int lockLevel) const
    {
        throw std::runtime_error ("class does not support locking");
    }

    void Class::unlock (const Ptr& ptr) const
    {
        throw std::runtime_error ("class does not support unlocking");
    }

    std::string Class::getScript (const Ptr& ptr) const
    {
        return "";
    }

    void Class::setForceStance (const Ptr& ptr, Stance stance, bool force) const
    {
        throw std::runtime_error ("stance not supported by class");
    }

    void Class::setStance (const Ptr& ptr, Stance stance, bool set) const
    {
        throw std::runtime_error ("stance not supported by class");
    }

    bool Class::getStance (const Ptr& ptr, Stance stance, bool ignoreForce) const
    {
        return false;
    }

    float Class::getSpeed (const Ptr& ptr) const
    {
        return 0;
    }

    MWMechanics::Movement& Class::getMovementSettings (const Ptr& ptr) const
    {
        throw std::runtime_error ("movement settings not supported by class");
    }

    Ogre::Vector3 Class::getMovementVector (const Ptr& ptr) const
    {
        return Ogre::Vector3 (0, 0, 0);
    }

    std::string Class::getInventoryIcon (const Ptr& ptr) const
    {
        throw std::runtime_error (
            "this class can not be put into an inventory; no inventory icon available");
    }

    const Class& Class::get (const std::string& key)
    {
        std::map<std::string, boost::shared_ptr<Class> >::const_iterator iter = sClasses.find (key);

        if (iter==sClasses.end())
            throw std::logic_error ("unknown class key: " + key);

        return *iter->second;
    }

    const Class& Class::get (const Ptr& ptr)
    {
        return get (ptr.getTypeName());
    }

    void Class::registerClass (const std::string& key,  boost::shared_ptr<Class> instance)
    {
        sClasses.insert (std::make_pair (key, instance));
    }
}
