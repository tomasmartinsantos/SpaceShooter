#ifndef ESHIPCONTROLLABLE_H
#define ESHIPCONTROLLABLE_H

#include "../../Engine/Types.h"
#include "EShip.h"
#include "../../Engine/Input/IMObserver.h"


class EShipControllable : public EShip, public IIMObserver
{
    // Methods
public:
    static Ptr<EShipControllable> Create();
    static Ptr<EShipControllable> Create(const Transform2D& Tranform2D, float MaxLinearAcceleration = 0.0f, float MaxAngularAcceleration = 0.0f, float LinearInertiaSec = 0.0f, float AngularInertiaSec = 0.0f);

    virtual void Init();
    virtual void Update(float elapsed);
    virtual void Render();
    virtual void ManageEvent(const InputEvent& Event);

    void DeactivateInput() { mInputActivated = false; }
    void ActivateInput() { mInputActivated = true; }

protected:
    EShipControllable() { mType = EntityType::ENTITY_SHIPCONTROLLABLE; }
    EShipControllable(const Transform2D& Tranform2D, float MaxLinearAcceleration = 0.0f, float MaxAngularAcceleration = 0.0f, float LinearInertiaSec = 0.0f, float AngularInertiaSec = 0.0f);

    // Variables
private:
    bool mInputActivated;

    friend class Ptr<EShipControllable>;
    friend class Ptr<const EShipControllable>;
};

#endif