#include "../include/strlib.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define U (unsigned char*)
#define C (char*)
#define T(COND) {tests++; if(!(COND)) {ret++; puts("test " #COND  " failed");}}

int main(int argc, char** argv) {
	int ret = 0, tests = 0;
	char b64str[64+1];
	unsigned char raw[BASE64DEC_BYTES(64)+1];

	T(1 == 1);

	T(BASE64ENC_BYTES(0) == 0);
	T(BASE64ENC_BYTES(1) == 4);
	T(BASE64ENC_BYTES(2) == 4);
	T(BASE64ENC_BYTES(3) == 4);
	T(BASE64ENC_BYTES(4) == 8);
	T(BASE64ENC_BYTES(5) == 8);
	T(BASE64ENC_BYTES(6) == 8);
	T(BASE64ENC_BYTES(7) == 12);

	T(BASE64DEC_BYTES(12) == 9);
	T(BASE64DEC_BYTES(8) == 6);
	T(BASE64DEC_BYTES(4) == 3);

	base64enc(b64str, U"a", 1);
	T(str_equal(b64str, "YQ=="));
	T(base64dec(raw, b64str, sizeof raw) == 1);
	T(str_equal(C raw, "a"));

	base64enc(b64str, U"aa", 2);
	T(str_equal(b64str, "YWE="));
	T(base64dec(raw, b64str, sizeof raw) == 2);
	T(str_equal(C raw, "aa"));

	base64enc(b64str, U"aaa", 3);
	T(str_equal(b64str, "YWFh"));
	T(base64dec(raw, b64str, sizeof raw) == 3);
	T(str_equal(C raw, "aaa"));

	T(base64enc_str(b64str, U"AAAA", sizeof 2) == 0);
	T(base64enc_str(b64str, U"AAAA", sizeof b64str) == 1);
	T(str_equal(b64str, "QUFBQQ=="));
	T(base64dec(raw, b64str, sizeof raw) == 4);
	T(str_equal(C raw, "AAAA"));

	base64enc(b64str, U"AAAA", 4);
	T(str_equal(b64str, "QUFBQQ=="));
	T(base64dec(raw, b64str, sizeof raw) == 4);
	T(str_equal(C raw, "AAAA"));

	base64enc(b64str, U"\x01\x02\x03\x04\x05", 5);
	T(str_equal(b64str, "AQIDBAU="));
	T(base64dec(raw, b64str, sizeof raw) == 5);
	T(str_equal(C raw, "\x01\x02\x03\x04\x05"));

	T(base64dec(raw, "=QIDBAU=", sizeof raw) == 0);
	T(base64dec(raw, "AQ(DBAU=", sizeof raw) == 0);
	T(base64dec(raw, "AQIDB@U=", sizeof raw) == 0);
	T(base64dec(raw, "AQIDBAU==", sizeof raw) == 0);
	T(base64dec(raw, "AQIDB===", sizeof raw) == 0);

	base64enc(b64str, U"\x26\xC7\x1B\x28\xFE\xB0\x90\x08\x5A\xDE\xC6\xE0\xF0\xF4\x76\xDD", 16);
	T(str_equal(b64str, "JscbKP6wkAha3sbg8PR23Q=="));
	T(base64dec(raw, b64str, sizeof raw) == 16);
	T(str_equal(C raw, "\x26\xC7\x1B\x28\xFE\xB0\x90\x08\x5A\xDE\xC6\xE0\xF0\xF4\x76\xDD"));

	base64enc(b64str, U"\xF3\x9B\x70\xA5\x63\xFA\x54\x86\xD1", 9);
	T(str_equal(b64str, "85twpWP6VIbR"));
	T(base64dec(raw, b64str, sizeof raw) == 9);
	T(str_equal(C raw, "\xF3\x9B\x70\xA5\x63\xFA\x54\x86\xD1"));


	printf("%d out of %d tests failed.\n", ret, tests);

	return ret;
}
