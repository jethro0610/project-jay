#include "EditorMode.h"

class Entity;

class ModifyEntityMode : public EditorMode {
public:
    ModifyEntityMode(EditorModeArgs args); 
    int GetBinding() override { return 'M'; }
    std::string GetName() override { return "Modify Entity"; };
    void OnStart() override;
    void OnEnd() override;
    void OnCancel() override;
    ConfirmBehavior OnConfirm() override;
    void Update() override;

private:
    EditorTarget& target_;
    Entity* entity_;
};
