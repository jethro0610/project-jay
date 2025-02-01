#include "EditorMode.h"
#include <unordered_map>

class Entity;

class GenericScalarMode : public EditorMode {
public:
    GenericScalarMode (EditorModeArgs args); 
    int GetBinding() override { return '`'; }
    void SetScalarId(char id) { curScalarId_ = id; };
    std::string GetName() override;
    void OnStart() override;
    void OnCancel() override;
    void OnEnd() override;
    void Update() override;

private:
    float originalValue_;
    float delta_;
    char curScalarId_;
    std::unordered_map<char, std::string> scalarNames_;
};
