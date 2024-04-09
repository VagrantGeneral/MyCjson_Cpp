#ifndef MYCJSON_HPP
#define MYCJSON_HPP
typedef enum
{
	cJSON_False = 0,
	cJSON_True = 1,
	cJSON_NULL = 2,
	cJSON_Long = 3,
	cJSON_Double = 4,
	cJSON_String = 5,
	cJSON_Array = 6,
	cJSON_Object = 7
}cJSON_Type;

typedef struct cJSON
{
	struct cJSON* prev, * next;
	struct cJSON* child;
	cJSON_Type type; //	 
	char* valuestring; //
	long long valueint;
	double valuedouble;
	char* namestring;
}cJSON;

extern cJSON* cJSON_CreateNull(void);
extern cJSON* cJSON_CreateTrue(void);
extern cJSON* cJSON_CreateFalse(void);
extern cJSON* cJSON_CreateBool(bool b);
extern cJSON* cJSON_CreateLong(long long num);
extern cJSON* cJSON_CreateDouble(double num);
extern cJSON* cJSON_CreateString(const char* string);
extern cJSON* cJSON_CreateArray(void);
extern cJSON* cJSON_CreateObject(void);

extern void cJSON_AddItemToArray(cJSON* array, cJSON* item);
extern void cJSON_AddItemToObject(cJSON* obj, const char* name, cJSON* item);

#endif 
