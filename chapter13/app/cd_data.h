
/* 目录表 */
#define CAT_CAT_LEN       30
#define CAT_TITLE_LEN     70
#define CAT_TYPE_LEN      30
#define CAT_ARTIST_LEN    70

typedef struct {
    char catalog[CAT_CAT_LEN + 1];
    char title[CAT_TITLE_LEN + 1];
    char type[CAT_TYPE_LEN + 1];
    char artist[CAT_ARTIST_LEN + 1];
} cdc_entry;

/* 跟踪表,一个条目对应一个跟踪条目。The tracks table, one entry per track */

#define TRACK_CAT_LEN     CAT_CAT_LEN
#define TRACK_TTEXT_LEN   70

typedef struct {
    char catalog[TRACK_CAT_LEN + 1];
    int  track_no;
    char track_txt[TRACK_TTEXT_LEN + 1];
} cdt_entry;

 /* 现在,我们有一些数据结构,我们可以定义一些我们需要访问例程。
    cdc_函数与目录条目相关;函数cdt_与跟踪条目相关。
	注意,函数返回的数据结构。
	我们通过使返回的结构内容为空来表示函数的失败 */

/* 初始化和终止函数 */
int database_initialize(const int new_database);
void database_close(void);

/* 两个简单的数据检索函数*/
cdc_entry get_cdc_entry(const char *cd_catalog_ptr);
cdt_entry get_cdt_entry(const char *cd_catalog_ptr, const int track_no);

/* 两个数据添加函数 */
int add_cdc_entry(const cdc_entry entry_to_add);
int add_cdt_entry(const cdt_entry entry_to_add);

/* 两个数据删除函数 */
int del_cdc_entry(const char *cd_catalog_ptr);
int del_cdt_entry(const char *cd_catalog_ptr, const int track_no);

/* 搜索函数 */
cdc_entry search_cdc_entry(const char *cd_catalog_ptr, int *first_call_ptr);
