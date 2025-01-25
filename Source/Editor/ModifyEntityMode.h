#include "EditorMode.h"
#include "Entity/EntityProperties.h"

class Entity;

class ModifyEntityMode : public EditorMode {
public:
    enum ModifyState {
        SelectProperty,
        ModifyProperty
    };

    ModifyEntityMode(EditorModeArgs args); 
    int GetBinding() override { return 'M'; }
    std::string GetName() override { return "Modify Entity"; };
    void OnStart() override;
    void OnEnd() override;
    void OnCancel() override;
    ConfirmBehavior OnConfirm() override;
    void Update() override;

    bool SetSelectedProperty(const std::string& property);
    bool ModifySelectedProperty(const std::string& value);

private:
    EntityProperties properties_;
    ModifyState state_;
    std::string property_;

    float* propFloat_;
    int* propInt_;
    bool* propBool_;
};
