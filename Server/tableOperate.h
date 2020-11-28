// テーブルの内容を読み込んで出力
char *tableSelect(char *tblName, int *count);
// テーブルへのデータ挿入
char *tableInsert(char *tblName, char **data, int *count);
// テーブルのデータ更新
char *tableUpdate(char *tblName, char **updateList, char **conditionList, int updateListCount, int conditionListCount, int *count);
// テーブルのデータ削除
char *tableDelete(char *tblName, char **conditionList, int conditionListCount, int *count);
