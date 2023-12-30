/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.1
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

void freeObject(Object* obj);
void freeScope(Scope* scope);
void freeObjectList(ObjectNode *objList);
void freeReferenceList(ObjectNode *objList);

SymTab* symtab;
Type* intType;
Type* charType;

/******************* Type utilities ******************************/

Type* makeIntType(void) {
  Type* type = (Type*) malloc(sizeof(Type));
  type->typeClass = TP_INT;
  return type;
}

Type* makeCharType(void) {
  Type* type = (Type*) malloc(sizeof(Type));
  type->typeClass = TP_CHAR;
  return type;
}

Type* makeArrayType(int arraySize, Type* elementType) {
  Type* type = (Type*) malloc(sizeof(Type));
  type->typeClass = TP_ARRAY;
  type->arraySize = arraySize;
  type->elementType = elementType;
  return type;
}

Type* duplicateType(Type* type) {
  // TODO
  Type* type_new = (Type*) malloc(sizeof(Type));
  type_new->arraySize = type->arraySize;
  type_new->elementType = type->elementType;
  type_new->typeClass = type->typeClass;
}

int compareType(Type* type1, Type* type2) {
  // TODO
  //return 1: equal or return 0: different
  if(type1->typeClass == type2->typeClass){
    if(type1->typeClass == TP_ARRAY){
      if((type1->arraySize == type2->arraySize) && (type1->elementType == type2->elementType)){
        return 1;
      } else {
        return 0;
      }
    }
    return 1;
  }
  return 0;
}

void freeType(Type* type) {
  // TODO
  if(type->elementType != NULL){
    freeType(type->elementType);
    free(type);
    type = NULL;
  }
}

/******************* Constant utility ******************************/

ConstantValue* makeIntConstant(int i) {
  // TODO
  ConstantValue *const_value = (ConstantValue*) malloc(sizeof(ConstantValue));
  const_value->type = TP_INT;
  const_value->intValue = i;
}

ConstantValue* makeCharConstant(char ch) {
  // TODO
  ConstantValue *const_value = (ConstantValue*) malloc(sizeof(ConstantValue));
  const_value->type = TP_CHAR;
  const_value->charValue = ch;
}

ConstantValue* duplicateConstantValue(ConstantValue* v) {
  // TODO
  ConstantValue *const_value_new = (ConstantValue*) malloc(sizeof(ConstantValue));
  const_value_new->type = v->type;
  if(v->type == TP_CHAR){
    const_value_new->charValue = v->charValue;
  } else if(v->type == TP_INT){
    const_value_new->intValue = v->intValue;
  }
  return const_value_new;
}

/******************* Object utilities ******************************/

Scope* CreateScope(Object* owner, Scope* outer) {
  Scope* scope = (Scope*) malloc(sizeof(Scope));
  scope->objList = NULL;
  scope->owner = owner;
  scope->outer = outer;
  return scope;
}

Object* CreateProgramObject(char *programName) {
  Object* program = (Object*) malloc(sizeof(Object));
  strcpy(program->name, programName);
  program->kind = OBJ_PROGRAM;
  program->progAttrs = (ProgramAttributes*) malloc(sizeof(ProgramAttributes));
  program->progAttrs->scope = CreateScope(program,NULL);
  symtab->program = program;

  return program;
}

Object* CreateConstantObject(char *name) {
  // TODO
  Object *constant_object = (Object*) malloc(sizeof(Object));
  strcpy(constant_object->name, name);
  constant_object->kind = OBJ_CONSTANT;
  constant_object->constAttrs = (ConstantAttributes*) malloc(sizeof(ConstantAttributes));
  constant_object->constAttrs->value = NULL;
  return constant_object;
}

Object* CreateTypeObject(char *name) {
  // TODO
  Object *type_object = (Object*) malloc(sizeof(Object));
  strcpy(type_object->name, name);
  type_object->kind = OBJ_TYPE;
  type_object->typeAttrs = (TypeAttributes*) malloc(sizeof(TypeAttributes));
  type_object->typeAttrs->actualType = NULL;
  return type_object;
}

Object* CreateVariableObject(char *name) {
  // TODO
  Object *variable_object = (Object*) malloc(sizeof(Object));
  strcpy(variable_object->name, name);
  variable_object->kind = OBJ_VARIABLE;
  variable_object->varAttrs = (VariableAttributes*) malloc(sizeof(VariableAttributes));
  variable_object->varAttrs->type = NULL;
  variable_object->varAttrs->scope = symtab->currentScope;
  return variable_object;
}

Object* CreateFunctionObject(char *name) {
  // TODO
  Object *function_object = (Object*) malloc(sizeof(Object));
  strcpy(function_object->name, name);
  function_object->kind = OBJ_FUNCTION;
  function_object->funcAttrs = (FunctionAttributes*) malloc(sizeof(FunctionAttributes));
  function_object->funcAttrs->returnType = NULL;
  function_object->funcAttrs->paramList = NULL;
  function_object->funcAttrs->scope = CreateScope(function_object, symtab->currentScope);
  return function_object;
}

Object* CreateProcedureObject(char *name) {
  // TODO
  Object *procedure_object = (Object*) malloc(sizeof(Object));
  strcpy(procedure_object->name, name);
  procedure_object->kind = OBJ_PROCEDURE;
  procedure_object->procAttrs = (ProcedureAttributes*) malloc(sizeof(ProcedureAttributes));
  procedure_object->procAttrs->paramList = NULL;
  procedure_object->procAttrs->scope = CreateScope(procedure_object, symtab->currentScope);
  return procedure_object;
}

Object* CreateParameterObject(char *name, enum ParamKind kind, Object* owner) {
  // TODO
  Object *parameter_object = (Object*) malloc(sizeof(Object));
  strcpy(parameter_object->name, name);
  parameter_object->kind = OBJ_PARAMETER;
  parameter_object->paramAttrs = (ParameterAttributes*) malloc(sizeof(ParameterAttributes));
  parameter_object->paramAttrs->kind = kind;
  parameter_object->paramAttrs->function = owner;
  parameter_object->paramAttrs->type = NULL;
  return parameter_object;
}

void freeObject(Object* obj) {
  // TODO
  if(obj != NULL){
    if(obj->progAttrs != NULL){
      if(obj->kind == OBJ_CONSTANT){
        if(obj->constAttrs->value != NULL){
          free(obj->constAttrs->value);
          obj->constAttrs->value = NULL;
        }
      } else if(obj->kind == OBJ_VARIABLE){
        if(obj->varAttrs->type != NULL){
          free(obj->varAttrs->type);
          obj->varAttrs->type = NULL;
        }
      } else if(obj->kind == OBJ_TYPE){
        if(obj->typeAttrs->actualType != NULL){
          free(obj->typeAttrs->actualType);
          obj->typeAttrs->actualType = NULL;
        }
      } else if(obj->kind == OBJ_PROGRAM){
        if(obj->progAttrs->scope != NULL){
          freeScope(obj->progAttrs->scope);
          obj->progAttrs->scope = NULL;
        }
      } else if(obj->kind == OBJ_FUNCTION){
        freeScope(obj->funcAttrs->scope);
      } else if(obj->kind == OBJ_PARAMETER){
        if(obj->paramAttrs->type != NULL){
          free(obj->paramAttrs->type);
          obj->paramAttrs->type = NULL;
        }
      }
      free(obj->progAttrs);
      obj->progAttrs = NULL;
    }
    free(obj);
    obj = NULL;
  }
}

void freeScope(Scope* scope) {
  // TODO
  if(scope != NULL) {
    freeObjectList(scope->objList);
    free(scope);
    scope = NULL;
  }
}

void freeObjectList(ObjectNode *objList) {
  // TODO
  if(objList != NULL){
    freeObject(objList->object);
    freeObjectList(objList->next);
    objList = NULL;
  }
}

void freeReferenceList(ObjectNode *objList) {
  // TODO
  if(objList != NULL){
    freeObject(objList->object);
    freeReferenceList(objList->next);
    objList = NULL;
  }
}

void AddObject(ObjectNode **objList, Object* obj) {
  ObjectNode* node = (ObjectNode*) malloc(sizeof(ObjectNode));
  node->object = obj;
  node->next = NULL;
  if ((*objList) == NULL) 
    *objList = node;
  else {
    ObjectNode *n = *objList;
    while (n->next != NULL) 
      n = n->next;
    n->next = node;
  }
}

Object* findObject(ObjectNode *objList, char *name) {
  // TODO
  ObjectNode *brow_node = objList;
  while(brow_node != NULL) {
    if(strcmp(brow_node->object->name, name) == 0) {
      return brow_node->object;
    }
    brow_node = brow_node->next;
  }
  return NULL;
}

/******************* others ******************************/

void initSymTab(void) {
  Object* obj;
  Object* param;

  symtab = (SymTab*) malloc(sizeof(SymTab));
  symtab->globalObjectList = NULL;
  
  obj = CreateFunctionObject("READC");
  obj->funcAttrs->returnType = makeCharType();
  AddObject(&(symtab->globalObjectList), obj);

  obj = CreateFunctionObject("READI");
  obj->funcAttrs->returnType = makeIntType();
  AddObject(&(symtab->globalObjectList), obj);

  obj = CreateProcedureObject("WRITEI");
  param = CreateParameterObject("i", PARAM_VALUE, obj);
  param->paramAttrs->type = makeIntType();
  AddObject(&(obj->procAttrs->paramList),param);
  AddObject(&(symtab->globalObjectList), obj);

  obj = CreateProcedureObject("WRITEC");
  param = CreateParameterObject("ch", PARAM_VALUE, obj);
  param->paramAttrs->type = makeCharType();
  AddObject(&(obj->procAttrs->paramList),param);
  AddObject(&(symtab->globalObjectList), obj);

  obj = CreateProcedureObject("WRITELN");
  AddObject(&(symtab->globalObjectList), obj);

  intType = makeIntType();
  charType = makeCharType();
}

void cleanSymTab(void) {
  freeObject(symtab->program);
  freeObjectList(symtab->globalObjectList);
  free(symtab);
  freeType(intType);
  freeType(charType);
}

void enterBlock(Scope* scope) {
  symtab->currentScope = scope;
}

void exitBlock(void) {
  symtab->currentScope = symtab->currentScope->outer;
}

void declareObject(Object* obj) {
  if (obj->kind == OBJ_PARAMETER) {
    Object* owner = symtab->currentScope->owner;
    switch (owner->kind) {
    case OBJ_FUNCTION:
      AddObject(&(owner->funcAttrs->paramList), obj);
      break;
    case OBJ_PROCEDURE:
      AddObject(&(owner->procAttrs->paramList), obj);
      break;
    default:
      break;
    }
  }
 
  AddObject(&(symtab->currentScope->objList), obj);
}


