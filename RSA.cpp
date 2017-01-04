#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#define MAX_15bit 32767
#define MIN_15bit 16384
#pragma warning(disable:4996)

using namespace std;
long long p, q, N, phi, e, d, Msg, Cip;

// Extended Euclidean Algorithm을 통한 역원 구하는 함수.
long long inverse(long long mod, long long num)
{
	long long p1 = 0, p2 = 1, p, temp = mod;
	long long remainder, portion;

	while (num != 1) // 나머지가 1이 될 때 까지 반복한다.
	{
		remainder = mod%num;
		portion = mod / num;
		p = (p1 - p2*portion) % temp;
		if (p < 0)
			p += temp;
		mod = num;
		num = remainder;
		p1 = p2;
		p2 = p;
	}

	return p; // While문을 다 돌고 나온 p값이 최종 역원이 된다.
}

// base, exponent, mod를 통해 큰 숫자에 대한 mod값을 구한다.
long long modular_exponentiation(long long base, long long exponent, long long mod)
{
	long long result = 1;
	while (exponent > 0)
	{
		if (exponent % 2 == 1)
			result = (result*base) % mod;
		exponent = exponent >> 1;
		base = (base*base) % mod;
	}
	return result;
}

// Miller-Rabin 알고리즘을 통해 소수판별을 한다.
bool miller_rabin(long long n)
{
	long long a, s = 0, d = 1, x, temp = n-1;
	bool flag = false;

	// n이 짝수일 경우 하위 코드를 실행하지 않고 소수가 아님을 알린다.
	if (n % 2 == 0)
		return false;
	
	// n-1을 (2^s)*d로 나타낸다.
	while (temp % 2 != 1)
	{
		s++;
		temp /= 2;
	}
	d = temp;

	for (int i = 0; i < 20; i++)
	{
		a = rand() % (n - 3) + 2; // a는 2부터 n-1까지 중 임의의 수로 지정한다.
		x = modular_exponentiation(a, d, n); // x = (a^d) mod n 으로 구한다.
		if (x == 1 || x == n - 1) // x가 해당 숫자일 경우 하위 코드를 진행하지 않고 루프를 한번 더 돈다.
			continue;
		for (int r = 1; r < s; r++) // r이 1부터 s-1번 돌 동안 합성수 판별을 진행한다.
		{
			x = modular_exponentiation(x, 2, n); // 위와 같은 방법으로 x를 구하고
			if (x == 1) // x가 1일 경우 합성수로 판별, 
				return false;
			if (x == n - 1) // x가 n-1일 경우 20번 진행하는 루프를 반복한다.
			{
				flag = true;
				break;
			}
		}
		if (flag) // x가 n-1일 경우 setting된 flag을 통해 return false를 하지 않고 루프를 한번 더 돈다.
			continue;

		return false; // s번의 루프를 다 돌고 나올 경우 합성수로 판별한다.
	}
	return true; // 20번의 루프를 다 돌고 나올 경우 99.9%확률로 소수이므로 소수가 맞음을 알린다.
}
void KeySetting()
{
	do
	{
		p = rand() % MAX_15bit + MIN_15bit; // 15-bit 범위의 수
	} while (!miller_rabin(p)); // Miller-Rabin 알고리즘을 통해 소수인 임의의 숫자를 선별한다.
	do
	{
		q = rand() % MAX_15bit + MIN_15bit; // 15-bit 범위의 수
	} while (!miller_rabin(q)); // Miller-Rabin 알고리즘을 통해 소수인 임의의 숫자를 선별한다.

	N = p * q;
	phi = (p - 1) * (q - 1);
	e = 257;
	printf("p = %lld\nq = %lld\nn = %lld\nphi = %lld\ne = %lld\n", p, q, N, phi, e);
	d = inverse(phi, e); // Extended Euclidean Algorithm을 통해 역원을 구한다.
	printf("d = %lld\n", d);
}
void RSA()
{
	KeySetting(); // RSA에 필요한 변수들을 세팅한다.

	printf("Message Input : ");
	Msg = 12345;
	printf("Message = %lld\n", Msg);
	printf("**Encryption\n");
	Cip = modular_exponentiation(Msg, e, N); // Ciper 연산
	printf("cipher = %lld\n", Cip);
	printf("**Decryption\n");
	Msg = modular_exponentiation(Cip, d, N); // Decryption 연산
	printf("decrypted cipher : %lld\n", Msg);
}
int main()
{
	srand(time(NULL));

	RSA();

	return 0;
}