#define _CRT_SECURE_NO_WARNINGS
#include<stdlib.h> // malloc free
#include<string.h> // memset;
#include <cassert>
#include"MyCjson.hpp"


//
static cJSON* cJSON_New_Item() {
	cJSON* node = (cJSON*)malloc(sizeof(cJSON));
	if (nullptr != node)
	{
		memset(node, 0, sizeof(cJSON));
	}
	return node;
}

//
static char* cJSON_strdup(const char* str) {
	size_t len = 0;
	char* copy = nullptr;
	if (nullptr == str) copy;
	len = strlen(str) + 1;
	copy = (char*)malloc(len);
	if (nullptr != copy)
	{
		strcpy(copy, str);
	}
	return copy;
}
//****************************************************************************************************

//
cJSON* cJSON_CreateNull() {
	cJSON* item = cJSON_New_Item();
	if (nullptr != item)
	{
		item->type = cJSON_NULL;
	}
	return item;
}

//
cJSON* cJSON_CreateTrue() {
	cJSON* item = cJSON_New_Item();
	if (nullptr != item)
	{
		item->type = cJSON_True;
	}
	return item;
}

//
cJSON* cJSON_CreateFalse() {
	cJSON* item = cJSON_New_Item();
	if (nullptr != item)
	{
		item->type = cJSON_False;
	}
	return item;
}

//
cJSON* cJSON_CreateBool(bool b) {
	cJSON* item = cJSON_New_Item();
	if (nullptr != item)
	{
		item->type = (b ? cJSON_True : cJSON_False);
	}
	return item;
}

//
cJSON* cJSON_CreateLong(long long num) {
	cJSON* item = cJSON_New_Item();
	if (nullptr != item)
	{
		item->type = cJSON_Long;
		item->valueint = num;
	}
	return item;
}

//
cJSON* cJSON_CreateDouble(double num) {
	cJSON* item = cJSON_New_Item();
	if (nullptr != item)
	{
		item->type = cJSON_Double;
		item->valuedouble = num;
	}
	return item;
}

//
cJSON* cJSON_CreateString(const char* string) {
	cJSON* item = cJSON_New_Item();
	if (nullptr != item)
	{
		item->type = cJSON_String;
		item->valuestring = cJSON_strdup(string);
	}
	return item;
}

//
cJSON* cJSON_CreateArray() {
	cJSON* item = cJSON_New_Item();
	if (nullptr != item)
	{
		item->type = cJSON_Array;
	}
	return item;
}

//
cJSON* cJSON_CreateObject() {
	cJSON* item = cJSON_New_Item();
	if (nullptr != item)
	{
		item->type = cJSON_Object;
	}
	return item;
}

//
void cJSON_AddItemToArray(cJSON* array, cJSON* item) {

	if (array->child == nullptr) {
		array->child = item;
	}
	else {
		cJSON* p = array->child;
		while (p->next != nullptr) {
			p = p->next;
		}
		p->next = item;
		item->prev = p;
	}
}

//
void cJSON_AddItemToObject(cJSON* obj, const char* name, cJSON* item) {
	assert(obj != nullptr);
	if (item->namestring != nullptr) {
		free(item->namestring);
	}
	item->namestring = strdup(name);
	cJSON_AddItemToArray(obj, item);
	/*if (obj->child == nullptr) {
		obj->child = item;
		
		item->namestring = strdup(name);
	}
	else {
		cJSON* op = obj->child;
		while (op->next != nullptr) {
			op = op->next;
		}
		op->next = item;
		item->prev = op;
		item->namestring = strdup(name);
	}*/
}

//将 item 转换为 字符串 
static char* print_value(cJSON* item, int depth, int fmt)
{
	char* out = 0;
	if (!item) return 0;
	//针对不同的成员有不同的 print_ 转换函数
	switch ((item->type) & 255)
	{
	case cJSON_NULL:	out = cJSON_strdup("null");	break;
	case cJSON_False:	out = cJSON_strdup("false"); break;
	case cJSON_True:	out = cJSON_strdup("true"); break;
	case cJSON_Long:	out = print_number(item); break;
	case cJSON_String:	out = print_string(item); break;
	case cJSON_Array:	out = print_array(item, depth, fmt); break;
	case cJSON_Object:	out = print_object(item, depth, fmt); break;
	}
	return out;
}

//
static char* print_number(cJSON* item) {

}

//
static char* print_string(cJSON* item) {

}

//
static char* print_string_ptr(char* s) {

}

//将对象转换为字符串
static char* print_object(cJSON* item, int dept, bool fmt) {
	int i = 0;
	int len = 5;
	bool fail = 0;
	int numentries = 0;

	char** entries = nullptr;
	char** names = nullptr;
	char* out = nullptr;
	char* ret = nullptr;
	char* ptr = nullptr;

	cJSON* p = item->child;
	//计算对象的元素个数
	while (p != nullptr) {
		p = p->next;
		numentries += 1;
	}

	if (numentries == 0) {
		out = (char*)malloc(sizeof(char) * 3);//
		if (out != nullptr) {
			strcpy(out, "{}");
		}
		return out;
	}

	entries = (char**)malloc(sizeof(char*) * numentries);//存储value的序列化字符串
	if (!entries) {
		return 0;
	}
	names = (char**)malloc(sizeof(char*) * numentries);//存储string的序列化字符串
	if (!names) {
		free(entries);
		return nullptr;
	}

	memset(entries, 0, numentries * sizeof(char*));
	memset(names, 0, numentries * sizeof(char*));
	
	p = item->child;
	while (p != nullptr) {

		ret = print_string_ptr(p->namestring);
		names[i] = ret;
		if (nullptr != ret) {
			// "name" : \0 
			len += strlen(ret) + 1 + 1 + (fmt ? 1 : 0);
		}
		else {
			fail = true;
			break;
		}


		ret = print_value(p, dept + 1, fmt);//递归调用
		entries[i++] = ret;
		if (nullptr != ret) {
			//"name" : \t "value" , \n
			len += strlen(ret) + 2 + (fmt ? 1 : 0);
		}
		else {
			break;
		}

		p = p->next;
	}

	out = (char*)malloc(sizeof(char) * len);
	dept += 1;
	*out = '{';
	ptr = out + 1;
	*ptr++ = '\n';

	for (i = 0; i < numentries; i++)
	{
		if (fmt) {
			for (int i = 0; i < dept; i++) {
				*ptr++ = '\t';
			}
		}

		strcpy(ptr, names[i]);
		ptr += strlen(names[i]);
		*ptr++ = ':';
		if (fmt) {
			*ptr++ = '\t';
		}

		strcpy(ptr, entries[i]);
		ptr += strlen(entries[i]);
		if (i != numentries - 1) {
			*ptr++ = ',';
			if (fmt) {
				*ptr++ = '\n';
			}
			*ptr++ = '\0';
		}
		//*ptr++ = '\n';

		free(entries[i]);
		free(names[i]);
	}
	free(entries);
	free(names);

	*ptr++ = '\n';
	*ptr++ = '}';
	*ptr++ = '\0';

	return out;
}

//将数组转换为字符串
static char* print_array(cJSON* item, int dept, bool fmt) {

	int i = 0;
	int numentries = 0;
	int len = 5;

	char** entries = nullptr;
	char* out = nullptr;
	char* ret = nullptr;
	char* ptr = nullptr;

	cJSON* p = item->child;

	//计算数组的元素个数
	while (p != nullptr) {
		p = p->next;
		numentries += 1;
	}
	
	//处理没有数组元素的情况
	if (numentries == 0) {
		out = (char*)malloc(sizeof(char) * 3);// 为 [ ] /0 3个元素 分配内存
		if (out != nullptr) {
			strcpy(out, "[]");//为内存添加内容
		}
		return out;
	}
	
	entries = (char**)malloc(sizeof(char*) * numentries);//
	if (!entries) {
		return 0;
	}
	memset(entries, 0, numentries * sizeof(char*));//将二维数组的内存以0覆盖

	p = item->child;
	while (p != nullptr) {

		ret = print_value(p, dept + 1, fmt);//递归调用 （对每个元素进行序列化）

		if (nullptr != ret) {
			len += strlen(ret) + 2 + (fmt ? 1 : 0);//元素本身长度 + '/0' + ',' + 是否格式化 （ '/t' ）
		}
		else {
			break;
		}

		entries[i] = ret;
		i++;
		p = p->next;
	}
	out = (char*)malloc(sizeof(char) * len);//为输出序列 分配实际内存空间

	*out = '[';
	ptr = out + 1;

	for (i = 0; i < numentries; i++)
	{
		strcpy(ptr, entries[i]);//
		ptr += strlen(entries[i]);

		//只要不是最后一个，就在其后添加 ',' 以及是否格式化 '/t'
		if (i != numentries - 1) {
			*ptr++ = ',';
			if (fmt) {
				*ptr++ = ' ';
			}
			*ptr = 0; 
		}
		free(entries[i]);//同时释放空间
	}
	free(entries);//最后释放总空间

	*ptr++ = ']';
	*ptr++ = 0;
	
	return out;
}