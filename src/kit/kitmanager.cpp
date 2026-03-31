#include "kitmanager.h"

bool KitManager::canAddKit(const Kit &kit) const
{
    return kit.isValid();
}

bool KitManager::canReplaceKit(const Kit &newKit, const Kit &oldKit) const
{
    Q_UNUSED(oldKit);
    return newKit.isValid();
}

bool KitManager::canRemoveKit(const Kit &kit) const
{
    Q_UNUSED(kit);
    return true;
}
