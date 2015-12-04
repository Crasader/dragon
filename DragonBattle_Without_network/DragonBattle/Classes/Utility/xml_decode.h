#ifndef XML_DECODE_H__
#define XML_DECODE_H__

/* 后台解析XML文件接口 */

// C89
#include <string.h>
// C99
#include <stdint.h>
// Posix
#include <arpa/inet.h>
#include <libxml/tree.h>


#ifdef WONPEE_SERVER
extern "C"
{
#include <libwonpee/log.h>
}
#else
#include "plat_diff.h"
#endif

#define JUMP_XML_TAG(cur, str) \
    if (!cur || xmlStrcmp(cur->name, (const xmlChar *)str))  \
        ERROR_RETURN (("xmlStrcmp error: tag=%s", str), -1);

#define DECODE_XML_PROP(cur, prop, str) \
    str = xmlGetProp(cur, (xmlChar*)prop);  \
    if (!str) { \
        /*ERROR_LOG("xml parse error: prop=%s", prop); */\
        return false; \
    }


#define DECODE_XML_PROP_INT_DEFAULT(v, cur, prop, def) \
    do { \
        xmlChar *str; \
        if (!cur || !(str = xmlGetProp(cur, (xmlChar*)prop))) { \
            v = def; \
        } else { \
            v = atoi ((const char*)str); \
            xmlFree (str); \
        } \
    } while (0)

#define DECODE_XML_PROP_INT(v, cur, prop) \
    do { \
        xmlChar *str; \
        DECODE_XML_PROP(cur, prop, str); \
        v = atoi ((const char*)str); \
        xmlFree (str); \
    } while (0)

#define DECODE_XML_PROP_UINT32(v, cur, prop) \
    do { \
        xmlChar *str; \
        DECODE_XML_PROP(cur, prop, str); \
        char* endpt; \
        v = strtoul((const char*)str, &endpt, 10); \
        xmlFree (str); \
    } while (0)

#define DECODE_XML_PROP_INT64(v, cur, prop) \
	do {\
		xmlChar *str; \
		DECODE_XML_PROP(cur, prop, str); \
		char* endpt; \
		v = strtoll((const char*)str, &endpt, 10); \
		xmlFree(str);\
	} while (0)

#define DECODE_XML_PROP_UINT64(v, cur, prop) \
	do {\
		xmlChar *str; \
		DECODE_XML_PROP(cur, prop, str); \
		char* endpt; \
		v = strtoull((const char*)str, &endpt, 10); \
		xmlFree(str);\
	} while (0)

#define DECODE_XML_PROP_FLOAT(v, cur, prop) \
    do {\
        xmlChar* str;\
        DECODE_XML_PROP(cur, prop, str); \
        v = atof((const char*)str);\
        xmlFree(str);\
    } while(0)

#define DECODE_XML_PROP_FLOAT_DEFAULT(val, cur, prop, def) \
    do {\
        xmlChar* str;\
        if (!cur || !(str = xmlGetProp(cur, (xmlChar*)prop))) {\
            val = def;\
        } else {\
            val = atof ((const char*)str);\
            xmlFree(str);\
        }\
    } while(0)


#define DECODE_XML_PROP_ARR_INT(arr_, len_, cur_, prop_) \
    do { \
        xmlChar* str; \
        DECODE_XML_PROP((cur_), (prop_), str); \
        int i = 0, cnt = 0, k; \
        size_t slen = strlen((const char*)str); \
        for (; (i != (len_)) && (cnt != slen); ++i, cnt += k) { \
            sscanf((const char*)str + cnt, "%d%n", &((arr_)[i]), &k); \
        } \
        xmlFree (str); \
    } while (0)

#define DECODE_XML_PROP_STR(v, cur, prop) \
    do { \
        xmlChar *str; \
        DECODE_XML_PROP(cur, prop, str); \
        strncpy (v, (const char*)str, sizeof (v) - 1); \
        v[sizeof(v) - 1] = '\0'; \
        xmlFree (str); \
    } while (0)

#define DECODE_XML_PROP_STR_DEFAULT(v, cur, prop, def) \
    do {\
        xmlChar* str;\
        if (!cur || !(str = xmlGetProp(cur, (xmlChar*)prop))) {\
            strncpy(v, def, sizeof(v) - 1);\
        } else {\
            strncpy (v, (const char*)str, sizeof (v) - 1);\
        }\
        v[sizeof(v) - 1] = '\0';\
        xmlFree(str);\
    } while(0)

static inline void
decode_xml_prop_uint32_default(uint32_t* val, xmlNodePtr cur, void* prop, uint32_t def)
{
    xmlChar* str;
    if (!cur || !(str = xmlGetProp(cur, (xmlChar*)prop))) {
        *val = def;
    } else {
        char* endpt;
        *val = strtoul((const char*)str, &endpt, 10);
        xmlFree (str);
    }
}

static inline size_t
decode_xml_prop_food_int_default(int arr[], size_t len, xmlNodePtr cur, void* prop, uint32_t def)
{
    int k;
    char c[16];
    size_t i = 0;
    
    xmlChar* str;
    for (i=0; i != len; ++i) {
        arr[i] = def;
    }
    
    if (!cur || !(str = xmlGetProp(cur, (xmlChar*)prop))) {
        return 0;
    }
    
    size_t idx = 0;
    int itmid = 0;
    size_t cnt = 0;
    size_t slen = strlen((const char*)str);
    for (i=0; (idx != len) && (cnt < slen); ++i, cnt += k) {
        sscanf((const char*)str + cnt, "%d%n", &itmid, &k);
        cnt += k;
        if (cnt < slen) {
            sscanf((const char*)str + cnt, "%[^0-9]%n", c, &k);
        }
        //sscanf((const char*)str + cnt, "%d%[^0-9]%n", &itmid, c, &k);
        if(itmid == 0) break;
        if(arr[idx] == itmid){
            arr[idx+1]++;
        } else {
            if(arr[idx]) idx += 2;
            arr[idx] = itmid;
            arr[idx+1] = 1;
        }
    }
    xmlFree(str);
    return i;
}

static inline size_t
decode_xml_prop_arr_int_default(int arr[], size_t len, xmlNodePtr cur, void* prop, int def)
{
    int k;
    char c[16];
    size_t i = 0;
    
    xmlChar* str;
    for (i=0; i != len; ++i) {
        arr[i] = def;
    }
    
    if (!cur || !(str = xmlGetProp(cur, (xmlChar*)prop))) {
        return 0;
    }
    
    size_t cnt = 0;
    size_t slen = strlen((const char*)str);
    for (i=0; (i != len) && (cnt < slen); ++i, cnt += k) {
        //sscanf((const char*)str + cnt, "%d%[^0-9]%n", &(arr[i]), c, &k);
        sscanf((const char*)str + cnt, "%d%n", &(arr[i]), &k);
        cnt += k;
        if (cnt < slen) {
            sscanf((const char*)str + cnt, "%[^0-9]%n", c, &k);
        }
    }
    xmlFree(str);
    return i;
}

static inline size_t
decode_xml_prop_arr_float_default(float arr[], size_t len, xmlNodePtr cur, void* prop, float def)
{
    int k;
    size_t i = 0;
    
    xmlChar* str;
    for (i=0; i != len; ++i) {
        arr[i] = def;
    }
    
    if (!cur || !(str = xmlGetProp(cur, (xmlChar*)prop))) {
        return 0;
    }
    
    size_t cnt = 0;
    size_t slen = strlen((const char*)str);
    for (i=0; (i != len) && (cnt < slen); ++i) {
        int symbol = 1;
        if (str[cnt] == '-')
        {
            symbol = -1;
            cnt++;
        }
        sscanf((const char*)str + cnt, "%f%n", &(arr[i]), &k);
        arr[i] = symbol * arr[i];
        cnt += k;
        
        while ((str[cnt] < '0' || str[cnt] > '9') && str[cnt] != '-' && cnt < slen)
        {
            cnt++;
        }
    }
    xmlFree(str);
    return i;
}

static inline void
decode_xml_prop_float_default(float* val, xmlNodePtr cur, void* prop, float def)
{
    xmlChar* str;
    if (!cur || !(str = xmlGetProp(cur, (xmlChar*)prop))) {
        *val = def;
    } else {
        sscanf((const char*)str, "%f", val);
        xmlFree(str);
    }
}

static inline void
decode_xml_prop_str(char* val, xmlNodePtr cur, void* prop)
{
    xmlChar* str;
    if (!cur || !(str = xmlGetProp(cur, (xmlChar*)prop))) {
    } else {
        sscanf((const char*)str, "%s", val);
        xmlFree(str);
    }
}

#endif
