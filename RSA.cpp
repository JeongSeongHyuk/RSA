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

// Extended Euclidean Algorithm�� ���� ���� ���ϴ� �Լ�.
long long inverse(long long mod, long long num)
{
	long long p1 = 0, p2 = 1, p, temp = mod;
	long long remainder, portion;

	while (num != 1) // �������� 1�� �� �� ���� �ݺ��Ѵ�.
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

	return p; // While���� �� ���� ���� p���� ���� ������ �ȴ�.
}

// base, exponent, mod�� ���� ū ���ڿ� ���� mod���� ���Ѵ�.
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

// Miller-Rabin �˰����� ���� �Ҽ��Ǻ��� �Ѵ�.
bool miller_rabin(long long n)
{
	long long a, s = 0, d = 1, x, temp = n-1;
	bool flag = false;

	// n�� ¦���� ��� ���� �ڵ带 �������� �ʰ� �Ҽ��� �ƴ��� �˸���.
	if (n % 2 == 0)
		return false;
	
	// n-1�� (2^s)*d�� ��Ÿ����.
	while (temp % 2 != 1)
	{
		s++;
		temp /= 2;
	}
	d = temp;

	for (int i = 0; i < 20; i++)
	{
		a = rand() % (n - 3) + 2; // a�� 2���� n-1���� �� ������ ���� �����Ѵ�.
		x = modular_exponentiation(a, d, n); // x = (a^d) mod n ���� ���Ѵ�.
		if (x == 1 || x == n - 1) // x�� �ش� ������ ��� ���� �ڵ带 �������� �ʰ� ������ �ѹ� �� ����.
			continue;
		for (int r = 1; r < s; r++) // r�� 1���� s-1�� �� ���� �ռ��� �Ǻ��� �����Ѵ�.
		{
			x = modular_exponentiation(x, 2, n); // ���� ���� ������� x�� ���ϰ�
			if (x == 1) // x�� 1�� ��� �ռ����� �Ǻ�, 
				return false;
			if (x == n - 1) // x�� n-1�� ��� 20�� �����ϴ� ������ �ݺ��Ѵ�.
			{
				flag = true;
				break;
			}
		}
		if (flag) // x�� n-1�� ��� setting�� flag�� ���� return false�� ���� �ʰ� ������ �ѹ� �� ����.
			continue;

		return false; // s���� ������ �� ���� ���� ��� �ռ����� �Ǻ��Ѵ�.
	}
	return true; // 20���� ������ �� ���� ���� ��� 99.9%Ȯ���� �Ҽ��̹Ƿ� �Ҽ��� ������ �˸���.
}
void KeySetting()
{
	do
	{
		p = rand() % MAX_15bit + MIN_15bit; // 15-bit ������ ��
	} while (!miller_rabin(p)); // Miller-Rabin �˰����� ���� �Ҽ��� ������ ���ڸ� �����Ѵ�.
	do
	{
		q = rand() % MAX_15bit + MIN_15bit; // 15-bit ������ ��
	} while (!miller_rabin(q)); // Miller-Rabin �˰����� ���� �Ҽ��� ������ ���ڸ� �����Ѵ�.

	N = p * q;
	phi = (p - 1) * (q - 1);
	e = 257;
	printf("p = %lld\nq = %lld\nn = %lld\nphi = %lld\ne = %lld\n", p, q, N, phi, e);
	d = inverse(phi, e); // Extended Euclidean Algorithm�� ���� ������ ���Ѵ�.
	printf("d = %lld\n", d);
}
void RSA()
{
	KeySetting(); // RSA�� �ʿ��� �������� �����Ѵ�.

	printf("Message Input : ");
	Msg = 12345;
	printf("Message = %lld\n", Msg);
	printf("**Encryption\n");
	Cip = modular_exponentiation(Msg, e, N); // Ciper ����
	printf("cipher = %lld\n", Cip);
	printf("**Decryption\n");
	Msg = modular_exponentiation(Cip, d, N); // Decryption ����
	printf("decrypted cipher : %lld\n", Msg);
}
int main()
{
	srand(time(NULL));

	RSA();

	return 0;
}