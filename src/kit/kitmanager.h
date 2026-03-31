#ifndef KITMANAGER_H
#define KITMANAGER_H

#include "abstractkitmanager.h"

class KitManager : public AbstractKitManager
{
protected:
    bool canAddKit(const Kit &kit) const override;
    bool canReplaceKit(const Kit &newKit, const Kit &oldKit) const override;
    bool canRemoveKit(const Kit &kit) const override;
};

#endif // KITMANAGER_H
