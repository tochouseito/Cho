#include "PrecompiledHeader.h"
#include "ScriptSystem.h"

void ScriptSystem::Start(EntityManager& entityManager, ComponentManager& componentManager)
{
    for (Entity entity : entityManager.GetActiveEntities()) {
        ScriptComponent* script = componentManager.GetScript(entity,Type::Object);
        script;
    }
}

void ScriptSystem::Update(EntityManager& entityManager, ComponentManager& componentManager)
{
    for (Entity entity : entityManager.GetActiveEntities()) {
        ScriptComponent* script = componentManager.GetScript(entity, Type::Object);
        if (script) {
            script->Update();
        }
    }
   
}
