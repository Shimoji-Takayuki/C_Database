// テーブルの内容を読み込んで出力
size_t split(char *target, const char *separator, char **result);

// int型配列に指定の数値が入っているindexを取得
int containInt(int *intAry, int count, int target);

// 文字列配列に指定の文字列が入っているindexを取得
int containStr(char **strAry, int count, char *target);

// システム日時を取得し、YYYYMMDDHHMISSの形で返す
void getSystemDate(char *systemDate);

// 文字列を比較（大文字小文字無視）
int strcmpIgnoreCase(const char *s1, const char *s2);
