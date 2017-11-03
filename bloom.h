#ifndef BLOOM_H_INCLUDED
#define BLOOM_H_INCLUDED

#include<stdlib.h>
#include<assert.h>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<math.h>
#include<string.h>
#include<rpc/des_crypt.h>	/*for ecd_crypt */
#include<limits.h>		/*for INT_MAX */
#include<errno.h>
#include<string>
using namespace std;

#define MAX_URLPATH_NUM	10000000	/*url path数目的上限(必须是1000的整倍数) */

int p_table[MAX_URLPATH_NUM] = { 0 };

int myEncrypt(char *str, char *key){
	assert(str != NULL);
	char *cipher = strdup(str);
	/*  int ecb_crypt(char *key, char *data, \
                   unsigned datalen,unsigned mode);
    */
	ecb_crypt(key, cipher, strlen(str), DES_ENCRYPT);	/*第一次映射函数采用ecb_crypt */
	int i;
	int var = 0;
	int len = strlen(cipher);
	for (i = 0; i < len; i++){
		var = (var * 7 + cipher[i]) % (int)INT_MAX;
	}
	free(cipher);
	return var;
}

/*判断path是否存在过，如果不存在就把它记入PathBloomTable中*/
int bloomFilter(char *path){

	int mod = 33 * MAX_URLPATH_NUM;
	int flag = 0;
	string salt[] = { "Dm", "VB", "ui", "LK", "uj", "RD", "we", "fc" };
	int f[8] = { 0 };
	int g[8] = { 0 };
	int i;
	for (i = 0; i < 8; i++) {
		char *key;
		key = strdup(salt[i].c_str());
		f[i] = myEncrypt(path, key);
		free(key);
		srand(f[i]);
		g[i] = rand() % mod;	/* rand() * 33 * MAX_URLPATH_NUM */
		int index = g[i] / 33;
		int pos = g[i] % 33;
		if (p_table[index] & (0x80000000 >> pos))//如果与p_table[index]相同
			flag++;
		else
			p_table[index] |= (0x80000000 >> pos);//不同，则添加这一位
	}
	if (flag == 8){
		return 1;
    }//已存在则返回true
	return 0;
}




#endif // BLOOM_H_INCLUDED
