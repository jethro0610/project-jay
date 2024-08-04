#pragma once
#include "Entity.h"

class TimerPod : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static constexpr const char* GetName() { return "e_timerpod"; }
    static EntityDependendies GetDeps();

    int timer_;

    void Update(); 
    void RenderUpdate() {};
    void OnHit(HitArgs args) {};
    void OnHurt(HurtArgs args);
    void OnCaptureSeed() {};
    void OnDestroy() {};
    void OnPush(glm::vec3 pushVec) {}

    #ifdef _DEBUG
    static bool DBG_Modifyable() { return true; };
    void DBG_OnStartModify(EditorTextInput& textInput, EditorNotification& notification);
    void DBG_OnCancelModify(EditorTextInput& textInput, EditorNotification& notification);
    void DBG_UpdateModify(EditorTextInput& textInput, EditorNotification& notification);
    ConfirmBehavior DBG_OnConfirmModify(EditorTextInput& textInput, EditorNotification& notification);
    #endif
};
