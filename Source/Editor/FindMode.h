#include "EditorMode.h"

class EntityS;

class FindMode : public EditorMode {
public:
    FindMode(EditorModeArgs args); 
    int GetBinding() override { return 'F'; }
    std::string GetName() override { return "Find Entity"; };
    void OnStart() override;
    void OnEnd() override;
    ConfirmBehavior OnConfirm() override;
    void Update() override;

private:
    int lastFindIndex_;
};
