#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/**
 * URL参数解析器
 * 输入：包含http/https超链接的字符串
 * 输出：解析出所有的key-value键值对，每行显示一个
 */

int parse_url(const char* url) {
    int err = 0;

    char* url_copy = strdup(url);
    if (!url_copy) {
        err = ENOMEM;
        goto exit;
    }

    char* query = strchr(url_copy, '?');
    if (!query) {
        err = 1;
        goto exit;
    }
    query++;

    char* params = strdup(query);
    if (!params) {
        err = ENOMEM;
        goto exit;
    }

    char* param = strtok(params, "&");
    while (param != NULL) {
        char* equal = strchr(param, '=');
        if (equal) {
            *equal = '\0';
            char* key = param;
            char* value = equal + 1;
            printf("key = %s, value = %s\n", key, value);
        }
        param = strtok(NULL, "&");
    }

    free(params);
exit:
    free(url_copy);
    return err;
}

int main() {
    const char* test_url = "https://cn.bing.com/search?name=John&age=30&city=New+York";

    printf("Parsing URL: %s\n", test_url);
    printf("Parameters:\n");

    parse_url(test_url);

    return 0;
}