#include "PrecompiledHeader.h"
#include "ScriptSystem.h"

void ScriptSystem::Start(EntityManager& entityManager, ComponentManager& componentManager)
{
    for (Entity entity : entityManager.GetActiveEntities()) {
        ScriptComponent* script = componentManager.GetScript(entity, ObjectType::Object);
        if (script && script->startFunc) {
            script->startFunc(script->id, script->type, script->ptr);
        }
    }
}

void ScriptSystem::Update(EntityManager& entityManager, ComponentManager& componentManager)
{
    for (Entity entity : entityManager.GetActiveEntities()) {
        ScriptComponent* script = componentManager.GetScript(entity, ObjectType::Object);
        if (script&&script->updateFunc) {
            script->updateFunc(script->id, script->type, script->ptr);
        }
    }
}

void ScriptSystem::Cleanup(EntityManager& entityManager, ComponentManager& componentManager)
{
    for (Entity entity : entityManager.GetActiveEntities()) {
        ScriptComponent* script = componentManager.GetScript(entity, ObjectType::Object);
        if (script && script->cleanupFunc) {
            script->cleanupFunc();
        }
    }
}
