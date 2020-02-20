#include "../../Engine/Input/InputEvent.h"
#include "../../Engine/Input/InputManager.h"
#include "../Components/CWeapon.h"
#include "../../Engine/Math.h"
#include "../UI/WLifebar.h"
#include "../Components/HeadersComponents.h"
#include "../../Engine/Audio/Audiosource.h"
#include "../../Engine/Input/Controller.h"
#include "EShipControllable.h"


Ptr<EShipControllable> EShipControllable::Create()
{
    Ptr<EShipControllable> p = new EShipControllable();
    p->mThis = p.UpCast<Entity>();
    return p;
}

Ptr<EShipControllable> EShipControllable::Create(const Transform2D& Tranform2D, float MaxLinearAcceleration, float MaxAngularAcceleration, float LinearInertiaSec, float AngularInertiaSec)
{
    Ptr<EShipControllable> p = new EShipControllable(Tranform2D, MaxLinearAcceleration, MaxAngularAcceleration, LinearInertiaSec, AngularInertiaSec);
    p->mThis = p.UpCast<Entity>();
    return p;
}

EShipControllable::EShipControllable(const Transform2D& Tranform2D, float MaxLinearAcceleration, float MaxAngularAcceleration, float LinearInertiaSec, float AngularInertiaSec)
{
    mType = EntityType::ENTITY_SHIPCONTROLLABLE;
    SetTransform(Tranform2D);
    SetTickMovement(true);
    SetMaxLinearAcc(MaxLinearAcceleration);
    SetMaxAngularAcc(MaxAngularAcceleration);
    SetLinearInertia(LinearInertiaSec);
    SetAngularInertia(AngularInertiaSec);

    mInputActivated = true;
    ActivateMovementController();
}

void EShipControllable::Init()
{
    EShip::Init();
    // All the inputs events that a controllable ship should observe
    std::vector<InputEvent::TEvent> EventsToObserve;
    EventsToObserve.push_back(InputEvent::EVENT_KEY_UP_PRESSED);
    EventsToObserve.push_back(InputEvent::EVENT_KEY_UP_RELEASED);
    EventsToObserve.push_back(InputEvent::EVENT_KEY_RIGHT_PRESSED);
    EventsToObserve.push_back(InputEvent::EVENT_KEY_RIGHT_RELEASED);
    EventsToObserve.push_back(InputEvent::EVENT_KEY_LEFT_PRESSED);
    EventsToObserve.push_back(InputEvent::EVENT_KEY_LEFT_RELEASED);
    EventsToObserve.push_back(InputEvent::EVENT_KEY_SPACE_PRESSED);
    EventsToObserve.push_back(InputEvent::EVENT_KEY_SPACE_RELEASED);
    EventsToObserve.push_back(InputEvent::EVENT_KEY_TAB_PRESSED);
    EventsToObserve.push_back(InputEvent::EVENT_KEY_TAB_RELEASED);
    
    // Pass these events to the InputMananger
    IInputManagerObserver::Instance()->AddObserver((mThis.DownCast<EShipControllable>()).UpCast<IIMObserver>(), EventsToObserve);
}

void EShipControllable::Update(float elapsed)
{
    EShip::Update(elapsed);
}

void EShipControllable::Render()
{
    EShip::Render();
}

void EShipControllable::ManageEvent(const InputEvent& Event)
{
    // How to manage the input event
    if (mInputActivated)
    {
        InputEvent::TEvent TypeEvent = Event.GetTEvent();

        switch (TypeEvent)
        {
        case InputEvent::EVENT_NONE:
        {
            break;
        }
        case InputEvent::EVENT_MOUSE_RIGHT_CLICK:
        {
            break;
        }
        case InputEvent::EVENT_MOUSE_LEFT_CLICK:
        {
            break;
        }
        case InputEvent::EVENT_KEY_SPACE_PRESSED:
        {
            if (IsPrimaryWeaponActive())
            {
                GetPrimaryWeaponComp()->Fire(GetProjectileSpawnPos(), GetRotation());
                SetPrimaryWeaponActive(false);
            }
            break;
        }
        case InputEvent::EVENT_KEY_SPACE_RELEASED:
        {
            SetPrimaryWeaponActive(true);
            break;
        }
        case InputEvent::EVENT_KEY_TAB_PRESSED:
        {
            if (IsSecondaryWeaponActive())
            {
                GetSecondaryWeaponComp()->Fire(GetProjectileSpawnPos(), GetRotation());
                SetSecondaryWeaponActive(false);
            }
            break;
        }
        case InputEvent::EVENT_KEY_TAB_RELEASED:
        {
            SetSecondaryWeaponActive(true);
            break;
        }
        case InputEvent::EVENT_KEY_UP_PRESSED:
        {
            if(IsMovementControllerActivated())
            {
                DeactivateLinearInertia();
                SetLinearSteering(vec2(GetForwardVector().x * GetMaxLinearAcc(), GetForwardVector().y* GetMaxLinearAcc()));
            }
            break;
        }
        case InputEvent::EVENT_KEY_UP_RELEASED:
        {
            if (IsMovementControllerActivated())
            {
                ActivateLinearInertia();
            }
            break;
        }
        case InputEvent::EVENT_KEY_DOWN_PRESSED:
        {
            break;
        }
        case InputEvent::EVENT_KEY_RIGHT_PRESSED:
        {
            if (IsMovementControllerActivated())
            {
                DeactivateAngularInertia();
                SetAngularSteering(-GetMaxAngularAcc());
                SetTurnDirection(-1);
            }
            break;
        }
        case InputEvent::EVENT_KEY_RIGHT_RELEASED:
        {
            if (IsMovementControllerActivated())
            {
                ActivateAngularInertia();
            }
            break;
        }
        case InputEvent::EVENT_KEY_LEFT_PRESSED:
        {
            if (IsMovementControllerActivated())
            {
                DeactivateAngularInertia();
                SetAngularSteering(GetMaxAngularAcc());
                SetTurnDirection(1);
            }
            break;
        }
        case InputEvent::EVENT_KEY_LEFT_RELEASED:
        {
            if (IsMovementControllerActivated())
            {
                ActivateAngularInertia();
            }
            break;
        }
        default:
            break;
        }
    }
}